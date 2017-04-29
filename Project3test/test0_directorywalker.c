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
	lstat(s,&s_path);
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

				char mountpoint[500] = "";
				strcpy(mountpoint,path);
				
				struct stat st;
				lstat(name, &st);
				
				textIndent(depth);
				printf("%s:\n", name);
				
				message m;
				m.m1_i1 = depth;
				m.m1_i2 = st.st_ino;
				m.m1_i3 = st.st_dev;
				
				/*
				if (strncmp(path, "/sys", 4) == 0) {
					m.m2_i1 = 1;
				}
				else if (strncmp(path, "/usr", 4) == 0) {
					m.m2_i1 = 2;
				}
				else if (strncmp(path, "/home", 5) == 0) {
					m.m2_i1 = 3;
				}
				else if (strncmp(path, "/proc", 5) == 0) {
					m.m2_i1 = 4;
				}
				else if (strncmp(path, "/", 1) == 0) {
					m.m2_i1 = 5;
				}
				
				if (S_ISDIR(st.st_mode) == 1 || S_ISREG(st.st_mode) == 1) {
					_syscall(VFS_PROC_NR, DIRECTORYWALKER, &m);

					(*size)++;
				}
				*/
				m.m2_i1 = 2;
				_syscall(VFS_PROC_NR, DIRECTORYWALKER, &m);
				(*size)++;
				
				//textIndent(depth);
				//printf("[debug]inode:%llu\n", st.st_ino);
				//textIndent(depth);
				//printf("[debug]dev:%d\n", st.st_dev);
				
				directoryWalker(name, size, depth + 1); //recursive call
			}
		}
		closedir(rep);
	}
}

int main(int argc, char *argv[]){
	int size = 0;
	//char *path = "/usr";
	char path[500] = "";
	if (argc == 1) {
		printf("Enter a directory as an argument.\n");
	}
	else {
		strcpy(path, argv[1]);
		if (strncmp(path, "/usr", 4) == 0) {
			if (isDir(path)) {
				directoryWalker(path, &size, 0);
				printf("Number of files = %d\n", size);
			}
			else {
				printf("%s is not a a directory.\n", path);
			}
		}
		else {
			printf("Only /usr (and subdirectories) are supported.\n");
		}
	}
}