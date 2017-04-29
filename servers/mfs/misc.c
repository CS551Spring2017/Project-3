#include "fs.h"
#include <stdio.h>
#include "buf.h"
#include "super.h"
#include <assert.h>
#include <minix/vfsif.h>
#include <minix/bdev.h>
#include "inode.h"
#include "clean.h"

/*===========================================================================*
 *				fs_sync					     *
 *===========================================================================*/
int fs_sync()
{
/* Perform the sync() system call.  Flush all the tables. 
 * The order in which the various tables are flushed is critical.  The
 * blocks must be flushed last, since rw_inode() leaves its results in
 * the block cache.
 */
  struct inode *rip;

  assert(lmfs_nr_bufs() > 0);

  /* Write all the dirty inodes to the disk. */
  for(rip = &inode[0]; rip < &inode[NR_INODES]; rip++)
	  if(rip->i_count > 0 && IN_ISDIRTY(rip)) rw_inode(rip, WRITING);

  /* Write all the dirty blocks to the disk. */
  lmfs_flushall();

  return(OK);		/* sync() can't fail */
}


/*===========================================================================*
 *				fs_flush				     *
 *===========================================================================*/
int fs_flush()
{
/* Flush the blocks of a device from the cache after writing any dirty blocks
 * to disk.
 */
  dev_t dev = (dev_t) fs_m_in.REQ_DEV;
  if(dev == fs_dev) return(EBUSY);
 
  lmfs_flushall();
  lmfs_invalidate(dev);

  return(OK);
}


/*===========================================================================*
 *				fs_new_driver				     *
 *===========================================================================*/
int fs_new_driver(void)
{
/* Set a new driver endpoint for this device. */
  dev_t dev;
  cp_grant_id_t label_gid;
  size_t label_len;
  char label[sizeof(fs_dev_label)];
  int r;

  dev = (dev_t) fs_m_in.REQ_DEV;
  label_gid = (cp_grant_id_t) fs_m_in.REQ_GRANT;
  label_len = (size_t) fs_m_in.REQ_PATH_LEN;

  if (label_len > sizeof(label))
	return(EINVAL);

  r = sys_safecopyfrom(fs_m_in.m_source, label_gid, (vir_bytes) 0,
	(vir_bytes) label, label_len);

  if (r != OK) {
	printf("MFS: fs_new_driver safecopyfrom failed (%d)\n", r);
	return(EINVAL);
  }

  bdev_driver(dev, label);

  return(OK);
}


/*===========================================================================*
 *				fs_directorywalker				     *
 *===========================================================================*/
int fs_directorywalker(void) {
	struct buf *bp;
	
	int depth = fs_m_in.m1_i1;
	int vnode = fs_m_in.m1_i2;
	struct inode *n = get_inode(fs_m_in.m1_i3, vnode);
	

	for (int di = 0; di < depth; di++) printf("  ");
	printf("Size: %d\n", n->i_size);
	int i = 0;
	
	for (int di = 0; di < depth; di++) printf("  ");
	printf("Direct Zones: ");
	for (i = 0; i < 7; i++){
		if(n->i_zone[i] != NO_ZONE)
			printf("%d, ", n->i_zone[i]);
	}
	printf("\n");

	if (n->i_zone[7] != NO_ZONE){
		for (int di = 0; di < depth; di++) printf("  ");
		printf("Single Indirect Zones: ");
		struct buf *bp = get_block(n->i_dev, (block_t)(n->i_zone[7] << n->i_sp->s_log_zone_size), NORMAL);
		for (i =0; i<n->i_nindirs; i++){
			int a = rd_indir(bp,i);
			if (a)
				printf("%d, ", a);
		}
		put_block(bp, INDIRECT_BLOCK);
		printf("\n");
	}

	if(n->i_zone[8] != NO_ZONE){
		for (int di = 0; di < depth; di++) printf("  ");
		printf("Double Indirect Zones: ");
		struct buf *bp = get_block(n->i_dev,(block_t)(n->i_zone[8] << n->i_sp->s_log_zone_size), NORMAL);
		for(i=0;i<n->i_nindirs;i++){
			block_t a = rd_indir(bp, i);
			if (a != NO_ZONE){
				struct buf *bp2 = get_block(n->i_dev,(block_t)(a<<n->i_sp->s_log_zone_size), NORMAL);
				int j;
				for (j = 0;j<n->i_nindirs;j++){
					int b = rd_indir(bp2, j);
					if(b)
						printf("%d, ", b);
				}
				put_block(bp2, INDIRECT_BLOCK);
			}
		}
		put_block(bp, INDIRECT_BLOCK);
		printf("\n");
	}

	put_inode(n);
	
	printf("\n");
	return 0;
}


