#include <linux/module.h>
#include <linux/kernel.h>

<<<<<<< HEAD
int init_module(void) 
{	
	printk(KERN_DEBUG "Hello Worlds.\n");
	return 0;
}

void cleanup_module(void) 
{
	printk(KERN_DEBUG "Goodbye world.\n");
}
=======
int init_module(void)
{
	pr_debug("Hello Worlds.\n");
	return 0;
}

void cleanup_module(void)
{
	pr_debug("Goodbye world.\n");
}
>>>>>>> 538d67f76d4f3803947180d25236fb0b41989b2a
