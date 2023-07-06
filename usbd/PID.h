#ifndef __PID_H__
#define __PID_H__

/*
	Packet Identifiers PID
*/

//token
#define PID_OUT	0b0001
#define PID_IN	0b1001
#define PID_SOF 0b0101
#define PID_SETUP 0b1101
//Data
#define PID_DATA0 0b0011
#define PID_DATA1 0b1011
#define PID_DATA2 0b0111
#define PID_MDATA 0b1111
//Handshake
#define PID_ACK	0b0010
#define PID_NAK	0b1010
#define PID_STALL 0b1110
#define PID_NYET 0b0110
//special
#define PID_PRE 0b1100
#define PID_ERR 0b1100
#define PID_SPLT 0b1000
#define PID_PING 0b0100
#define PID_RES	0b0000


#endif