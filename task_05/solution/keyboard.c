#include <linux/module.h>	/* Needed by all modules */
#include <linux/kernel.h>	/* Needed for KERN_INFO */
#include <linux/init.h>		/* Needed for macros */
#include <linux/hid.h>
#include <linux/usb.h>

#define DRIVER_AUTHOR "Tran Ly Vu <vutransingapore@gmail.com>"
#define DRIVER_DESC "usb keyboard module"
#define DRIVER_LICENSE "GPL"

static int __init usb_keyboard_init(void)
{
	pr_info("Hello\n");

	return 0;
}


static void __exit usb_keyboard_exit(void)
{
	pr_info("Goodbye\n");
}

static struct usb_device_id usb_keyboard_id_table[] = {
	{ USB_INTERFACE_INFO(
				USB_INTERFACE_CLASS_HID,
				USB_INTERFACE_SUBCLASS_BOOT,
				USB_INTERFACE_PROTOCOL_KEYBOARD) },
	{}
};

MODULE_DEVICE_TABLE(usb, usb_keyboard_id_table);

module_init(usb_keyboard_init);
module_exit(usb_keyboard_exit);

MODULE_LICENSE(DRIVER_LICENSE);
MODULE_AUTHOR(DRIVER_AUTHOR);
MODULE_DESCRIPTION(DRIVER_DESC);
