#include"LibSlave.h"




modbus_t *ctx =0;


//链接modbus
int dev_conn(const char *device , int baud,char parity,int data_bit,int stop_bit)
{
	// create a libmodbus context for RTU
	// doesn't check if the serial is really there
	ctx = modbus_new_rtu(device, baud, parity, data_bit, stop_bit);

	//设置串口模式
	modbus_rtu_set_serial_mode(ctx, MODBUS_RTU_RS232);
	if (ctx == 0) {

		fprintf(stderr, "Unable to create the libmodbus context\n");
		return -1;

	} else {
		struct timeval old_response_timeout;
		struct timeval response_timeout;

		// enable debug
		modbus_set_debug(ctx, true);



	}

	// try to connet to the first DZT on the line
	// assume that line address is 1, the default
	// send nothing on the line, just set the address in the context
	if(modbus_set_slave(ctx, 1) == -1) {
		fprintf(stderr, "Didn't connect to slave/n");
		return -1;
	}

	// establish a Modbus connection
	// in a RS-485 context that means the serial interface is opened
	// but nothing is yet sent on the line
	if(modbus_connect(ctx) == -1) {

		fprintf(stderr, "Connection failed: %s\n", modbus_strerror(errno));
		modbus_free(ctx);
		return -1;

	} else 
	{
		fprintf(stderr, "Connected\n");
		return 1;
	}	
}
//关闭modbus
int dev_close()
{
	modbus_close(ctx);
	modbus_free(ctx);

}

//led
int dev_LED(int isState)
{
	int nreg = 0;
	uint16_t tab_reg[32];
	nreg = modbus_read_registers(ctx,0,32,tab_reg);

	int i;

	// dump all registers content

	fprintf (stderr, "Register dump:\n");
	for(i=0; i < nreg; i++)
		printf("reg #%d: %d\n", i, tab_reg[i]);




}

int set(int register_offset ,uint16_t* val)
{
	int nreg = 0;
	const uint16_t reg  = val[0];
	nreg = modbus_write_register(ctx, register_offset,reg);
	return nreg;

}
int get(int register_offset ,uint16_t* val)
{
	int nreg = modbus_read_registers(ctx,register_offset,2,val);
	return nreg;
}