/*===========================================================================*
 *				fs_inodebitmapwalker				     *
 *===========================================================================*/
int fs_inodebitmapwalker(void) {
	struct super_block * sp = get_super(fs_dev);
	block_t start_block = START_BLOCK;
	unsigned block = 0;
	unsigned word = 0;
	unsigned bit = 0;
	bitchunk_t k, mask;
	struct buf *bp;
	
	int i = 0;
	int limit = FS_BITS_PER_BLOCK(sp->s_block_size) * sp->s_imap_blocks;
	for(i = 0; i < limit; i++){
		block = i / FS_BITS_PER_BLOCK(sp->s_block_size);
		word = (i % FS_BITS_PER_BLOCK(sp->s_block_size)) / FS_BITCHUNK_BITS;
		bit = i % FS_BITCHUNK_BITS;

		mask = 1 << bit;

		bp = get_block(sp->s_dev, start_block + block, NORMAL);
		k = (bitchunk_t) conv4(sp->s_native, (int) b_bitmap(bp)[word]);
		if(k&mask){
			printf("%d, ",i);
		}
		put_block(bp, MAP_BLOCK);
	}
	printf("\n");
	printf("%d\n",limit);
	printf("\n");
	return 0;
}


/*===========================================================================*
 *				fs_zonebitmapwalker				     *
 *===========================================================================*/
int fs_zonebitmapwalker(void) {
	struct super_block * sp = get_super(fs_dev);
	block_t start_block = START_BLOCK;
	unsigned block = 0;
	unsigned word = 0;
	unsigned bit = 0;
	bitchunk_t k, mask;
	struct buf *bp;
	
	start_block = START_BLOCK + sp->s_imap_blocks;
	int i = 0;
	int limit = FS_BITS_PER_BLOCK(sp->s_block_size) * sp->s_zmap_blocks;

	for(i = 0; i < limit; i++){
		block = i / FS_BITS_PER_BLOCK(sp->s_block_size);
		word = (i % FS_BITS_PER_BLOCK(sp->s_block_size)) / FS_BITCHUNK_BITS;
		bit = i % FS_BITCHUNK_BITS;
		mask = 1 << bit;
		bp = get_block(sp->s_dev, start_block + block, NORMAL);
		k = (bitchunk_t) conv4(sp->s_native, (int)b_bitmap(bp)[word]);
		if(k&mask){
			printf("%d, ", i);
		}
		put_block(bp, MAP_BLOCK);

	}
	printf("\n");
	return 0;
}

/*===========================================================================*
 *				fs_damageinodemap				     *
 *===========================================================================*/


int fs_damageinodemap(void){
	struct super_block * sp = get_super(fs_dev);
	block_t start_block = START_BLOCK;
	unsigned block = 0;
	unsigned word = 0;
	unsigned bit = 0;
	bitchunk_t k, mask;
	struct buf *bp;
	

	int i = fs_m_in.m1_i1;
	// inode
	int limit = FS_BITS_PER_BLOCK(sp->s_block_size) * sp->s_imap_blocks;
	if(i>limit || i<0){
		printf("Error: Invalid Inode Number\n");
		return 0;
	}

	block = i / FS_BITS_PER_BLOCK(sp->s_block_size);
	word = (i % FS_BITS_PER_BLOCK(sp->s_block_size)) / FS_BITCHUNK_BITS;
	bit = i % FS_BITCHUNK_BITS;
	printf("block = %d, word = %d, bit = %d\n",block,word,bit);
	mask = 1 << bit;

	bp = get_block(sp->s_dev, start_block + block, NORMAL);
	k = (bitchunk_t) conv4(sp->s_native, (int) b_bitmap(bp)[word]);
	
	// break it
	if(k&mask){
		k &= ~mask;
		b_bitmap(bp)[word] = (bitchunk_t) conv4(sp->s_native, (int) k);
		MARKDIRTY(bp);
		printf("Flipped inode bit %d from 1 to 0.\n", i);
	}
	else
	{
		k |= mask;
		b_bitmap(bp)[word] = (bitchunk_t) conv4(sp->s_native, (int) k);
		MARKDIRTY(bp);
		printf("Flipped inode bit %d from 0 to 1.\n", i);
	}
	put_block(bp, MAP_BLOCK);
	
	printf("DAMAGE INODE MAP\n");
	return 0;
}


