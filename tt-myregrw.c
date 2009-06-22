#include <stdio.h>
#include <fcntl.h>		/* open */
#include <unistd.h>		/* exit */
#include<stdlib.h>		
#include <sys/ioctl.h>		/* ioctl */

#include "myregrw.h"
#include "phy_addr.h"

#define DEVICE_NAME "./regrw"
#if 0
#define ADDR_FILE "./addr_conf.txt"
#define CONTENT_FILE "./content.txt"
#endif


extern int query_lines(const char *filename);
extern int parse_config(const char *filename, struct addr_info *p_addr_info);
extern void record_contend(const char *filename, unsigned long reg_info[]);



int main(int argc, char *argv[])
{
	int fd;
	int i, j;
	int ret_val;
	
	int num;

	char *content_fp, *addr_fp;
	
	struct addr_info *p_addr_info, *p_addr_info_bat;

	/*
	 * reg_info[0] : register address
	 * reg_info[1] : register data  tobe write to the address 
	*/
	unsigned long reg_info[2] ={0};     	

	if(argc != 3) {
		printf("%s : error in arguments format: \n\t%s addr_file content_file\n", argv[0], argv[0]);
		exit(1);
	} else {
		addr_fp = argv[1];
		content_fp = argv[2];
	}
   	
	fd = open(DEVICE_NAME, O_RDWR);
	if(fd < 0)
	{
		printf("error\n");
		return -1;
	}
	
	//num = query_lines(ADDR_FILE);
	num = query_lines(addr_fp);
	//printf("There are %d items in file %s, totally.\n", num, ADDR_FILE);
	printf("There are %d items in file %s, totally.\n", num, addr_fp);


	p_addr_info = (struct addr_info *)(malloc(2*num*sizeof(struct addr_info *)));
	p_addr_info_bat = p_addr_info;

	//parse_config(ADDR_FILE, p_addr_info);
	parse_config(addr_fp, p_addr_info);

	for(i=0; i<num; i++) {
		for(j=0; j<p_addr_info->count; j++) {
			reg_info[0] = p_addr_info->base_addr + j;
			//printf("addr = %lx\n", reg_info[0]);
			ret_val = ioctl(fd, MYREGRW_READ, reg_info);
			if (ret_val < 0) {
				printf("ioctl put and get msg failed : %d\n", ret_val);
				//exit(-1);
			} else {
				//printf("addr = %lx,\tvalue = %lx\n", reg_info[0], reg_info[1]);
				//record_contend(CONTENT_FILE, reg_info);
				record_contend(content_fp, reg_info);
			}
		}
		p_addr_info++;
		printf("----------------------------------------------\n");
	}	

	if(p_addr_info_bat)
		free(p_addr_info_bat);
	
	//	sleep(10);   //used to test multiple open to the device.
	close(fd);
	return 0;


}


