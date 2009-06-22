#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/init.h>
#include <linux/fs.h>
#include <asm/uaccess.h>
#include <linux/cdev.h>
#include <asm/io.h>

//#ifdef  EVB_IM9815
#include <mach/hardware.h>
//#endif

#include "myregrw.h"

#define REGISTER_1
//#define REGISTER_2
static int __init myregrw_init(void);
static void __exit myregrw_exit(void);

static int myregrw_open(struct inode *, struct file *);
static int myregrw_release(struct inode *, struct file *);
static ssize_t myregrw_read(struct file *, char *, size_t, loff_t *);
static ssize_t myregrw_write(struct file *, const char *, size_t, loff_t*);
static int myregrw_ioctl(struct inode *inode, struct file *filp, unsigned int cmd, unsigned long arg);


#define SUCCESS 0
#define DEVICE_NAME "regrw"
#define BUF_LEN 80
static int major;
static int myregrw_opened = 0;
static char msg[BUF_LEN];
static char *msg_ptr;

static unsigned long reg_info[2];			/* reg_info[0] : register address
											   reg_info[1] : register data  tobe write to the address */

#ifdef REGISTER_2
static dev_t devid;
static struct cdev *my_cdev;
#endif
static struct file_operations fops = {
	.open		= myregrw_open,
	.release	= myregrw_release,
	.read		= myregrw_read,
	.write		= myregrw_write,
	.ioctl		= myregrw_ioctl,
};

static int __init myregrw_init(void)
{
#ifdef REGISTER_1
	major = register_chrdev(0, DEVICE_NAME, &fops);
	if(major < 0){
		printk(KERN_ALERT "Register char device myregrw failed with %d\n", major);
		return major;
	}
	printk(KERN_INFO "I was assigned major number %d.\n", major);
	return SUCCESS;
#endif

#ifdef REGISTER_2
	int err;
	if(alloc_chrdev_region(&devid, 0, 1, DEVICE_NAME)){
		return -ENODEV;
	}
	major = MAJOR(devid);
	my_cdev = cdev_alloc();
	if(!my_cdev){
		return -ENOMEM;
	}
	cdev_init(my_cdev, &fops);
	err = cdev_add(my_cdev, devid, 1);
	if(err){
		printk(KERN_INFO "I was assigned major number %d.\n", major);
		return -1;
	}
#endif
	return SUCCESS;

} 

static void __exit myregrw_exit(void)
{
#ifdef REGISTER_1	
	unregister_chrdev(major, DEVICE_NAME);
	printk(KERN_ALERT "unregister chrdev %d\n", major);
#endif

#ifdef REGISTER_2
	cdev_del(my_cdev);
	unregister_chrdev_region(devid, 1);
#endif

}

static int myregrw_open(struct inode *inode, struct file *file)
{
	static int counter = 0;
	if(myregrw_opened){
		return -EBUSY;
	}
	myregrw_opened++;
	sprintf(msg, "I've already tole you %d times, hello world\nmyregrw_open=%d\n", \
			counter++, myregrw_opened);
	msg_ptr = msg;
	try_module_get(THIS_MODULE);
	return SUCCESS;
}

static int myregrw_release(struct inode *inode, struct file *file)
{
	myregrw_opened--;
	module_put(THIS_MODULE);
	return 0;
}

static ssize_t myregrw_read(struct file *file, char __user *buf, size_t length, loff_t *offset)
{
	int bytes_read = 0;
	if(*msg_ptr =='\0'){
		return 0;
	}

	printk(KERN_ALERT "length=%d\n", length);
	while(length && *msg_ptr){
		put_user(*(msg_ptr++), buf++);
		length--;
		bytes_read++;
	}
	return bytes_read;
}

static ssize_t myregrw_write(struct file *file, const char __user *buf, size_t length, loff_t *offset)
{
	printk(KERN_ALERT "Sorry, this operation isn't supported\n");
	return -EINVAL; 
}


