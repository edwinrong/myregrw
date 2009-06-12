obj-m += myregrw.o

KERNELDIR := /lib/modules/$(shell uname -r)/build
default:
	make -C $(KERNELDIR) M=$(shell pwd) modules
	gcc -Wall tt-myregrw.c parse_conf.c record_content.c

install:
	insmod register_char.ko	
uninstall:
	@rmmod register_char.ko

clean:
	make -C $(KERNELDIR) M=$(shell pwd) clean
	rm -rf *.symvers a.out *.markers *.order
