/*
 * queue.h
 *
 *  Created on: Jul 20, 2016
 *      Author: pgi8114
 */

#ifndef UTIL_H_
#define UTIL_H_


/*
 * A struct that can hold a coordinate. Really only used for queue
 */
struct tuple {
	int x;
	int y;
};

/**
 * A struct that stores the information about an object
 */
struct Object {
	float maxval;    // value at position
	int position[2]; // position of pixel of max intensity of object in full image.
					 // this is the central pixel of the postage stamp.
	float center[2]; // center based on COM evaluated over small area around position.
};

/**
 * A node for object linked list
 */
struct Object_Node {
	struct Object *obj;
	struct Object_Node *next;
};


struct Stars {
	int frame;
	float xcen[3*NUM_STAR_TRACK];
	float ycen[3*NUM_STAR_TRACK];
	int xpos[3*NUM_STAR_TRACK];
	int ypos[3*NUM_STAR_TRACK];
};

/*
 * structures used for queue
 */
struct Q_Node {
	struct tuple data;
	struct Q_Node* next;
};

struct Queue {
	struct Q_Node* front;
	struct Q_Node* rear;
};


extern unsigned short errorout[1024];

//function declarations
//main.c
int read_binary(int argc, char *argv[]);
int read_int(int alpha, int beta, char *file[]);
int process_im(int *img, int *th_img, int *th_pos, int alpha, int beta);

//detect.c
struct Object_Node* detect_stars( int *img, int *th_img, int *th_pos, FILE * log);
struct Object* extract_star( int *img, struct tuple pos, FILE * log  );
struct tuple flood_fill( int *img, int *th_img, struct tuple start );
void set_centroid( float *image, struct Object *obj, FILE * log );
int is_star( float *image, struct Object *obj, FILE * log );
float shape( float *image, struct Object *obj );
float sharp( float *image, struct Object *obj );
float com( float *image, int dim );

//track.c
struct Stars* choose_stars( struct Object_Node* root, int counter);
struct Stars* get_previous_stars( int counter, int flag  );
struct Stars* track_stars( struct Stars* prev_stars, struct Object_Node* root, FILE * log );
struct Stars* search_third_star( struct Object_Node *first_star,
	struct Object_Node *second_star, int side,
	float prev_dist1, float prev_dist2 );
struct Stars* match_stars( struct Object_Node *first_star,
	struct Object_Node *second_star, struct Object_Node *third_star );
void set_stars( struct Stars* stars,  int flag );
void save_pststamps( struct Stars* stars, int *img );

//attitude.c
int attitude_correction( struct Stars* prev_stars, struct Stars* curr_stars,  FILE * log );

//util.c
void print_list( struct Object_Node* root, FILE * log );
void print_log();
void clean_up( struct Object_Node* root );
float distance( float x1, float x2, float y1, float y2 );
int offset( int size, int x, int y );
void enqueue(struct Queue *Q, struct tuple coordinate );
void dequeue(struct Queue *Q);
struct tuple getFront(struct Queue *Q);
int isEmpty(struct Queue *Q);


//JEFF'S EDITS

//uart.c



struct AcsOutput{
	/* 
	output format is in the form of:
	
	byte		|		description
	---------------------------------------
	0,1					ascii "C",ascii "T"
	2,3,4,5				32bit frame counter
	6,7					uint16 status word
	8,9,10,11			X
	12,13,14,15			Y
	16,17,18,19			Z
	checksum			u8 sum, no overflow
	*/
	char header[2];
	uint32_t frame_counter;
	uint16_t status_word;
	int	x;
	int y;
	int z;
	uint8_t checksum;
}


struct UartOutput{
	char header[] = CSTARS_TELEMETRY_HEADER;
	unsigned short num_postage_stamps;
	unsigned short data[ERROR_OUT_SIZE];
	char footer[] = CSTARS_TELEMETRY_FOOTER;
}
#endif /* UTIL_H_ */
