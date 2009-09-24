

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

insmod kernel module:
	# insmod myregrw.ko
create device node:
	mknod ./regrw c major 0
execute applictaion:
	# ./myregrw addr_file.txt content.txt

In the addr_file.txt Annotations can be noted down with "#" at the beginning of every line.
The format of register addresses in addr_file.txt is as following:

#baseaddr count
F0000001 1 

#read registers F0000002 and F0000003
F0000002 2

#read registers F0000003, F0000004, and F0000005
F0000003 3 

#F0000004 4
#F0000005 5 

To see the content:

vi content.txt






