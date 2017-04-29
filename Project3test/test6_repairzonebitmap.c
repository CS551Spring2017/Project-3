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

void directoryWalkerZones(char* path);
int isDir(char *s);

int isDir(char *s) {
	struct stat s_path;
	stat(s,&s_path);
	return S_ISDIR(s_path.st_mode);
}

void directoryWalkerZones(char* path)
{
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
				
				message m;
				m.m1_i1 = st.st_ino;
				m.m1_i2 = st.st_dev;
				_syscall(VFS_PROC_NR, FIXZONEMAP, &m);
				
				directoryWalkerZones(name); //recursive call
			}
		}
		closedir(rep);
	}
}

int main() {
	printf("Fixing Zone Map...\n");
	directoryWalkerZones("/usr/testfiles/test/TestFiles/");
	printf("Finished Fixing Zone Map.\n");
	return 0;

}