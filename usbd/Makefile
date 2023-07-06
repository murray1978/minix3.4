#Makefile for time driver
PROG=	usbd
SRCS=	usbd.c

FILES=${PROG}.conf
FILESNAME=${PROG}
FILESDIR=/etc/system.conf.d

DPADD+= ${LIBCHARDRIVER} ${LIBSYS}
LDADD+= -lchardriver -lsys

.include <minix.service.mk>

