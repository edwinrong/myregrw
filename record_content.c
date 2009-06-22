#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

#define LEN 30

void record_contend(const char *filename, unsigned long reg_info[])
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


