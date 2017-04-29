#include "fs.h"
#include <stdio.h>
#include <string.h>
#include <minix/vfsif.h>
#include "vmnt.h"
#include "vnode.h"
#include "path.h"
#include "fproc.h"

int do_directorywalker(){
	message m;
	m.m_type = REQ_DIRECTORYWALKER;

	m.m1_i1 = m_in.m1_i1;
	m.m1_i2 = m_in.m1_i2;
	m.m1_i3 = m_in.m1_i3;

	char mountpoint[500];
	switch (m_in.m2_i1) {
		case 1:
			strcpy(mountpoint, "/sys");
		break;
		case 2:
			strcpy(mountpoint, "/usr");
		break;
		case 3:
			strcpy(mountpoint, "/home");
		break;
		case 4:
			strcpy(mountpoint, "/proc");
		break;
		case 5:
			strcpy(mountpoint, "/");
		break;
	}
	
	struct vmnt *vmp;
	int skipped_first_root = 0;
	for (vmp = &vmnt[0]; vmp < &vmnt[NR_MNTS]; ++vmp) {
		if (strlen(vmp->m_mount_path) > 0) {
			if (strcmp(vmp->m_mount_path, mountpoint) == 0) {
				if (m_in.m2_i1 == 5) {
					if (skipped_first_root == 1) {
						//printf("m_mount_path GOOD = %s,%s\n", mountpoint, vmp->m_mount_path);
						fs_sendrec(vmp->m_fs_e, &m);
						return 0;
					}
					else {
						skipped_first_root = 1;
						//printf("m_mount_path SKIP = %s,%s\n", mountpoint, vmp->m_mount_path);
					}
				}
				else {
					//printf("m_mount_path GOOD = %s,%s\n", mountpoint, vmp->m_mount_path);
					fs_sendrec(vmp->m_fs_e, &m);
					return 0;
				}
			}
			else {
				//printf("m_mount_path BAD = %s,%s\n", mountpoint, vmp->m_mount_path);
			}
		}
	}
	return 0;
}
int do_inodebitmapwalker(){
	message m;
	m.m_type = REQ_INODEBITMAPWALKER;

	/*
	int i = 4;
	fs_sendrec(vmnt[i].m_fs_e, &m);
	return 0;
	*/
	
	char mountpoint[500];
	strcpy(mountpoint, "/usr");
	
	struct vmnt *vmp;
	for (vmp = &vmnt[0]; vmp < &vmnt[NR_MNTS]; ++vmp) {
		if (strlen(vmp->m_mount_path) > 0) {
			if (strcmp(vmp->m_mount_path, mountpoint) == 0) {
				//printf("m_mount_path GOOD = %s,%s\n", mountpoint, vmp->m_mount_path);
				fs_sendrec(vmp->m_fs_e, &m);
				return 0;
			}
			else {
				//printf("m_mount_path BAD = %s,%s\n", mountpoint, vmp->m_mount_path);
			}
		}
	}
	return 0;
}
int do_zonebitmapwalker(){
	message m;
	m.m_type = REQ_ZONEBITMAPWALKER;

	/*
	int i = 4;
	fs_sendrec(vmnt[i].m_fs_e, &m);
	return 0;
	*/
	char mountpoint[500];
	strcpy(mountpoint, "/usr");
	
	struct vmnt *vmp;
	for (vmp = &vmnt[0]; vmp < &vmnt[NR_MNTS]; ++vmp) {
		if (strlen(vmp->m_mount_path) > 0) {
			if (strcmp(vmp->m_mount_path, mountpoint) == 0) {
				//printf("m_mount_path GOOD = %s,%s\n", mountpoint, vmp->m_mount_path);
				fs_sendrec(vmp->m_fs_e, &m);
				return 0;
			}
			else {
				//printf("m_mount_path BAD = %s,%s\n", mountpoint, vmp->m_mount_path);
			}
		}
	}
	return 0;
}

