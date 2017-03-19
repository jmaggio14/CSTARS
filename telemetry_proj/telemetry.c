/*
Chapter 10 telemetry Reader

can pull and decode packets from chapter 10 file or chapter 10 bitstream


                THIS CODE USES HUNGARIAN NOTATION
--------------------------------------------------------------------------------
        PREFIX        |             TYPE
    -----------------------------------------------------
          i           |       Signed Integer
          u           |       Unsigned Integer
          b           |       Boolean Flag
          ch          |       Character
          by          |       Byte
          sz          |       Null terminated char array
          en          |       Enumerated Type
          m_          |       Variable with module scope
          g_          |       Variable with global scope
          su          |       Structure variable
          Su          |       Structure Name
          a           |       Array of...
          p           |       Pointer to
--------------------------------------------------------------------------------
.

TO DO
need to add precompiler debugs
check libirig106 imports --> ".a"
how to work with dropped packets?
macros for outputting to file if necessary

*/


//IRIG
#include "config.h"
#include "libirig106.a"
//std libraries
#include <stdio.h>
#include <stflib.h>
#include <stdint.h>

//for cmd args
#define CSTARS_FILE 0;
#define CSTARS_STREAM 1;
//default values
#define CSTARS_STREAM_HANDLE_DEFAULT 1;
#define CSTARS_DEFAULT_BUFFER_SIZE 1024; //default overly-large value
#define CSTARS_STARTING_PACKET_SEQUENCE = 0x00;


//Packet Parameters Struct -- stores all pertinent information for each packet
struct SuPacketParameters{
  int iHandle;
  unsigned int uBufferSize;
  bool bSecondHeaderExists;
  int iHeaderLength;
  int iTrailerLength;
  int iDataLength;
  struct SuI106Ch10Header suHeader;
  void *pvBuffer;
  void *data;
}

//PERSISTENT DATA FOR STREAM
struct SuPersistent{
  uint8_t uStartingSequenceNumber = CSTARS_STARTING_PACKET_SEQUENCE;
  uint16_t uCstarsId;
  int iTotalPackets;
  int iDroppedPackets;
  int *aiDroppedPacketLocations[];
}

struct SuOutputBinaryPacket{
  int separator[25] = {0};
  int iPacketNumber;
  void *pData;
}

// struct SuPacketBox{
//   void *psuPackets;
// }

//defining function
int readCh10File(char achFilename[], struct SuPacketParameters *psuPacketParam, struct SuPersistent *psuPersistent);
// int readCh10Stream(uint16_t uPort, struct SuPacketParameters *psuPacketParam);
int decodeCstarsPacket(struct SuPacketParameters *psuPacketParam, struct SuPersistent *psuPersistent);


int main(int argc, char const *argv[]) {
// [EXECUTABLE] [STREAM OR FILE] [FILENAME OR PORT] [CHANNEL ID]

  if (argc < 5){
    printf("need 4 args: [EXECUTABLE] [STREAM (0) OR FILE (1)] [FILENAME OR PORT] [CHANNEL ID] [OUTPUT FILENAME]\n", );
  }//end if
  else{
    struct SuPacketParameters suPacketParam; //creating parameter struct
    int iMode; //0 for Stream, 1 for FILE
    suPacketParam.uCstarsId = (uint16_t)argv[4]; //assigning channelID
    //setting up file vs stream parameters
    if( (int)argv[1] == CSTARS_FILE){ //using file not bitstream mode
      iMode = CSTARS_FILE;
      char achFilename[] = argv[2]; //setting 3rd arg as achFilename
      readCh10File(filename,&suPacketParam);
    }//end if
    else{
      iMode = CSTARS_STREAM;
      uint16_t uPort = (int)argv[2]; //setting 3 arg as port
      readCh10Stream(uPort,&suPacketParam);
    }//end else
  }//end else

  return 0;
}




