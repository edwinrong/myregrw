#ifndef __PHY_ADDR_H__
#define __PHY_ADDR_H__

struct config_info
{	
	unsigned long base_addr;
	unsigned int count;
	unsigned long val;  /* This member will not be used in reading operation, 
						while plays 2 roles here in write operation, before writing, it sustains 
						the config value tobe written to the specific register, after writing, 
						current content of the register will be read out and stored back to 
						this member. */
};




#endif


