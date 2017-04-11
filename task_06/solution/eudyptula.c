#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>	

#define DRIVER_AUTHOR "Tran Ly Vu <vutransingapore@gmail.com>"
#define DRIVER_DESC "A simple Linux char driver"
#define DRIVER_LICENSE "GPL"
#define DRIVER_VERSION "0.1" 


static struct file_operations simple_driver_fops =
{
	.owner = DRIVER_AUTHOR,
	.read = device_file_read,
};

static int device_file_major_number = 0;
static const char device_name[] = "char-driver";

static int register_device(void)
{
        int result = 0;
        printk( KERN_NOTICE "eudyptula: register_device() is called." );
        result = register_chrdev( 0, device_name, &simple_driver_fops );
        if( result < 0 )
        {
            printk( KERN_WARNING "eudyptula:  can\'t register character device with errorcode = %i", result );
            return result;
        }
        device_file_major_number = result;
        printk( KERN_NOTICE "eudyptula: registered character device with major number = %i and minor numbers 0...255"
             , device_file_major_number );
        return 0;
}

void unregister_device(void)
{
    printk( KERN_NOTICE "eudyptula: unregister_device() is called" );
    if(device_file_major_number != 0)
    {
        unregister_chrdev(device_file_major_number, device_name);
    }
}

static const char    g_s_Hello_World_string[] = "Hello world from kernel mode!\n\0";
static const ssize_t g_s_Hello_World_size = sizeof(g_s_Hello_World_string);
static ssize_t device_file_read(
                        struct file *file_ptr
                       , char __user *user_buffer
                       , size_t count
                       , loff_t *position)
{
    printk( KERN_NOTICE "Seudyptula: Device file is read at offset = %i, read bytes count = %u"
                , (int)*position
                , (unsigned int)count );
    /* If position is behind the end of a file we have nothing to read */
    if( *position >= g_s_Hello_World_size )
        return 0;
    /* If a user tries to read more than we have, read only as many bytes as we have */
    if( *position + count > g_s_Hello_World_size )
        count = g_s_Hello_World_size - *position;
    if( copy_to_user(user_buffer, g_s_Hello_World_string + *position, count) != 0 )
        return -EFAULT;    
    /* Move reading position */
    *position += count;
    return count;
}

module_init(register_device);
module_exit(unregister_device);

MODULE_LICENSE(DRIVER_LICENSE);
MODULE_AUTHOR(DRIVER_AUTHOR);
MODULE_DESCRIPTION(DRIVER_DESC);
MODULE_VERSION(DRIVER_VERSION);