//CHAPTER 10 FILE READING
int readCh10File(char achFilename, struct SuPacketParameters *psuPacketParam, , struct SuPersistent *psuPersistent){
    EnI106Status enStatus;
    uint8_t uPacketSequence = psuPersistent->uStartingSequenceNumber;

    //opening files
    enI106Ch10Open( &(psuPacketParam->iHandle), achFilename, I106_READ );
    while (true) {
      enStatus = enI106Ch10ReadNextHeader(psuPacketParam->iHandle,
                                                              &(psuPacketParam->suHeader);
      if (enStatus == I106_EOF){return 1;}//checking if end of file reached

      //reading data, packet saved in pvBuffer
      enStatus = en106Ch10ReadData(psuPacketParam->iHandle,
                                                 &(psuPacketParam->uBufferSize),
                                                   psuPacketParam->pvBuffer);

      //counting packets and also checking to see if packet sequence is correct
      //If packet channel ID matches our pre-defined CSTARS ID, then it's our packet
      if(psuPacketParam->suHeader.uChID == suPersistent->uCstarsId){
          (psuPersistent->iTotalPackets)++;
          uPacketSequence = uPacketSequence + 1;

          if(psuPacketParam->suHeader.ubySeqNum != uPacketSequence){
            (psuPersistent->iDroppedPackets)++;
          }//end if packet sequence

          decodeCstarsPacket(psuPacketParam);
      }//end if channel ID

    }//end while
    return 0;
}//end readCh10File()



// //CHAPTER 10 UDP STREAM READING
// int readCh10Stream(uint16_t uPort, struct SuPacketParameters *psuPacketParam){
//   EnI106Status enStatus; //stores error status
//   //assigning static handle value (this only works if you have 1 open stream!)
//   psuPacketParam->iHandle = CSTARS_STREAM_HANDLE_DEFAULT;
//
//   //opennig netstream
//   enStatus = enI106_OpenNetStream(psuPacketParam->iHandle,uPort);
//   if(enStatus != I106_OK){return 1}//end if unable to properly open handle
//
//   while(true){
//     enStatus = enI106_ReadNetStream(psuPacketParam->iHandle,
//                                          psuPacketParam->pvBuffer,
//                                          psuPacketParam->uBufferSize);
//     if (enStatus != I106_OK){return 1;}//end if error occurs
//     //CURRENTLY NOT SURE HOW TO DETERMINE DEFINED END OF STREAM
//
//     //WE ARE CURRENTLY NOT RECIEVING THE HEADER AT ALL
//     //If packet channel ID matches our pre-defined CSTARS ID, then it's our packet
//     if(psuPacketParam->suHeader.uChID == psuPacketParam->uCstarsId){
//       decodeCstarsPacket(psuPacketParam);
//     }//end if
//
//   }//end while
//   return 0;
// }

int decodeCstarsPacket(struct SuPacketParameters *psuPacketParam, struct SuPersistent *psuPersistent){
  //shorter pointers for readability'
  void *pData;
  void *packet = psuPacketParam->pvBuffer; //entire packet
  void *pStartingPoint = packet;
  struct SuI106Ch10Header *psuHeader = psuPacketParam->suHeader;//packet

  //----------------DETERMINING SIZE OF HEADER----------------
  //determining whether secondary header exists
  if ((packet->ubyPacketFlags & I106CH10_PFLAGS_SEC_HEADER) != 0){
    psuPacketParam->iHeaderLength = HEADER_SIZE + SEC_HEADER_SIZE;
  }
  else{
    psuPacketParam->iHeaderLength = HEADER_SIZE;
  }

  //------------------PULLING OUT DATA INTO OUR OWN BLOCK-----------------------
  pStartingPoint = (pStartingPoint + psuPacketParam->iHeaderLength);
  memcopy(pData,pStartingPoint,(size_t)packet->ulDataLen);

  saveToBinary(pData,psuPersistent->iPacketNumber); //saving to a raw binary as a backup



  return 0;

}



//NEED TO BE MODIFIED TO SAVE TO A FIFO
int saveToBinary(void *pData,int iPacketNumber){
    struct SuOutputBinaryPacket suOutput;
    suOutput.iPacketNumber = iPacketNumber;
    suOutput.pData = packet;

    FILE *pOutputFile = fopen("output.cstars","a");
    fwrite(suOutput,sizeof(suOutput),1,pOutputFile);
}


// int appendToPacketBox(void *packet, int iPacketNumber){
//   struct SuOutputBinaryPacket suOutput;
//   suOutput.iPacketNumber = iPacketNumber;
//   suOutput.pData = packet;
// }
