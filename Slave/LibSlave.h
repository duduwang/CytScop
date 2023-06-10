


#ifndef _LIB_SLAVE
#define _LIB_SLAVE



#include <stdbool.h>
#include<stdio.h>
#include<stdlib.h>
#include<memory.h>
#include<errno.h>
#include<modbus/modbus.h>





//链接modbus
int dev_conn(const char *device , int baud,char parity,int data_bit,int stop_bit);

//关闭modbus
int dev_close();

//led
int dev_LED(int isState);

#endif
