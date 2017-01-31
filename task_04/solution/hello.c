#include <linux/module.h>
#include <linux/kernel.h>

int init_module(void)
{
	pr_debug("Hello Worlds.\n");
	return 0;
}

void cleanup_module(void)
{
	pr_debug("Goodbye world.\n");
}
