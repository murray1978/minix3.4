/*
	USBD
	USB Driver, from scratch
	based off time driver tutorial for minix3
	Using rtl8169.c and w_ini.c drivers as base.
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
/**/
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

static int pci_probe( )
{
	int  r, devind;
	const char *dname;
	u16_t vidp, didp;
	
	pci_init();
	
	r = pci_first_dev(&devind, &vidp, &didp);
	if( r<= 0 ) return -1;
	//printf("%i, %i\n", (int)vidp, (int)didp );
	dname = pci_dev_name(vidp, didp);
	printf("Fisrt device %x:%x = %s\n", vidp, didp, dname );
	
	while(r > 0)
	{
		r = pci_next_dev(&devind, &vidp, &didp);
		if( r<= 0 ) return -1;
		//printf("%i, %i\n", (int)vidp, (int)didp );
		dname = pci_dev_name(vidp, didp);
		printf("Next device %x:%x = %s\n", vidp, didp, dname );
	}
	//pci_reserve(devind);
	return 0;
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

	printf("usbd_read()\n");
		/*PCI probe*/
	int n = pci_probe( );

	
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

	sef_startup();

}

int sef_cb_init_fresh( int type, sef_init_info_t *info)
{
	return OK;
}

int main( int argc, char **argv)
{
	sef_local_startup();

	chardriver_task(&usbd_tab);
	return OK;
}
