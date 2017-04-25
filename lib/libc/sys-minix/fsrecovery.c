#include <sys/cdefs.h>
#include <lib.h>
#include "namespace.h"

#ifdef __weak_alias
__weak_alias(directorywalker, _directorywalker)
__weak_alias(inodebitmapwalker, _inodebitmapwalker)
__weak_alias(zonebitmapwalker, _zonebitmapwalker)
#endif

#include <unistd.h>

int directorywalker(const char *name)
{
  message m;

  return(_syscall(VFS_PROC_NR, REQ_DIRECTORYWALKER, &m));
}

int inodebitmapwalker()
{
  message m;

  return(_syscall(VFS_PROC_NR, REQ_INODEBITMAPWALKER, &m));
}

int zonebitmapwalker()
{
  message m;

  return(_syscall(VFS_PROC_NR, REQ_ZONEBITMAPWALKER, &m));
}