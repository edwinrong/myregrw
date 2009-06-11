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
	char msg[100];
	int count;	
	int i;
	int ret_val;


	long reg_info[2] = {0xf0000000, 0xf0000002};

	fd = open(DEVICE_NAME, O_RDWR);
	if(fd < 0)
	{
		printf("error\n");
		return -1;
	}
	
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

	printf("before issuing ioctl to exchange data between user and kernel spaces.\n");
	printf("--------------------------------------------------------------------\n");
	for (i=0; i<2; i++)	{
		printf("reg_info[%d] = %lx \n", i, reg_info[i]);
	}


	ret_val = ioctl(fd, SCULLP_IOCXORDER, reg_info);
	if (ret_val < 0) {
		printf("ioctl put and get msg failed : %d\n", ret_val);
		exit(-1);
	}

	printf("after issuing ioctl to exchange data between user and kernel spaces.\n");
	printf("--------------------------------------------------------------------\n");
	for (i=0; i<2; i++)	{
		printf("reg_info[%d] = %lx\n", i, reg_info[i]);
	}


//	sleep(10);
	close(fd);
	return 0;
}

