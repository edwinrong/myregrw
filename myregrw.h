/* -*- C -*-
 * mydevice.h -- definitions for the scullp char module
 *
 * Copyright (C) 2001 Alessandro Rubini and Jonathan Corbet
 * Copyright (C) 2001 O'Reilly & Associates
 *
 * The source code in this file can be freely used, adapted,
 * and redistributed in source or binary form, so long as an
 * acknowledgment appears in derived source files.  The citation
 * should list that the code comes from the book "Linux Device
 * Drivers" by Alessandro Rubini and Jonathan Corbet, published
 * by O'Reilly & Associates.   No warranty is attached;
 * we cannot take responsibility for errors or fitness for use.
 */

#include <linux/ioctl.h>
//#include <linux/cdev.h>


#define  READ_NUM 1
#define	 WRITE_NUM 2

/*
 * Macros to help debugging
 */

#undef PDEBUG             /* undef it, just in case */
#ifdef MYREGRW_DEBUG
#  ifdef __KERNEL__
     /* This one if debugging is on, and kernel space */
#    define PDEBUG(fmt, args...) printk( KERN_DEBUG "myregrw: " fmt, ## args)
#  else
     /* This one for user space */
#    define PDEBUG(fmt, args...) fprintf(stderr, fmt, ## args)
#  endif
#else
#  define PDEBUG(fmt, args...) /* not debugging: nothing */
#endif

#undef PDEBUGG
#define PDEBUGG(fmt, args...) /* nothing: it's a placeholder */


/*
 * Ioctl definitions
 */

/* Use 'K' as magic number */
#define MYREGRW_MAGIC  'K'

#define MYREGRW_CMD0    _IO(MYREGRW_MAGIC, 0)

/*
 * S means "Set" through a ptr,
 * T means "Tell" directly
 * G means "Get" (to a pointed var)
 * Q means "Query", response is on the return value
 * X means "eXchange": G and S atomically
 * H means "sHift": T and Q atomically
 */
#define MYREGRW_CMD1	_IOW(MYREGRW_MAGIC,  1, int)
#define MYREGRW_CMD2	_IO(MYREGRW_MAGIC,   2)
#define MYREGRW_CMD3	_IOR(MYREGRW_MAGIC,  3, int)
#define MYREGRW_WRITE	_IOWR(MYREGRW_MAGIC, 4, int)
#define MYREGRW_READ	_IOWR(MYREGRW_MAGIC, 5, int)


#define MYREGRW_MAXNR 5