/* display controller register I/O routines */
#ifdef  EVB_IM9815
static __inline__ unsigned long read_reg(u32 phy_addr)
{
	return (ioread32(IO_ADDRESS(phy_addr)));
}
static __inline__ unsigned long write_reg(u32 phy_addr, u32 val)
{
	iowrite32(val, IO_ADDRESS(phy_addr));
	return (val);
}
#else
static __inline__ unsigned long read_reg(u32 phy_addr)
{
	return (ioread32((void __iomem *)phy_addr));
}
static __inline__ unsigned long write_reg(u32 phy_addr, u32 val)
{
	iowrite32(val, (void __iomem *)phy_addr);
	return (val);
}
#endif



static int myregrw_ioctl(struct inode *inode, struct file *filp, unsigned int cmd, unsigned long arg)
{
		int i, err = 0, retval, ret = 0;

	/* don't even decode wrong cmds: better returning  ENOTTY than EFAULT */
	if (_IOC_TYPE(cmd) != MYREGRW_MAGIC) return -ENOTTY;
	if (_IOC_NR(cmd) > MYREGRW_MAXNR) return -ENOTTY;

	/*
	 * the type is a bitmask, and VERIFY_WRITE catches R/W
	 * transfers. Note that the type is user-oriented, while
	 * verify_area is kernel-oriented, so the concept of "read" and
	 * "write" is reversed
	 */
	if (_IOC_DIR(cmd) & _IOC_READ)
		err = !access_ok(VERIFY_WRITE, (void __user *)arg, _IOC_SIZE(cmd));
	else if (_IOC_DIR(cmd) & _IOC_WRITE)
		err =  !access_ok(VERIFY_READ, (void __user *)arg, _IOC_SIZE(cmd));
	if (err)
		return -EFAULT;

	switch(cmd) {

	case MYREGRW_CMD0:							
		//scullp_qset = SCULLP_QSET;
		//scullp_order = SCULLP_ORDER;
		break;

	case MYREGRW_CMD1: /* Set: arg points to the value */   
		//ret = __get_user(scullp_order, (int __user *) arg);
		break;

	case MYREGRW_CMD2: /* Tell: arg is the value */			
		//scullp_order = arg;
		break;

	case MYREGRW_CMD3: /* Get: arg is pointer to result */		
		//ret = __put_user (scullp_order, (int __user *) arg);
		break;

	case MYREGRW_WRITE: /* eXchange: use arg as pointer */		
		printk("register info read from userspace.\n");
		printk("----------------------------------\n");
		for(i=0; i<2; i++) {
			retval = __get_user(reg_info[i], (int __user *)arg + i);
			if (retval == 0) {
				printk("reg_info[%d] = 0x%lx\n", i, reg_info[i]);
			}
		}

		printk("register info write to userspace .\n");
		printk("----------------------------------\n");
		for(i=0; i<2; i++) {
			reg_info[i] += 3;
			retval = __put_user(reg_info[i], (int __user *)arg + i);
			if (retval == 0) {
				printk("reg_info[%d] = 0x%lx\n", i, reg_info[i]);
			}
		}
		break;

	case MYREGRW_READ: /* eXchange: use arg as pointer */		
		printk("Physical address:\n");
		printk("-----------------\n");
		retval = __get_user(reg_info[0], (unsigned long __user *)arg);
		if (retval == 0) {
			//printk("reg_info[0] = %lx\n", reg_info[0]);
			printk("PHY ADDR = 0x%lx\n", reg_info[0]);
		}
		
		printk("\n\n");

		printk("write the content of the PHY ADDR into userspace .\n");
		printk("--------------------------------------------------\n");
		retval = __put_user(read_reg(reg_info[0]), (unsigned long __user *)arg+1);
		if (retval == 0) {
			printk("Content of PHY ADDR 0x%lx = 0x%lx\n", reg_info[0], read_reg(reg_info[0]));
		}
	
		break;

	default:  /* redundant, as cmd was checked against MAXNR */
		return -ENOTTY;
	}

	return ret;
};

module_init(myregrw_init);
module_exit(myregrw_exit);

MODULE_AUTHOR("edwin");
MODULE_LICENSE("GPL");
