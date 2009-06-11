#include <stdio.h>
#include <fcntl.h>		/* open */
#include <unistd.h>		/* exit */
#include<stdlib.h>		
#include <sys/ioctl.h>		/* ioctl */


#include "myregrw.h"

//#define DEVICE_NAME "/dev/chardev"
#define DEVICE_NAME "./regrw"

int main(int argc, char *argv[])
{
	int fd;
//	char msg[100];
//	int count;	
	int i;
	int ret_val;

	/*
	 * reg_info[0] : register address
	 * reg_info[1] : register data  tobe write to the address 
	*/
//	unsigned long reg_info[2] = {0xf0000000, 0xf0000002};     	
	unsigned long reg_info[2] ={0};     	
	
#if 0
	printf("\n\nThese are the %d command-line arguments passed to main:\n", argc);
	printf("---------------------------------------------------------------\n");
	for(i=0; i<argc; i++)
	printf("argv[%d]:%s\n", i, argv[i]);
#endif
	
	if(argc < 2 || argc > 3) {
		printf("error inputing arguments!\n");
		exit(0);
	}

	for(i=1; i<argc; i++) {
		sscanf(argv[i], "%lx", &reg_info[i-1]);
		printf("reg_info[%d] = %lx \n", i-1, reg_info[i-1]);
	}
	
	
	fd = open(DEVICE_NAME, O_RDWR);
	if(fd < 0)
	{
		printf("error\n");
		return -1;
	}

#if 0	
	count = read(fd, msg, 100);
	if(count < 0)
	{
		printf("read error\n");
	}
	else
	{
		for(i=0; i<count; i++)
			printf("%c", msg[i]);
		printf("\n");
	}
#endif

	printf("\nbefore issuing ioctl to exchange data between user and kernel spaces.\n");
	printf("--------------------------------------------------------------------\n");
	for (i=0; i<2; i++)	{
		printf("reg_info[%d] = %lx \n", i, reg_info[i]);
	}


	ret_val = ioctl(fd, MYREGRW_READ, reg_info);
	if (ret_val < 0) {
		printf("ioctl put and get msg failed : %d\n", ret_val);
		exit(-1);
	}

	printf("\nafter issuing ioctl to exchange data between user and kernel spaces.\n");
	printf("--------------------------------------------------------------------\n");
	for (i=0; i<2; i++)	{
		printf("reg_info[%d] = %lx\n", i, reg_info[i]);
	}


//	sleep(10);   //used to test multiple open to the device.
	close(fd);
	return 0;
}

