
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <omp.h>


#include "parameters.h"
#include "util.h"

//attempts to open multiple UART devices if they exist
int set_up_uart(void){
	//referencing global variable (in main.c)
	char device[] = UART_DEVICE;
	for(int i =0, i<UART_CONNECTION_ATTEMPTS, i++){
		device[-1] = (char)i;
		UartDevice_ptr = fopen(device,'w');
		if(UartDevice_ptr != NULL) break; //break if file open
		//else if exit(0); //DO NOT EXIT ON REAL CONNECTION
	}
}

//simply writes to the uart device file assuming, it's possible
int send_over_uart(struct UartOutput *message_ptr){
	if(UartDevice_ptr == NULL) set_up_uart();
	else fwrite( (const void *)message_ptr, 1, sizeof(message_ptr), UartDevice_ptr);
}


//USES GLOBAL VARIABLE errorout as message
struct UartOutput build_uart_output(void){
	struct UartOutput output;
	output.num_postage_stamps = NUMBER_POSTAGE_STAMPS;
	output.data = errorout;
	return output;
}


