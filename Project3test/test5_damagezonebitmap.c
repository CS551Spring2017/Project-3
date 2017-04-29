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
	}
	else {
		message m;
		m.m1_i1 = atoi(argv[1]);
		
		printf("Damaging Zone Map:\n");
		_syscall(VFS_PROC_NR, DAMAGEZONEMAP, &m);
	}
}