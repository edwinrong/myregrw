#include <stdio.h>
#include <string.h>		/* strncmp */
#include <stdlib.h>	

#include "phy_addr.h"

extern int query_lines(const char *filename);
extern int parse_config(const char *filename, struct config_info *p_config_info);
extern void record_contend(const char *config_fp, const char *content_fp, int mode);


int main(int argc, char *argv[])
{
	int mode; /* write : mode = 0, read : mode = 1; */
	char *config_fp, *content_fp;

	if(argc != 4) {
		printf("%s : error in arguments format: \n\t%s config_file content_file mode[read, write]\n", argv[0], argv[0]);
		exit(1);
	} else if ((strncmp(argv[3], "write", 5)) && (strncmp(argv[3], "read", 4)) ) {
		printf("plese check the 3rd option\n");
		exit(1);
	} else {
		config_fp = argv[1];
		content_fp = argv[2];
		mode = (strncmp(argv[3], "write", 5) != 0);
	}
	//printf("mode = %d\n", mode);
	record_contend(config_fp, content_fp, mode);
	
	return 0;


}


