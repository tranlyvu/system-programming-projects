#include <linux/init.h>
#include <linux/fs.h>
#include <linux/miscdevice.h>
#include <linux/uaccess.h>
#include <linux/module.h>

#define DEVICE_NAME "eudyptula"
#define MY_ID "f4173b67ddf8"
#define MY_ID_LENG 13 /*include the final NULL*/
#define DRIVER_AUTHOR "Tran Ly Vu <vutransingapore@gmail.com>"
#define DRIVER_DESC "A simple msc char device driver written by Tran Ly Vu"

static struct miscdevice my_dev;

static ssize_t device_read(struct file *filep,
			char __user *buffer,
			size_t len,
			loff_t *offset)
{
	return simple_read_from_buffer(buffer, len, offset, MY_ID, MY_ID_LENG);
}

static ssize_t device_write(struct file *filep,
			const char __user *buffer,
			size_t len,
			loff_t *offset)
{
	char *my_str = MY_ID;

	char input[MY_ID_LENG];

	int error_count = copy_from_user(input, buffer, MY_ID_LENG);

	if ((len != MY_ID_LENG) || error_count != 0 || (!strcmp(my_str, input)))
		return -EINVAL;
	else
		return len;
}

static const struct file_operations fops = {
	.read = device_read,
	.write = device_write
};

static int __init eudyptula_init(void)
{
	int retval;

	my_dev.minor = MISC_DYNAMIC_MINOR;

	my_dev.name = DEVICE_NAME;
	my_dev.fops = &fops;
	retval = misc_register(&my_dev);
	if (retval)
		return retval;
	return 0;
}

static void __exit eudyptula_exit(void)
{
	misc_deregister(&my_dev);
}

module_init(eudyptula_init);
module_exit(eudyptula_exit);
MODULE_AUTHOR(DRIVER_AUTHOR);
MODULE_DESCRIPTION(DRIVER_DESC);