/*====================================================	======================*
 *				fs_fixinodemap				     *
 *===========================================================================*/

int fs_fixinodemap(void){
	int i = fs_m_in.m1_i1;
	int valid = fs_m_in.m1_i2;
	
	struct super_block * sp = get_super(fs_dev);
	if(i == -1 && valid == -1)
	{
		printf("In fixinodemap -1 -1\n");
		//fs_out.m1_i3 = FS_BITS_PER_BLOCK(sp->s_block_size) * sp->s_imap_blocks;
		int maxinodes =  FS_BITS_PER_BLOCK(sp->s_block_size) * sp->s_imap_blocks;
		printf("maxinodes: %d", maxinodes);
		fs_m_out.m1_i1 = maxinodes;
		return maxinodes;
	}

	block_t start_block = START_BLOCK;
	unsigned block = 0;
	unsigned word = 0;
	unsigned bit = 0;
	bitchunk_t k, mask;
	struct buf *bp;
	
	block = i / FS_BITS_PER_BLOCK(sp->s_block_size);
	word = (i % FS_BITS_PER_BLOCK(sp->s_block_size)) / FS_BITCHUNK_BITS;
	bit = i % FS_BITCHUNK_BITS;
	
	mask = 1 << bit;

	bp = get_block(sp->s_dev, start_block + block, NORMAL);
	k = (bitchunk_t) conv4(sp->s_native, (int) b_bitmap(bp)[word]);
	if(k&mask && valid)
	{
		// ok (occupied)
		put_block(bp, MAP_BLOCK);
		fs_m_out.m1_i1 = 0;
		return 0;
	}
	else if(k&mask && !valid)
	{
		// exists in inode bitmap, but not in directory walker
		k|=mask;
		b_bitmap(bp)[word] = (bitchunk_t) conv4(sp->s_native, (int) k);
		MARKDIRTY(bp);
		printf("Inode bitmap is 1, but inode is free, Changing bit %d to 0...\n", i);
		fs_m_out.m1_i1 = 1;	
		put_block(bp, MAP_BLOCK);
		return 1;
	}
	else if(!(k&mask) && valid)
	{
		// exists in directory walker, but not in inode bitmap
		k |= mask;
		b_bitmap(bp)[word] = (bitchunk_t) conv4(sp->s_native, (int) k);
		MARKDIRTY(bp);
		printf("Inode bitmap is 0, but inode is allocated, Changing bit %d to 1...\n", i);
		fs_m_out.m1_i1 = 1;	
		put_block(bp, MAP_BLOCK);
		return 1;
	}
	else if(!(k&mask) && !valid)
	{
		// ok (free)
	}
	put_block(bp, MAP_BLOCK);
	fs_m_out.m1_i1 = 0;
	return 0;
}

/*===========================================================================*
 *				fs_damagezonemap				     *
 *===========================================================================*/

