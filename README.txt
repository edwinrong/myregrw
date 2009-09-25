

Introduction:
~~~~~~~~~~~~~

The utility is used to access hardware registers, now supports read operation. It consists of 
kernel module myregrw.ko and application myregrw. Addresses of registers of interests can be configured in one file 
and with this address file specified as the first argument of application myregrw, and the contents of registers in 
the address file are saved in another file. 

Cross-compile the utility:
~~~~~~~~~~~~~~~~~~~~~~~~~~

In Makefile file, assign the path to the kernel as following,

	KERNELDIR :=  /work/Android_data/kernel-l03/

then execute:
	make ARCH=arm CROSS_COMPILE=arm-eabi-


Usage:
~~~~~~
Copy the myregrw.ko myregrw to xxx
	#/system/bin/busybox cp -rf /sdcard/myregrw xxx

Change the permission of the patition with "RW", where xxx resides in, eg, /data
	#mount -o remount, rw /data
NOTE: if the above operaitons is not done, no permission to do something promotion will be present.

Common operations:
~~~~~~~~~~~~~~~~~
insmod kernel module:
	# insmod myregrw.ko
create device node:
	mknod ./regrw c major 0

Read Registers:
~~~~~~~~~~~~~~~~~
execute applictaion:
	./myregrw config_read.txt content_read.txt read
In the  config_read.txt Annotations can be noted down with "#" at the beginning of every line.
The base address must be aligned with "4".
The format of register addresses in config_read.txt is as following:

#baseaddr count
F0000000 1 

#read registers F0000004 and F0000008
F0000004 2

#read registers F0000000, F0000004, and F0000008
F0000000 3 

#F0000004 4
#F0000008 5 

To see the content:
vi content_read.txt

Write Registers:
~~~~~~~~~~~~~~~~
execute applictaion:
      ./myregrw config_read.txt content_read.txt read
In the  config_read.txt Annotations can be noted down with "#" at the beginning of every line.
The format of register addresses in config_read.txt is as following:

#baseaddr count val(tobe write and read back)
#Write 100 to register f0000000
F0000000 1 100

#Write 5 to register f0000004
#F0000004 5

To see the content of registers after written operations:
vi content_write.txt





