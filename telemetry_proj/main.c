#include <stdio.h>
#include <libirig106.a>
#include <stdint.h>
#include <config.h>

struct FileParameters{
	int fileHandle; //this will be used to identify the file across the irig106 library
			//UNSURE: will be assigned a value in enI106Ch10Open
}

struct PacketParameters{
	EnStatus enStatus;
	int buffer_size;
	int data_buffer;
}

int readch10File(int file_handle, char filename[]){
	bool keep_reading;
	int buffer_size;

	enI106Ch10Open(&file_handle, filename, I106_READ);

	Enstatus enStatus = enI106Ch10ReadNextHeader(file_handle,);
	while (enStatus == I106_OK){
		enstatus


		switch (suI106Hdr.ubyDataType){
			case

		}


		enstatus = enI106Ch10ReadNextHeader(file_handle, );

	}

}



int decodeCh10Packet(){
	//may have to write own decoding system for imagery -- no usable module
	switch (FileParameters.header.ubyDataType) {
		case I106CH10_DTYPE_COMPUTER_0{ //0x00 computer 0 (recorder data)
		}
		case I106CH10_DTYPE_USER_DEFINED{// user_defined
		}
		case I106CH10_DTYPE_COMPUTER_1{//0x01 computer 1 (TMATS recorder config)
		}
		case I106CH10_DTYPE_TMATS{ //
		}
		case I106CH10_DTYPE_COMPUTER_2{//0x02 computer 2 (records occurance of "events"
		}
		case I106CH10_DTYPE_RECORDING_EVENT{
		}
		case I106CH10_COMPUTER_3{//0x03 computer 3 (used for recording index -- searching)
		}
		case I106CH10_RECORDING_INDEX{//
		}
		case I106CH10_DTYPE_COMPUTER_4{//0x04 computer 4 (unused as of March'17)
		}
		case I106CH10_DTYPE_COMPUTER_5{//0x05 computer 5 (unused as of March'17)
		}
		case I106CH10_DTYPE_COMPUTER_6{//0x06 computer 6 (unused as of March'17)
		}
		case I106CH10_DTYPE_COMPUTER_7{//0x07 computer 7 (usused as of March'17)
		}
		case I106CH10_DTYPE_PCM_FMT_0{//0x08 PCM (unused as of March'17)
		}
		case I106CH10_DTYPE_PCM_FMT_1{//0x08 pulse code modulation
		}
		case I106CH10_DTYPE_PCM{//0x0A? unused as of March'17
		}
		case I106CH10_DTYPE_IRIG_TIME{//0x10 Time Data (unused as of March'17)
		}
		case I106CH10_DTYPE_1553_FMT_1{//
		}
		case I106CH10_DTYPE_1553_FMT_2{//
		}
		case I106CH10_DTYPE_ANALOG{//
		}
		case I106CH10_DTYPE_DISCRETE{//
		}
		case I106CH10_DTYPE_MESSAGE{//
		}
		case I106CH10_DTYPE_ARINC_429_FMT_0{
		}
		case I106CH10_DTYPE_VIDEO_FMT_0{
		}
		case I106CH10_DTYPE_VIDEO_FMT_1{
		}
		case I106CH10_DTYPE_VIDEO_FMT_2{
		}
		case I106CH10_DTYPE_IMAGE_FMT_0{
		}
		case I106CH10_DTYPE_IMAGE_FMT_1{
		}
		case I106CH10_DTYPE_UART_FMT_0{
		}
		case I106CH10_DTYPE_1394_FMT_0{
		}
		case I106CH10_DTYPE_1394_FMT_1{
		}
		case I106CH10_DTYPE_PARALLEL_FMT_0{
		}
		case I106CH10_DTYPE_ETHERNET_FMT_0{
		}
		case I106CH10_DTYPE_CAN{
		}
	}
}



int main(int argc, char* argv[]){



	readCh10File(fileHandle);
	return 0;
}
