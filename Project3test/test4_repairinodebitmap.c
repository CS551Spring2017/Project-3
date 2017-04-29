#include <lib.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <minix/drivers.h>
#include <minix/vtreefs.h>
#include <sys/stat.h>
#include <time.h>
#include <assert.h>
#include <dirent.h>
#include <string.h>

void directoryWalkerInodes(char* path, int *inodeValid);
int isDir(char *s);

int isDir(char *s) {
	struct stat s_path;
	lstat(s,&s_path);
	return S_ISDIR(s_path.st_mode);
}

void directoryWalkerInodes(char* path, int *inodeValid)
{
	if(isDir(path))
	{
		DIR *rep = opendir(path);
		struct dirent* ent = NULL;
		while ((ent = readdir(rep)) != NULL) {
			if (strcmp(ent->d_name,".") == 0 || strcmp(ent->d_name,"..") == 0) {
				//do nothing
			}	
			else {
				char name[500] = "";
				strcat(name,path);
				strcat(name,"/");
				strcat(name,ent->d_name);

				struct stat st;
				lstat(name, &st);
				inodeValid[st.st_ino] = 1;
				directoryWalkerInodes(name, inodeValid); //recursive call
			}
		}
		closedir(rep);
	}
}

int main() {
	printf("Fixing Inode Map...\n");
	int numErrors = 0;
	message m;
	m.m1_i1 = -1;
	m.m1_i2 = -1;
	int max_inodes = _syscall(VFS_PROC_NR, FIXINODEMAP, &m);
	printf("max_inodes: %d\n",max_inodes);
	int inodeValid[max_inodes];
	for(int i = 0; i < max_inodes; i++) 
	{	
		inodeValid[i]=0;
	}
	directoryWalkerInodes("/usr",inodeValid);
	printf("Finished directoryWalker\n");
	for(int i = 2; i < max_inodes; i++)
	{
		m.m1_i1 = i;
		m.m1_i2 = inodeValid[i];
		numErrors += _syscall(VFS_PROC_NR, FIXINODEMAP, &m);
	}
	message m2;
	_syscall(VFS_PROC_NR, SYNC, &m2);
	printf("InodeBitMap Recovery found and fixed %d errors\n", numErrors);
}