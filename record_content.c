#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <fcntl.h>		/* open */
#include <unistd.h>		/* exit */
#include <stdlib.h>	
#include <string.h>
#include <sys/ioctl.h>		/* ioctl */
#include <errno.h>

#include "myregrw.h"
#include "phy_addr.h"

#define DEVICE_NAME "./regrw"
#define LEN 30

extern int query_lines(const char *filename);
extern int parse_config(const char *filename, struct config_info *p_config_info, int mode);

void __record_contend(const char *filename, unsigned long reg_info[])
{
	FILE *fp;
	int ret=0;
	char buf[LEN]={0};

	fp = fopen(filename, "a+");
	if (fp == NULL) {
		printf("Cannot open configuration file %s\n", filename);
		exit(-1);
	}
	
	sprintf(buf, "%lx\t\t%-lx\n", reg_info[0], reg_info[1]);
		
	ret = fwrite(buf, strlen(buf), 1, fp);
	//printf("ret = %d\n", ret);
	if(ret != 1)
		printf("Error in recording the contents of registers!\n");

	fclose(fp);

}

void record_contend(const char *config_fp, const char *content_fp, int mode)
{
	int fd;
	int num;
	int i, j;
	int ret_val;
	/*
	 * reg_info[0] : register address
	 * reg_info[1] : register data  tobe write to the address 
	 */
	unsigned long reg_info[2] ={0};

	struct config_info *p_config_info, *p_config_info_bat;

	fd = open(DEVICE_NAME, O_RDWR);
	if(fd < 0)
	{
		printf("error:  %s (%d)\n", strerror(errno), errno);
		return;
	}
	
	num = query_lines(config_fp);
	printf("There are %d items in file %s, totally.\n", num, config_fp);


	p_config_info = (struct config_info *)(malloc(num*sizeof(struct config_info)));
	if (p_config_info == NULL) {
		printf("no mem avaliable\n");
		exit(1);
	}
	p_config_info_bat = p_config_info;
	//printf("address of p_config_info = %p, sizeof(struct config_info)=%d\n", p_config_info, sizeof(struct config_info));
	parse_config(config_fp, p_config_info, mode);
	for(i=0; i<num; i++) {
		for(j=0; j<p_config_info->count; j++) {
			if (p_config_info->base_addr%4 == 0) {
				reg_info[0] = p_config_info->base_addr + j*4;
			} else {
				printf("The base address must be aligned with 4\n");
			}
			//printf("addr = %lx\n", reg_info[0]);
			if (0 == mode)
				ret_val = ioctl(fd, MYREGRW_WRITE, reg_info);
			else
				ret_val = ioctl(fd, MYREGRW_READ, reg_info);
			if (ret_val < 0) {
				printf("ioctl put and get msg failed : %d\n", ret_val);
			} else {
				//printf("addr = %lx,\tvalue = %lx\n", reg_info[0], reg_info[1]);
				//record_contend(CONTENT_FILE, reg_info);
				__record_contend(content_fp, reg_info);
			}
		}
		p_config_info++;
		//printf("----------------------------------------------\n");
	}	

	if(p_config_info_bat)
		free(p_config_info_bat);
	
	//	sleep(10);   //used to test multiple open to the device.
	close(fd);


}
