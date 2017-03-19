//general imports
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
//from irig106 src
#include <libirig106.a>
#include <config.h>

struct Parameters{
	int handle;
	int error_status;
	int header_length;
	int trailer_length;
	int data_length;
	uint16_t port;
	uint16_t cstars_id; //need to determine what our ID will bed
	suI106Ch10Header header;
	unsigned int buffer_size = 1024;
	bool second_header;
	void *pvBuffer;
	void *data;

}


int readch10File(char filename[], struct Parameters *PacketParameters){
	//opening the file and checking error status, assigns handle
	PacketParameters->error_status = enI106Ch10Open(&(PacketParameters->handle),filename, I106_READ);

	while (true){
		//reading next header and data, checking error status on each iteration
		PacketParameters->error_status = enI106Ch10ReadNextHeader(PacketParameters->handle,PacketParameters->header);
		if (PacketParameters->error_status != I106_OK){
			errorPrint(PacketParameters->error_status,"Read Error occured, check docs for more info")
			break;
		} //breaking loop if End Of File reached or other error occurs
		PacketParameters->error_status = enI106Ch10ReadData(PacketParameters->handle,PacketParameters->buffer_size,PacketParameters->pvBuffer);

		//checking channelID to determine if this is our packet
		if (PacketParameters->header->uChID == PacketParameters->cstars_id){
			decodeCstarsPacket(PacketParameters);
		}
	}
	return 0;
}


int openBitstream(uint16_t port, struct Parameters *PacketParameters){
	PacketParameters->handle = 1;
	PacketParameters->port = port;
	while (true) {
	  PacketParameters->error_status = enI106_OpenNetStream(PacketParameters->handle, PacketParameters->port);
	  PacketParameters->error_status = enI106_ReadNetStream(PacketParameters->handle, &(PacketParameters->packet), PacketParameters->buffer_size);
		if (PacketParameters->error_status != I106_OK){
			errorPrint(PacketParameters->error_status,"Read Error occured, check docs for more info")
			break;

		//checking channelID to determine if this is our packet
		if (PacketParameters->header->uChID == PacketParameters->cstars_id){
			decodeCstarsPacket(PacketParameters);
		}
	}
  return 0;
}


int decodeCstarsPacket(void *parameters){
	void *packet = parameters->pvBuffer;

	//----------------BEGIN DETERMINING DATA SIZE----------------
	//determining whether secondary header exists
	if ((packet->ubyPacketFlags & I106CH10_PFLAGS_SEC_HEADER) != 0){
		parameters->header_length = HEADER_SIZE + SEC_HEADER_SIZE;
	}
	else{
		parameters->header_length = HEADER_SIZE;
	}

	//trailer determined by length of checksum
	int checksum_type = packet->ubyPacketFlags & 0x03;
	switch (checksum_type){
				//trailer length is entirely determined by checksum length (8bit, 16bit, or 32bit)
			case I106CH10_PFLAGS_CHKSUM_NONE :
					break;
			case I106CH10_PFLAGS_CHKSUM_8    :
					parameters->trailer_length = 1;
					break;
			case I106CH10_PFLAGS_CHKSUM_16   :
					parameters->trailer_length = 2;
					break;
			case I106CH10_PFLAGS_CHKSUM_32   :
					parameters->trailer_length = 4;
					break;
			default :
					parameters->trailer_length = 0;
				} // end switch checksum_type

	//NEED TO DETERMINE IF THERE ARE INTRA-PACKET HEADERS



	//determining the length of datastream
	parameters->data_length = packet->ulDataLen

	//finding starting point for data in packet buffer_size
	void *starting_point = packet + parameters->header_length;
	//copying out the data
	memcpy(parameters->data, starting_point, (size_t)packet->ulDataLen);
	//void parameters->data now contains data from packet


	//NOW HAVE TO DETEMINE HOW TO PARSE THIS SECTION OF MEMORY
	// --> MUST TALK TO POPPY ABOUT THIS


	//PACKET HEADER | 24 Bytes
	//PACKET SECONDARY HEADER | 0 Bytes or 12 Bytes --> determined via ubyPacketFlags 4th bit

	// for (i<packet length) |
	// star = pointer value
	// pointer ++


	//RESET ALL DATA

}

int write_binary(void *data, char filename[]){
		FILE *data_file;
		data_file = fopen(filename,"w");
		fwrite(data,1,sizeof(data),data_file) // images should be 16bit uints
}

int errorPrint(int error, char message[]){
	printf( ( message + "( " + (char)error + ")" ) );
}

bool fileExists(char filename[]){
	FILE *file;
	file = fopen(filename,"r");
	if(file){
		fclose(file);
		return true;
	}
	else{
		return false;
	}
}


int main(int argc, char* argv[]){
	//need to take in command line args here
	if (argc == 1 or argc == 2){
		printf("need 2 args, [FILENAME] [CHANNEL ID]\n", );
	}
	if (argc >= 3){
		if ( fileExists(argv[1]) ){
			printf("preparing to use filename: %s\n",argv[1]);
			printf("using CHANNEL ID: %s\n",argv[2] );
			char filename[] = argv[1];
			struct Parameters PacketParameters;
			PacketParameters.cstars_id = argv[2];
			readCh10File(filename,&PacketParameters);
		}
		else{
			printf("Invalid filename: %s\n",argv[1] );
			return 1;
		}
	}
	return 0;
}
