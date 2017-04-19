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

static int __init eudyptula_init(void)
{
        pr_info("eudyptula: Initializing the eudyptula LKM\n");
        majorNumber = register_chrdev(0, DEVICE_NAME, &fops);
        if (majorNumber < 0) {
                printk(KERN_ALERT "eudyptula failed to register a major number\n");
                return majorNumber;
        }
        pr_info("eudyptula: registered correctly with major number %d\n", majorNumber);
        eudyptulaClass = class_create(THIS_MODULE, CLASS_NAME);
        if (IS_ERR(eudyptulaClass)) {
                unregister_chrdev(majorNumber, DEVICE_NAME);
		printk(KERN_ALERT "Failed to register device class\n");
		return PTR_ERR(eudyptulaClass);
        }
        pr_info("eudyptula: device class registered correctly\n");
        eudyptulaDevice = device_create(eudyptulaClass, NULL, MKDEV(majorNumber, 0), NULL, DEVICE_NAME);
        if (IS_ERR(eudyptulaDevice)) {
		class_destroy(eudyptulaClass);
		unregister_chrdev(majorNumber, DEVICE_NAME);
		printk(KERN_ALERT "Failed to create the device\n");
		return PTR_ERR(eudyptulaDevice);
        }
        pr_info("eudyptulaChar: device class created correctly\n");
        return 0;
}
/** @brief The LKM cleanup function
 *  Similar to the initialization function, it is static.
 *  The __exit macro notifies that if this
 *  code is used for a built-in driver (not a LKM)
 *   that this function is not required.
 */
static void __exit eudyptula_exit(void)
{
        device_destroy(eudyptulaClass, MKDEV(majorNumber, 0));
        class_unregister(eudyptulaClass);
        class_destroy(eudyptulaClass);
        unregister_chrdev(majorNumber, DEVICE_NAME);
        pr_info("eudyptula: Goodbye from the LKM!\n");
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
