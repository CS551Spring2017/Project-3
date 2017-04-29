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

int main(int argc, char *argv[]){
	
	if (argc == 1) {
		printf("Enter an inode number as an argument.\n");
		return 0;
	}
	else {
		int inode = atoi(argv[1]);
		if(inode == 0 || inode == 1){
			printf("Unable to modify Inode 0 or 1\n");
			return 0;
		}
		message m;
		m.m1_i1 = inode;
		
		printf("Damaging Inode Map:\n");
		_syscall(VFS_PROC_NR, DAMAGEINODEMAP, &m);
	}
	return 0;
}