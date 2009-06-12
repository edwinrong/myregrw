#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

#include "phy_addr.h"

static void strchrnull(char *str, int ch)
{
	int i = 0;
	while (str[i]) {
		if (str[i] == ch) {
			str[i] = '\0';
			break;
		}
		i++;
	}
}


int query_lines(const char *filename)
{
	FILE *fp;
	char *line;
	ssize_t n;
	size_t len = 0;
	
	int num = 0;

	fp = fopen(filename, "r");
	if (fp == NULL) {
		printf("Cannot open configuration file %s\n", filename);
		exit(-1);
	}

	while ((n = getline(&line, &len, fp)) != -1) {
		if (line[n - 1] == '\n')
			line[n - 1] = '\0';

		/* Because the file format does not know any form of quoting we
		   can search forward for the next ';' character and if found
		   make it terminating the line.  */
		strchrnull (line, '#');

		/* If the line is blank it is ignored.  */
		if (line[0] == '\0' || line[0] == '\r'){
			//printf("num = %d\n", num);
			continue;
		}

		num++;
		
		
		#if 0
		//printf("\\n = %d\n", '\n');
		//printf("\\r = %d\n", '\r');
	
		//printf("line[0]=%d/%c\n", line[0],line[0]);
		//printf("num[%d] n[%d]: %s\n", num, n, line);
		#endif
	}

	if (line)
		free (line);
	fclose (fp);

	return num;
}


#if 1
int parse_config(const char *filename, struct addr_info *p_addr_info)
{
	FILE *fp;
	char *line;
	ssize_t n;
	size_t len = 0;
	char *base_addr, *count;

	fp = fopen(filename, "r");
	if (fp == NULL) {
		printf("Cannot open configuration file %s\n", filename);
		exit(-1);
	}

	while ((n = getline(&line, &len, fp)) != -1) {
		if (line[n - 1] == '\n')
			line[n - 1] = '\0';

		/* Because the file format does not know any form of quoting we
		   can search forward for the next ';' character and if found
		   make it terminating the line.  */
		strchrnull (line, '#');

		/* If the line is blank it is ignored.  */
		if (line[0] == '\0' || line[0] == '\r')
			continue;

		base_addr = strtok(line, " ");
		count = strtok(NULL, " ");
#if 0
		printf("base_addr=%s\n", base_addr);
		printf("count=%s\n", count);
#endif
		p_addr_info->base_addr = strtoul(base_addr, NULL, 16);
		p_addr_info->count = strtoul(count, NULL, 10);
		//printf("base_addr = %lx, count=%d\n", p_addr_info->base_addr, p_addr_info->count);
		
		p_addr_info++;

	}

	if (line)
		free (line);
	fclose (fp);

	return 0;
}

#endif
