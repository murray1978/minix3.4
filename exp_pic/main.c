#include <minix/sysutil.h>
#include <minix/type.h>
#include <minix/endpoint.h>
#include <minix/syslib.h>
#include <sys/ioc_disk.h>
#include <machine/pci.h>
#include <sys/mman.h>

/*
	Look at at_wini.c PCI_IDE driver.
*/

static long w_instance = 10;

int init_params()
{
		return w_instance;
}

static int w_probe( )
{
	int  r, devind;
	const char *dname;
	u16_t vidp, didp;
	
	pci_init();
	
	r = pci_first_dev(&devind, &vidp, &didp);
	if( r<= 0 ) return -1;
	//printf("%i, %i\n", (int)vidp, (int)didp );
	dname = pci_dev_name(vidp, didp);
	printf("Fisrt device = %s\n", dname );
	
	while(r > 0)
	{
		r = pci_next_dev(&devind, &vidp, &didp);
		if( r<= 0 ) return -1;
		//printf("%i, %i\n", (int)vidp, (int)didp );
		dname = pci_dev_name(vidp, didp);
		printf("Next device = %s\n", dname );
	}
	//pci_reserve(devind);
	return 0;
}



int main() {
	int skip, devind;
	u16_t vid, did;
	
	devind = w_probe();
	
    return 0;
}
