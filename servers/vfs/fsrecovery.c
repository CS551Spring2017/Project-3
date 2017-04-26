#include "fs.h"
#include <stdio.h>
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

	int i = 4;
	fs_sendrec(vmnt[i].m_fs_e, &m);
	return 0;
}
int do_inodebitmapwalker(){
	message m;
	m.m_type = REQ_INODEBITMAPWALKER;

	int i = 4;
	fs_sendrec(vmnt[i].m_fs_e, &m);
	return 0;
}
int do_zonebitmapwalker(){
	message m;
	m.m_type = REQ_ZONEBITMAPWALKER;

	int i = 4;
	fs_sendrec(vmnt[i].m_fs_e, &m);
	return 0;
}

int do_damageinodemap(){
	message m;
	m.m_type = REQ_DAMAGEINODEMAP;

	int i =4;
	fs_sendrec(vmnt[i].m_fs_e, &m);
	return 0;
}

int do_fixinodemap(){
	message m;
	m.m_type = REQ_FIXINODEMAP;

	int i =4;
	fs_sendrec(vmnt[i].m_fs_e, &m);
	return 0;
}

int do_damagezonemap(){
	message m;
	m.m_type = REQ_DAMAGEZONEMAP;

	int i =4;
	fs_sendrec(vmnt[i].m_fs_e, &m);
	return 0;
}

int do_fixzonemap(){
	message m;
	m.m_type = REQ_FIXZONEMAP;

	int i =4;
	fs_sendrec(vmnt[i].m_fs_e, &m);
	return 0;
}
