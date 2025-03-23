#ifndef FIND_OBJECTS_H_
#define FIND_OBJECTS_H_

#include <stdlib.h>
#include <math.h>
#include <stdio.h>
#include <string.h>
#include <stdint.h>

//struct to store collected sensor data and angles
typedef struct {
	float dist;
	uint8_t angle;
} sensor_data;

typedef struct {
	float dist;
	float width;
	int16_t delta;
	int16_t bisector;
} obj_data;

void free_objs(obj_data** objects, uint8_t* const size);

obj_data* getObjects(obj_data* obj_list, uint8_t* const obj_list_sz, float cm_ping_cutoff);

#endif //!FIND_OBJECTS_H_
