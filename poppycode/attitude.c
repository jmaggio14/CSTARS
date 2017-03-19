/*
 * attitude.c
 *
 *
 *  Created on: Oct 24, 2016
 *      Author: pgi8114
 */

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <omp.h>

#include "parameters.h"
#include "util.h"

/*
 * Takes the position info for the previous image and this image and computes
 * the change in position. Will identify the x_delta, y_delta, and r_delta
 * (rotation change). Will out put this information as a binary string.
 *
 * Input:
 * 			prev_stars: A struct Stars object that contains two list of the x and y
 * 				coordinates of the NUM_STARS_TRACK stars, grouped in sets of three.
 * 				See util.h for struct description.
 * 			new_stars: A struct Stars object that contains two list of the x and y
 * 				coordinates of the NUM_STARS_TRACK stars, grouped in sets of three.
 * 				See util.h for struct description.
 *
 * Output:
 * 			none
 *
 * Note: may want to add a check to see if it worked
 */
int attitude_correction( struct Stars* prev_stars,
		struct Stars* curr_stars, FILE * log ){
	if (prev_stars == NULL || curr_stars == NULL) {
		return (1); //ERROR
	}
	int i, index, tracked_r = 0, tracked_x = 0, tracked_y = 0;
	float delta_z = 0, delta_x = 0, delta_y = 0, delta_temp;
	double theta_1, theta_2, theta_temp;
	float tempx, tempy, newx, newy;
	FILE * out;


	for ( i = 0; i < NUM_STAR_TRACK; i++){
		index = i*3;
		printf("returned! %f \n",curr_stars->xpos[index]);
		if (curr_stars->xpos[index] == -1) continue;
		printf("returned! %f \n",curr_stars->xpos[index]);
		//position of first star of each group set as origin

		//align first star in the two images
		theta_1 = atan2(prev_stars->ypos[index] - prev_stars->ypos[index+1],
				prev_stars->xpos[index] - prev_stars->xpos[index+1]);

		theta_2 = atan2(curr_stars->ypos[index] - curr_stars->ypos[index+1],
				curr_stars->xpos[index] - curr_stars->xpos[index+1]);

		theta_temp = (theta_2 - theta_1); //+ theta_0 which is the x_0 line on the detector
		printf("group %d, z = %f\n", i, theta_temp);
		if (theta_temp > EXPT_ROT || theta_temp < (-1*EXPT_ROT)) continue;
		delta_z += (float)theta_temp; //+ theta_0 which is the x_0 line on the detector
		tracked_r += 1;

		//align first star in the two images, first make center of image the origin
		tempx = curr_stars->xpos[index] - DIMENSION/2 + 1/2;
		tempy = curr_stars->ypos[index] - DIMENSION/2 + 1/2;
		//rotation
		newx = (float)(tempx*cos(theta_temp) +
				tempy*sin(theta_temp) + DIMENSION/2 - 1/2);
		newy = (float)(-tempx*sin(theta_temp) +
				tempy*cos(theta_temp) + DIMENSION/2 - 1/2);

		delta_temp = newy - prev_stars->ypos[index];
		printf("group %d, x = %f\n", i, delta_temp);
		if (delta_temp > EXPT_DRT || delta_temp < (-1*EXPT_DRT)) continue;
		delta_y += delta_temp;
		//printf("y: %f\n", delta_temp);
		tracked_y += 1;

		delta_temp = newx - prev_stars->xpos[index];
		printf("group %d, x = %f\n", i, delta_temp);
		if (delta_temp > EXPT_DRT || delta_temp < (-1*EXPT_DRT)) continue;
		delta_x += delta_temp;
		//printf("x: %f\n", delta_temp);
		tracked_x += 1;


	}

	delta_z = delta_z/(float)tracked_r;
	delta_x = delta_x/(float)tracked_x;
	delta_y = delta_y/(float)tracked_y;
	printf("rotation: %f\n", delta_z);
	printf("x: %f\n", delta_x);
	printf("y: %f\n", delta_y);


	//rotation is correct, drift is not

	out = fopen("out.bin","wb");
	if (out == NULL) return (0); //ERROR

	if (delta_x != delta_x || delta_y != delta_y || delta_z != delta_z) {
		delta_x = 0;
		delta_y = 0;
		delta_z = 0;

		int positions[3] = {(int)delta_x, (int)delta_y, (int)delta_z};

		fprintf(log, "pixels mas\n%f %d\n%f %d\n%f %d\n",
				delta_x, positions[0], delta_y, positions[1], delta_z, positions[2]);
		printf("here!!!!\n");
		return(1);
	}

	int positions[3] = {(int)delta_x, (int)delta_y, (int)delta_z};

	fprintf(log, "pixels mas\n%f %d\n%f %d\n%f %d\n",
			delta_x, positions[0], delta_y, positions[1], delta_z, positions[2]);
	printf("pixels mas\n%f %d\n%f %d\n%f %d\n",
				delta_x, positions[0], delta_y, positions[1], delta_z, positions[2]);
	//fprintf(log, "%d, %d, %d\n\n", positions[0], positions[1], positions[2]);
	//fwrite(positions,sizeof(int),3,out);

	fclose(out);

	return (0);
}

