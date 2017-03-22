/*
 * parameters.h
 *
 *  Created on: Oct 24, 2016
 *      Author: pgi8114
 */

#ifndef PARAMETERS_H_
#define PARAMETERS_H_

//values that are predefined
#define DIMENSION 1024 		//Size of image
#define THRESHOLD 1197		//1100?//Threshold based on noise and star brightness

#define INNER 400 			//Number of pixels from border to look for stars
#define OUTER 200 			//Number of pixels away from center that stars can drift

#define SIZE 10				//Size of a postage-stamp
#define BACK 4				//Size of background border around postage-stamp

#define HWIDTH 0.8493218 	//((fwhm)/(2*sqrt(2*log(2)))), where fwhm = 2
#define WEIGHT 1 			//Test if this has effect. may need to change to array in sharp

#define SHAPE_UPP_BND 1     //Bounds for star checking parameters
#define SHAPE_LOW_BND 0
#define SHARP_UPP_BND 1.5
#define SHARP_LOW_BND 0

#define NUM_STAR_TRACK 3	//Number of groups of 3 stars to track.

#define POS_FN_MAIN "star_pos_001.txt"	//File where previous star positions are stored
#define POS_FN_SEC "star_pos_002.txt"	//File where previous star positions are stored
#define FRAME_OFFSET 3		// Number of previous frames to store when no match found
#define LPIX 1				//Deviation allowed on distance matching
#define EXPT_DRT 7 		//plus/min 17 pix
#define EXPT_ROT 0.5  	//plus/min pi/6

#define MAS_PER_PIX 21093.75 	//pixel conversion to milli-arc-second.
#define MAS_PER_RAD 206264806.247 	//radian conversion to milli-arc-second.


//JEFF's EDITS
#define CSTARS_TELEMETRY_HEADER (char)"start_rit_cstars" //16bytes
#define CSTARS_TELEMETRY_FOOTER (char)"endof_rit_cstars" //16bytes
#define POPPY_HEADER_SIZE 16
#define JEFF_HEADER_SHIFT_SIZE (POPPY_HEADER_SIZE + 18)
#define NUMBER_POSTAGE_STAMPS (unsigned short)9
#define ACS_HEADER "CT"
#define ERROR_OUT_SIZE 1024 //THIS WILL NEED TO SHRINK

#define TEMPERATURE_PATH "nasa/temperature.cstasrs"
#define ON_TARGET_PATH "nasa/on-target.cstars"
#define	SHUTTER_DOOR_PATH "nasa/shutter-door.cstars"


#define X_POSITION 13
#define Y_POSITION 14
#define Z_POSITION 15
#define FRAME_POSITION 9

//bit masks for status word
#define STATUS_WORD_TEMPLATE (unsigned short)0x0000 
#define ALIVE_MASK  = (unsigned short)0x8000 //first bit is high  [0]
#define BUSY_MASK   = (unsigned short)0x4000 //second bit is high [1]
#define LOCKED_MASK = (unsigned short)0x2000 //third bit is high  [2]
#define ROLL_MASK   = (unsigned short)0x1000 //fourth bit is high [3]
#define TARGET_MASK = (unsigned short)0x0800  //fifth bit is high [4]
#define DOOR_MASK   = (unsigned short)0x0400  //sixth bit is high [5]

#define UART_DEVICE "/dev/ttyACM0" //MAY HAVE TO BE CHANGED TO '/dev/ttyUSB*'
#define UART_CONNECTION_ATTEMPTS 10

#endif /* PARAMETERS_H_ */




