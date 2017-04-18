#include <linux/init.h>
#include <linux/module.h>
#include <linux/device.h>
#include <linux/kernel.h>
#include <linux/fs.h>
#include <asm/uaccess.h>

#define CLASS_NAME "eudyptula"
#define DEVICE_NAME "eudyptula"

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
/** @brief This function is called whenever device is
 *  being read from user space i.e. data is
 *  being sent from the device to the user.
 *  In this case is uses the copy_to_user() function to
 *  send the buffer string to the user and captures any errors.
 *  @param filep A pointer to a file object (defined in linux/fs.h)
 *  @param buffer The pointer to the buffer to which this function writes the data
 *  @param len The length of the b
 *  @param offset The offset if required
 */
static ssize_t dev_read(struct file *filep, char *buffer, size_t len, loff_t *offset)
{
    int error_count = 0;
    error_count = copy_to_user(buffer, message, size_of_message);
    if (error_count == 0) {
        pr_info("eudyptula: Sent %d characters to the user\n", size_of_message);
        return (size_of_message = 0);
    } else {
        pr_info("eudyptula: Failed to send %d characters to the user\n", error_count);
        return -EFAULT;
    }
}
/** @brief This function is called whenever
 *  the device is being written to from user space i.e.
 *  data is sent to the device from the user.
 *  The data is copied to the message[] array in this
 *  LKM using the sprintf() function along with the length of the string.
 *  @param filep A pointer to a file object
 *  @param buffer The buffer to that contains the string to write to the device
 *  @param len The length of the array of data that is being
 *  passed in the const char buffer
 *  @param offset The offset if required
 */
static ssize_t dev_write(struct file *filep, const char *buffer, size_t len, loff_t *offset)
{
    printf(message, "%s(%zu letters)", buffer, len);
    size_of_message = strlen(message);
    pr_info("eudyptula: Received %zu characters from the user\n", len);
    return len;
}

module_init(eudyptula_init);
module_exit(eudyptula_exit);
MODULE_LICENSE(DRIVER_LICENSE);
MODULE_AUTHOR(DRIVER_AUTHOR);
MODULE_DESCRIPTION(DRIVER_DESC);
MODULE_VERSION(DRIVER_VERSION);
