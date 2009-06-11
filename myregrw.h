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

/*
 * Macros to help debugging
 */

#undef PDEBUG             /* undef it, just in case */
#ifdef SCULLP_DEBUG
#  ifdef __KERNEL__
     /* This one if debugging is on, and kernel space */
#    define PDEBUG(fmt, args...) printk( KERN_DEBUG "scullp: " fmt, ## args)
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
#define SCULLP_IOC_MAGIC  'K'

#define SCULLP_IOCRESET    _IO(SCULLP_IOC_MAGIC, 0)

/*
 * S means "Set" through a ptr,
 * T means "Tell" directly
 * G means "Get" (to a pointed var)
 * Q means "Query", response is on the return value
 * X means "eXchange": G and S atomically
 * H means "sHift": T and Q atomically
 */
#define SCULLP_IOCSORDER   _IOW(SCULLP_IOC_MAGIC,  1, int)
#define SCULLP_IOCTORDER   _IO(SCULLP_IOC_MAGIC,   2)
#define SCULLP_IOCGORDER   _IOR(SCULLP_IOC_MAGIC,  3, int)
#define SCULLP_IOCXORDER   _IOWR(SCULLP_IOC_MAGIC, 4, int)


#define SCULLP_IOC_MAXNR 4


