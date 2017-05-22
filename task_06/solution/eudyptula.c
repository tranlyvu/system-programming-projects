#include <linux/init.h>
#include <linux/module.h>
#include <linux/device.h>
#include <linux/kernel.h>
#include <linux/fs.h>
#include <asm/uaccess.h>

#define CLASS_NAME "eudyptula"
#define DEVICE_NAME "eudyptula"
#define MY_ID "f4173b67ddf8"
#define DRIVER_AUTHOR "Tran Ly Vu <vutransingapore@gmail.com>"
#define DRIVER_DESC "A simple Linux char driver"
#define DRIVER_LICENSE "GPL"
#define DRIVER_VERSION "0.1"


static int    majorNumber;
static char   message[256] = {0};
static short  size_of_message;
static int    numberOpens;
static struct class *eudyptulaClass;
static struct device *eudyptulaDevice;
static ssize_t dev_read(struct file *, char *, size_t, loff_t *);
static ssize_t dev_write(struct file *, const char *, size_t, loff_t *);

static const struct file_operations fops = {
        .read = dev_read,
        .write = dev_write,
};

static struct miscdevice hello_dev = {
        MISC_DYNAMIC_MINOR,
        "eudyptula",
        &hello_fops
};

static int __init eudyptula_init(void)
{
        int ret;

        ret = misc_register(&hello_dev);

        return ret;
}

static void __exit eudyptula_exit(void)
{
        misc_deregister(&hello_dev);
}

static ssize_t dev_read(struct file *filep, char *buffer, size_t len, loff_t *offset)
{
	int error_count = copy_to_user(buffer, message, size_of_message);
	if (error_count == 0) {
		pr_info(MY_ID);
		return (size_of_message = 0);
	} else {
		return -EINVAL;
	}
}

static ssize_t dev_write(struct file *filep, const char *buffer, size_t len, loff_t *offset)
{
	size_of_message = strlen(MY_ID);
        if (!strcmp(message, MY_ID)) {
return len;             
        } else {
                return -EINVAL;
        }	
}

module_init(eudyptula_init);
module_exit(eudyptula_exit);
MODULE_LICENSE(DRIVER_LICENSE);
MODULE_AUTHOR(DRIVER_AUTHOR);
MODULE_DESCRIPTION(DRIVER_DESC);
MODULE_VERSION(DRIVER_VERSION);
