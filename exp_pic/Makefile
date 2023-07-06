#CC = cc
#CFLAGS = -Wall -Werror

#SRCS = main.c 
#OBJS = $(SRCS:.c=.o)
#EXEC = exp_pic

#all: $(EXEC)

#$(EXEC): $(OBJS)
#	$(CC) $(CFLAGS) $(OBJS) -o $(EXEC)

#%.o: %.c
#	$(CC) $(CFLAGS) -c $< -o $@

#clean:
#	rm -f $(OBJS) $(EXEC)
# Makefile for the AT disk driver (AT_WINI)
.include <bsd.own.mk>
.include <bsd.subdir.mk>
PROG=	exp_pci
SRCS=	main.c

DPADD+=	${LIBBLOCKDRIVER} ${LIBSYS} ${LIBTIMERS}
LDADD+=	-lblockdriver -lsys -ltimers

.include <minix.service.mk>