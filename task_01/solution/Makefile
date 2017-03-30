ifneq ($(KERNELRELEASE),)
	obj-m += hello.o

else
	KVERSION = $(shell uname -r)
	KDIR ?= /lib/modules/$(KVERSION)/build
	PWD = $(shell pwd) 
all:
	make -C $(KDIR) M=$(PWD) modules

clean:
	make -C $(KDIR) M=$(PWD) clean
endif
