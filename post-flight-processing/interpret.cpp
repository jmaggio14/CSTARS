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


/*
-------------------FOLLOWING IS PSUEDOCODE------------------------

Open up our data file
look for unique marker
read X amount of bytes in HEADER
determine how much data is in there, and whether it overlaps with other markers
read out postage stamp x,y locations
read out postage stamps individuallty
reconstruct imagery with postage stamps using openCV
save image to openCV video writer

look for next header --> repeat


NOTES:
skip troublesome areas entirely
*/

//openCV
#include <opencv2/opencv.hpp>



int readData(char achFilename[], void *pData);


int main(int argc, char const *argv[]) {
  //reading in data file
  if (argc > 1){
    printf("must provide file name\n", );
  }
  else{
      char achFilename = argv[1];
  }

  void outPut
  readData(achFilename);

  return 0;
}



int readData(char achFilename,)
