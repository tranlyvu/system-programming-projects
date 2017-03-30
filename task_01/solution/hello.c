#include <linux/module.h>
#include <linux/kernel.h>

#define DRIVER_AUTHOR "Tran Ly Vu <vutransingapore@gmail.com>"
#define DRIVER_DESC "usb keyboard module"
#define DRIVER_LICENSE "GPL"

int init_module(void) 
{	
		printk(KERN_DEBUG "Hello Worlds.\n");
		return 0;
}

void cleanup_module(void) 
{
		printk(KERN_DEBUG "Goodbye world.\n");
}

MODULE_LICENSE(DRIVER_LICENSE);
MODULE_AUTHOR(DRIVER_AUTHOR);
MODULE_DESCRIPTION(DRIVER_DESC);