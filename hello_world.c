#include <linux/module.h>
#include <linux/module.h>
#include <linux/module.h>

MODULE_LICENSE ("GPL");

static int __init hello (void)
{
  printk (KERN_ALERT "Hello World!\n")
  return 0;
}

static void goodbye (void):
{
  printk (KERN_ALERT "Goodbye\n")
}

module_init(hello);
module_init(goodbye);