int do_damageinodemap(){
	message m;
	m.m_type = REQ_DAMAGEINODEMAP;
	m.m1_i1 = m_in.m1_i1;
	
	/*
	int i =4;
	fs_sendrec(vmnt[i].m_fs_e, &m);
	return 0;
	*/
	char mountpoint[500];
	strcpy(mountpoint, "/usr");
	
	struct vmnt *vmp;
	for (vmp = &vmnt[0]; vmp < &vmnt[NR_MNTS]; ++vmp) {
		if (strlen(vmp->m_mount_path) > 0) {
			if (strcmp(vmp->m_mount_path, mountpoint) == 0) {
				//printf("m_mount_path GOOD = %s,%s\n", mountpoint, vmp->m_mount_path);
				fs_sendrec(vmp->m_fs_e, &m);
				return 0;
			}
			else {
				//printf("m_mount_path BAD = %s,%s\n", mountpoint, vmp->m_mount_path);
			}
		}
	}
	return 0;
}

int do_fixinodemap(){
	message m;
	m.m_type = REQ_FIXINODEMAP;

	m.m1_i1 = m_in.m1_i1;
	m.m1_i2 = m_in.m1_i2;
	
	/*
	int i =4;
	fs_sendrec(vmnt[i].m_fs_e, &m);
	return m.m1_i1;
	*/
	char mountpoint[500];
	strcpy(mountpoint, "/usr");
	
	struct vmnt *vmp;
	for (vmp = &vmnt[0]; vmp < &vmnt[NR_MNTS]; ++vmp) {
		if (strlen(vmp->m_mount_path) > 0) {
			if (strcmp(vmp->m_mount_path, mountpoint) == 0) {
				//printf("m_mount_path GOOD = %s,%s\n", mountpoint, vmp->m_mount_path);
				fs_sendrec(vmp->m_fs_e, &m);
				return m.m1_i1;
			}
			else {
				//printf("m_mount_path BAD = %s,%s\n", mountpoint, vmp->m_mount_path);
			}
		}
	}
	return 0;
} 

int do_damagezonemap(){
	message m;
	m.m_type = REQ_DAMAGEZONEMAP;
	m.m1_i1 = m_in.m1_i1;

	/*
	int i =4;
	fs_sendrec(vmnt[i].m_fs_e, &m);
	return 0;
	*/
	char mountpoint[500];
	strcpy(mountpoint, "/usr");
	
	struct vmnt *vmp;
	for (vmp = &vmnt[0]; vmp < &vmnt[NR_MNTS]; ++vmp) {
		if (strlen(vmp->m_mount_path) > 0) {
			if (strcmp(vmp->m_mount_path, mountpoint) == 0) {
				//printf("m_mount_path GOOD = %s,%s\n", mountpoint, vmp->m_mount_path);
				fs_sendrec(vmp->m_fs_e, &m);
				return 0;
			}
			else {
				//printf("m_mount_path BAD = %s,%s\n", mountpoint, vmp->m_mount_path);
			}
		}
	}
	return 0;
}

int do_fixzonemap(){
	message m;
	m.m_type = REQ_FIXZONEMAP;
	m.m1_i1 = m_in.m1_i1;
	m.m1_i2 = m_in.m1_i2;

	/*
	int i =4;
	fs_sendrec(vmnt[i].m_fs_e, &m);
	return m.m1_i1;
	//return 0;
	*/
	char mountpoint[500];
	strcpy(mountpoint, "/usr");
	
	struct vmnt *vmp;
	for (vmp = &vmnt[0]; vmp < &vmnt[NR_MNTS]; ++vmp) {
		if (strlen(vmp->m_mount_path) > 0) {
			if (strcmp(vmp->m_mount_path, mountpoint) == 0) {
				//printf("m_mount_path GOOD = %s,%s\n", mountpoint, vmp->m_mount_path);
				fs_sendrec(vmp->m_fs_e, &m);
				return m.m1_i1;
			}
			else {
				//printf("m_mount_path BAD = %s,%s\n", mountpoint, vmp->m_mount_path);
			}
		}
	}
	return 0;
}
