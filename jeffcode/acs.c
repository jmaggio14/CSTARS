/*
AUTHOR NOTES
==================

MUST DEFINE MAX_PTR_LENGTH AS 20 --> clear if longer

*/

/***************************** Include Files *********************************/
#include "xaxidma.h"
#include "xparameters.h"
#include "xdebug.h"

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <omp.h>


#include "parameters.h"
#include "util.h"



/******************** Constant Definitions **********************************/

/*
 * Device hardware build related constants.
 */

#define DMA_DEV_ID		XPAR_AXIDMA_0_DEVICE_ID //should be the specific one

#ifdef XPAR_AXI_7SDDR_0_S_AXI_BASEADDR
#define DDR_BASE_ADDR		XPAR_AXI_7SDDR_0_S_AXI_BASEADDR
#elif XPAR_MIG7SERIES_0_BASEADDR
#define DDR_BASE_ADDR	XPAR_MIG7SERIES_0_BASEADDR
#elif XPAR_MIG_0_BASEADDR
#define DDR_BASE_ADDR	XPAR_MIG_0_BASEADDR
#elif XPAR_PSU_DDR_0_S_AXI_BASEADDR
#define DDR_BASE_ADDR	XPAR_PSU_DDR_0_S_AXI_BASEADDR
#endif

#ifndef DDR_BASE_ADDR
#warning CHECK FOR THE VALID DDR ADDRESS IN XPARAMETERS.H, \
		 DEFAULT SET TO 0x01000000
#define MEM_BASE_ADDR		0x01000000
#else
#define MEM_BASE_ADDR		(DDR_BASE_ADDR + 0x1000000)
#endif


//these number are from Dma example
#define TX_BUFFER_BASE		(MEM_BASE_ADDR + 0x00100000)
#define RX_BUFFER_BASE		(MEM_BASE_ADDR + 0x00300000) 
#define RX_BUFFER_HIGH		(MEM_BASE_ADDR + 0x004FFFFF)

#define MAX_PKT_LEN		0x20 //--> (ACS output is 21bytes)

#define NUMBER_OF_TRANSFERS	10

/**************************** Structs *******************************/
		 //ADDED IN util.h --> kept here for easy reference
// struct AcsOutput{
	 
// 	output format is in the form of:
	
// 	byte		|		description
// 	---------------------------------------
// 	0,1					ascii "C",ascii "T"
// 	2,3,4,5				32bit frame counter
// 	6,7					uint16 status word
// 	8,9,10,11			X
// 	12,13,14,15			Y
// 	16,17,18,19			Z
// 	checksum			u8 sum, no overflow
	
// 	char header[2];
// 	uint32_t frame_counter;
// 	uint16_t status_word;
// 	int	x;
// 	int y;
// 	int z;
// 	uint8_t checksum;
// }

/*********************************************************************
*
* Sets up DMA device
*
*********************************************************************/


struct AcsOutput acsOutput;

int sendOverDma(struct AcsOutput *message){
	int error_status
	u8 *tx_buffer_ptr = (u8 *)TX_BUFFER_BASE;

	for(int i = 0, i < sizeof(message), i++){
		tx_buffer_ptr[i] = message[i]
	}
	//flushing src buffer
	Xil_DCacheFlushRange((UINTPTR)tx_buffer_ptr, sizeof(message)); 

	//ADDD IN MULTIPLE ATTEMPTS HERE?
	error_status = XAxiDma_SimpleTransfer(&AxiDma,(UINTPTR) tx_buffer_ptr,
										sizeof(message), XAXIDMA_DMA_TO_DEVICE);

	//REMEMBER TO DO SOMETHING WITH THIS IN ERROR HANDLING
	if (Status != XST_SUCCESS) {return XST_FAILURE;}

	while (XAxiDma_Busy(&AxiDma,XAXIDMA_DMA_TO_DEVICE)) {
			/* Waiting for devices to be ready before ending function*/}

	free(tx_buffer_ptr);
	return XST_SUCCESS;
}


int set_up_dma(u16 DeviceId){
	//SETTING UP VARS
	XAxiDma_Config *CfgPtr;

	/* INITATILIZES THE DMA DEVICE */
	CfgPtr = XAxiDma_LookupConfig(DeviceId);
	if (!CfgPtr) {
		xil_printf("No config found for %d\r\n", DeviceId);
		return XST_FAILURE;
	}

	Status = XAxiDma_CfgInitialize(&AxiDma, CfgPtr);
	if (Status != XST_SUCCESS) {
		xil_printf("Initialization failed %d\r\n", Status);
		return XST_FAILURE;
	}

	if(XAxiDma_HasSg(&AxiDma)){
		xil_printf("Device configured as SG mode \r\n");
		return XST_FAILURE;
	}

	//DISABLING INTERUPTS -- necessary for polling mode
	XAxiDma_IntrDisable(&AxiDma, XAXIDMA_IRQ_ALL_MASK,
						XAXIDMA_DEVICE_TO_DMA);
	XAxiDma_IntrDisable(&AxiDma, XAXIDMA_IRQ_ALL_MASK,
						XAXIDMA_DMA_TO_DEVICE);

	return 0;
}

//Jeff's edits

struct AcsOutput constructACS(void){
	struct AcsOutput acs_output;
	unsigned short status_word;

	//PULLING DATA DIRECTLY OUT OF errorout
	acs_output.header = ACS_HEADER;
	acs_output.frame_counter = errorout[9];
	acs_output.status_word = construct_status_word();
	acs_output.x = errorout[13];
	acs_output.y = errorout[14];
	acs_output.z = errorout[15];

	// //CALCULATING CHECKSUM
	uint8_t sum = 0;
	uint8_t *pChecksum = (uint8_t *) &acs_output; //recasting to series of char for math purposes
	for(int i=0, i<sizeof(acs_output)-1,i++){
		sum += pChecksum[i]
	}
	acs_output.checksum = sum;

	//Via Input Pointer
	return 0;
}

unsigned short construct_status_word(void){

	unsigned short status_word = STATUS_WORD_TEMPLATE;
	//pulling data directly from errorout
	unsigned short num_stars = errorout[10] 

	//ALIVE (always alive with this setup)
	status_word = status_word | ALIVE_MASK; //turns [0] high

	//BUSY, LOCKED, ROLL GOOD
	if(num_stars < 3){
	/*busy*/ 
		status_word = status_word | BUSY_MASK; //turns [1] high
	/*locked*/
		 //do nothing --> [2] remains low
	/*roll_good*/
		 //do nothing --> [3] remains low 
	}
	else{
	/*busy*/ 
		//do nothing --> [0] remains low
	/*locked*/
		status_word = status_word | LOCKED_MASK; //[2] goes high
	/*roll_good*/
		status_word = status_word | ROLL_MASK; //[3] goes high
	}

	//ON TARGET
	FILE *target_file_ptr;
	target_file_ptr = fopen(ON_TARGET_PATH, "r");
	if(target_file_ptr[0] == '1'){
		status_word = status_word | TARGET_MASK;  //[4] goes high
	}
	fclose(target_file_ptr);

	//DOOR 
	FILE *door_file_ptr;
	door_file_ptr = fopen(SHUTTER_DOOR_PATH, "r");
	if(door_file_ptr[0] == '1'){
		/*door*/ status_word = status_word | DOOR_MASK; //[5] goes high
	}
	flose(door_file_ptr);

	return status_word;

}
