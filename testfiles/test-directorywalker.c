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

void directoryWalker(char* path, int *size, int depth);
int isDir(char *s);
void testIndent(int depth);

int isDir(char *s) {
	struct stat s_path;
	stat(s,&s_path);
	return S_ISDIR(s_path.st_mode);
}

void textIndent(int depth) {
	for (int i = 0; i < depth; i++) {
		printf("  ");
	}
}

void directoryWalker(char* path, int *size, int depth) {
	if (isDir(path)) {
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
				stat(name, &st);
				
				textIndent(depth);
				printf("%s:\n", name);
				
				message m;
				m.m1_i1 = depth;
				m.m1_i2 = st.st_ino;
				m.m1_i3 = st.st_dev;
				//textIndent(depth);
				//printf("[debug]inode:%llu\n", st.st_ino);
				//textIndent(depth);
				//printf("[debug]dev:%d\n", st.st_dev);
				
				//m.m1_i1 = 4;
				_syscall(VFS_PROC_NR, DIRECTORYWALKER, &m);

				(*size)++;
				directoryWalker(name, size, depth + 1); //recursive call
			}
		}
		closedir(rep);
	}
}

int main() {
	int size = 0;
	char *path = "/usr/testfiles/test";
	directoryWalker(path, &size, 0);
	printf("Number of files = %d\n", size);
}