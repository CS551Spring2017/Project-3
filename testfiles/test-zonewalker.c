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

int main() {
	message m;
	m.m1_i1 = 2;
	m.m1_i2 = 2;
	printf("Printing all used zones:\n");
	_syscall(VFS_PROC_NR, ZONEBITMAPWALKER, &m);

}