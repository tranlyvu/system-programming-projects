#include <linux/module.h>
#include <linux/kernel.h>


static int kb_probe(struct usb_interface *interface,
		const struct usb_device_id *id)
{
	pr_alert("USB keyboard plugged in\n");
	return 0;
}

static void kb_disconnect(struct usb_interface *interface)
{
	pr_alert("USB keyboard disconnected.\n");
}


static struct usb_driver kb_driver = {
	.owner		 = THIS_MODULE,			
	.name        = "keyboard",
	.probe       = kb_probe,
	.disconnect  = kb_disconnect,
	.id_table    = kb_table,
};

static struct usb_device_id kb_table [] = {
        { USB_DEVICE(USB_KB_VENDOR_ID, USB_KB_PRODUCT_ID) },
        { }                      /* Terminating entry */
};

MODULE_DEVICE_TABLE (usb, kb_table);

static int __init usb_kb_init(void)
{
	int result;

	/* register this driver with the USB subsystem */
	result = usb_register(&kb_driver);

	if (result) 
		err("usb_register failed. Error number %d", result);

	return result;
}


static void __exit usb_kb_exit(void)
{
        /* deregister this driver with the USB subsystem */
        usb_deregister(&kb_driver);
}

module_exit(usb_kb_exit);
module_init(usb_kb_init);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Tran Ly Vu");
MODULE_DESCRIPTION("keyboard");