int fs_damagezonemap(void){
	struct super_block * sp = get_super(fs_dev);
	block_t start_block = START_BLOCK;
	unsigned block = 0;
	unsigned word = 0;
	unsigned bit = 0;
	bitchunk_t k, mask;
	struct buf *bp;
	
	start_block = START_BLOCK + sp->s_imap_blocks;
	
	int i = fs_m_in.m1_i1;
	// zones
	int limit = FS_BITS_PER_BLOCK(sp->s_block_size) * sp->s_zmap_blocks;	
	if(i>limit || i<0){
		printf("Error: Invalid Zone Number\n");
		return 0;
	}


	block = i / FS_BITS_PER_BLOCK(sp->s_block_size);
	word = (i % FS_BITS_PER_BLOCK(sp->s_block_size)) / FS_BITCHUNK_BITS;
	bit = i % FS_BITCHUNK_BITS;
	printf("block = %d, word = %d, bit = %d\n",block,word,bit);
	mask = 1 << bit;

	bp = get_block(sp->s_dev, start_block + block, NORMAL);
	k = (bitchunk_t) conv4(sp->s_native, (int) b_bitmap(bp)[word]);
	
	// break it
	if(k&mask){
		k &= ~mask;
		b_bitmap(bp)[word] = (bitchunk_t) conv4(sp->s_native, (int) k);
		MARKDIRTY(bp);
		printf("Flipped zone bit %d from 1 to 0.\n", i);
	}
	else
	{
		k |= mask;
		b_bitmap(bp)[word] = (bitchunk_t) conv4(sp->s_native, (int) k);
		MARKDIRTY(bp);
		printf("Flipped zone bit %d from 0 to 01.\n", i);
	}
	
	put_block(bp, MAP_BLOCK);
	
	printf("DAMAGE ZONE MAP\n");
	return 0;
}

/*===========================================================================*
 *				fs_fixzonemap				     *
 *===========================================================================*/

int fs_fixzonemap(void){
	int numErrors = 0;
 	struct buf *bp;
	
	int vnode = fs_m_in.m1_i1;
	struct inode *n = get_inode(fs_m_in.m1_i2, vnode);
	
	for (int i = 0; i < 7; i++){
		if(n->i_zone[i] != NO_ZONE)
			numErrors += zonebitfix(n->i_zone[i]);
	}

	if (n->i_zone[7] != NO_ZONE){
		struct buf *bp = get_block(n->i_dev, (block_t)(n->i_zone[7] << n->i_sp->s_log_zone_size), NORMAL);
		for (int i =0; i<n->i_nindirs; i++){
			int a = rd_indir(bp,i);
			if (a)
				numErrors += zonebitfix(a);
		}
		put_block(bp, INDIRECT_BLOCK);
	}

	if(n->i_zone[8] != NO_ZONE){
		struct buf *bp = get_block(n->i_dev,(block_t)(n->i_zone[8] << n->i_sp->s_log_zone_size), NORMAL);
		for(int i=0;i<n->i_nindirs;i++){
			block_t a = rd_indir(bp, i);
			if (a != NO_ZONE){
				struct buf *bp2 = get_block(n->i_dev,(block_t)(a<<n->i_sp->s_log_zone_size), NORMAL);
				for (int j = 0;j<n->i_nindirs;j++){
					int b = rd_indir(bp2, j);
					if(b)
						numErrors += zonebitfix(b);
				}
				put_block(bp2, INDIRECT_BLOCK);
			}
		}
		put_block(bp, INDIRECT_BLOCK);
	}

	put_inode(n);
	fs_m_out.m1_i1 = numErrors;	
	return 0;
 }

int zonebitfix(int zone)
 {
	int i = zone;
	struct super_block * sp = get_super(fs_dev);
	block_t start_block = START_BLOCK;
	start_block = START_BLOCK + sp->s_imap_blocks;
	unsigned block = 0;
	unsigned word = 0;
	unsigned bit = 0;
	bitchunk_t k, mask;
	struct buf *bp;
	
	block = i / FS_BITS_PER_BLOCK(sp->s_block_size);
	word = (i % FS_BITS_PER_BLOCK(sp->s_block_size)) / FS_BITCHUNK_BITS;
	bit = i % FS_BITCHUNK_BITS;
	mask = 1 << bit;
	bp = get_block(sp->s_dev, start_block + block, NORMAL);
	k = (bitchunk_t) conv4(sp->s_native, (int)b_bitmap(bp)[word]);

	if(k&mask)
	{
		// ok (occupied)
		put_block(bp, MAP_BLOCK);
		return 0;
	}
	else
	{
		// exists in directory walker, but not in zone bitmap
		k |= mask;
		b_bitmap(bp)[word] = (bitchunk_t) conv4(sp->s_native, (int) k);
		MARKDIRTY(bp);
		printf("Zone bitmap is 0, but zone is allocated, Changing bit %d to 1...\n", i);
		put_block(bp, MAP_BLOCK);
		return 1;
	}
	put_block(bp, MAP_BLOCK);
	return 0;
 }