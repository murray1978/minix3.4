/*
	USBD
	USB Driver, from scratch
	based off time driver tutorial for minix3
	Using rtl8169.c and at_wini.c drivers as base.
	syslib.h - SEF framework
	pci.h - pci driver.
		pci_inb
		pci_inw
		pci_outb
		pci_outw
		pcii_rreg8 - read/write to pci registers.
*/
#include <stdio.h>
#include <stdlib.h>
#include <minix/drivers.h>
#include <minix/ds.h>
#include <minix/chardriver.h>
#include <dev/pci/pciio.h>
#include <minix/rs.h>

#include <minix/sysutil.h>
#include <minix/type.h>
#include <minix/endpoint.h>
#include <minix/syslib.h>
#include <sys/ioc_disk.h>
#include <machine/pci.h>
#include <sys/mman.h>

#include "usbd.h"

/* Function prototypes for the usbd driver */
int usbd_open(devminor_t minor, int access, endpoint_t user_endpt);
int usbd_close(devminor_t minor);
ssize_t  usbd_read(devminor_t minor, u64_t position, endpoint_t usr_endpt, cp_grant_id_t grant, size_t size, int flags, cdev_id_t id);
ssize_t  usbd_write(devminor_t minor, u64_t position, endpoint_t usr_endpt, cp_grant_id_t grant, size_t size, int flags, cdev_id_t id);
static void usbd_other(message *m, int ipc_status);
static int usbd_ioctl(devminor_t minor, unsigned long request, endpoint_t endpt, cp_grant_id_t grant, int flags, endpoint_t user_endpt, cdev_id_t id);

/*Convert BCD to Binary*/
unsigned bcd_to_bin(unsigned value);

/*SEF functions and varibles*/
void sef_local_startup(void);
int sef_cb_init_fresh(int type, sef_init_info_t *info);

/*Entry points to the usbd driver */
static struct chardriver usbd_tab =
{
	.cdr_open = usbd_open,
	.cdr_close = usbd_close,
	.cdr_read = usbd_read,
	.cdr_write = usbd_write,
	.cdr_ioctl = usbd_ioctl,
	.cdr_other = usbd_other
};

/* USB Device structure*/
struct USB_DEVICE 
{
	unsigned int did;	//device id
	unsigned int vid;	//vendor id
};

//enum usb_type = {hub, etc, null};
struct USB_DEVICE usb_device;


struct PCI_DEVICE
{
	int port;		//port pci device is on
	int busind; 	//bus pci is on
	u16_t uid;		//vid
	u16_t vid;		//pid
	const char *dname;		//device name
	int devind;
	u8_t bcr;		//baseclass register
	u8_t scr;		//Subclass register
	u32_t bar;		//Base address register?
};

struct PCI_DEVICE pci_device;

/*
	PCI Probe,
	Looks for USB devices
	
*/
static int pci_probe( int skip, u16_t *vidp, u16_t *didp)
{
	int  r, devind;
	const char *dname;
	u8_t v8;
	u16_t v16;
	u32_t v32;
	
	pci_init();
	
	r = pci_first_dev(&devind, vidp, didp);
	if( r<= 0 ) return -1;
	//printf("%i, %i\n", (int)vidp, (int)didp );
	//dname = pci_dev_name(vidp, didp);
	//printf("Fisrt device %x:%x = %s\n", vidp, didp, dname );
	
	while(r > 0)
	{
		/*
			Better whay to do this for other vid:did combo's
			Or multiple USB Controller's
		*/
		if( *didp == 0x265C ) //Virtual box 82801FB/FR EHCI controller
		{
			dname = pci_dev_name(*vidp, *didp);
			pci_device.vid = *vidp;
			pci_device.uid = *didp;
			pci_device.dname = dname;
			printf("using device %X:%X = %s\n", pci_device.vid, pci_device.uid, pci_device.dname );
			pci_device.port = 0x0000;
			pci_device.devind = devind;
			printf("\tport = 0x%X, device id = %d\n", pci_device.port, pci_device.devind );
			pci_device.busind = 0x0000;
			//print_capabilities(pci_device.devind);
			//pci_reserve(devind);		//reserve pci device, needed?
			
			//Base Clase Register =
			v8 = pci_attr_r8(pci_device.devind, PCI_BCR);
			pci_device.bcr = v8;
			printf("\tBase Class Register = 0x%X\n", pci_device.bcr);
			
			//Sub Class Register =
			if( (v8 = pci_attr_r8(pci_device.devind, PCI_SCR)) != 0x03)
			{
				panic("\nNOT a USB device subclass = 0x%X", v8);
				return -1;	//not a pci-usb device
			}
			pci_device.scr = v8;
			printf("\tSub Class Register = 0x%X\n", pci_device.scr);
			
			//Base address
			//v32= pci_attr_r32(devind, TI_CB_BASEADDR);
			//printf("\tUSB base address 0x%X\n",v32);
			
			return devind;
		}
		r = pci_next_dev(&devind, vidp, didp);
		if( r<= 0 ) return -1;	//no more devices found that match
	}
	return -1; //no devices found that match
}

static void usbd_other(message *m, int ipc_status)
{
		printf("usbd_other()\n");
}

static int usbd_ioctl(devminor_t minor, unsigned long request, endpoint_t endpt, cp_grant_id_t grant, int flags, endpoint_t user_endpt, cdev_id_t id)
{
	printf("usbd_ioctl\n");
	return OK;
}


int usbd_open(devminor_t minor, int access, endpoint_t user_endpt)
{
	printf("usbd_open()\n");
	
	return OK;
}

int usbd_close(devminor_t minor)
{
	printf("usbd_close()\n");
	return OK;
}


unsigned bcd_to_bin(unsigned value)
{
	return(value & 0x0f) + ((value>>4) * 10);
}


ssize_t usbd_read( devminor_t minor, u64_t position, endpoint_t endpt,
		cp_grant_id_t grant, size_t size, 
		int flags, cdev_id_t id)
{
	int bytes, ret;
	char buffer[1024];

	return OK;
}

ssize_t  usbd_write(devminor_t minor, u64_t position, endpoint_t usr_endpt, cp_grant_id_t grant, size_t size, int flags, cdev_id_t id)
{
	return OK;
}

void sef_local_startup()
{
	/*register init callbacks*/
	sef_setcb_init_fresh(sef_cb_init_fresh);
	sef_setcb_init_lu(sef_cb_init_fresh);
	sef_setcb_init_restart(sef_cb_init_fresh);

	/*register live update call backs */
	sef_setcb_lu_prepare(sef_cb_lu_prepare_always_ready);
	sef_setcb_lu_state_isvalid(sef_cb_lu_state_isvalid_standard);
	
	/*register shutdown callbacks*/
	
	/*register interupt callbacks*/

	sef_startup();

}

static int init_params(void)
{
	
	
	return 0;
}

int sef_cb_init_fresh( int type, sef_init_info_t *info)
{
	int devind;
	int skip;
	u16_t vid, did;
	
	/*set USB parameters */
	skip = init_params();
	
	printf("usbd_read()\n");
	/*PCI probe*/
	devind = pci_probe( skip, &vid, &did );
	if ( devind < 0 )
	{
		panic("No PCI-USB found");
		return ENODEV;
	}

	
	return OK;
}

int main( int argc, char **argv)
{
	sef_local_startup();

	chardriver_task(&usbd_tab);
	return OK;
}
