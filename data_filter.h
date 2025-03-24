#ifndef DATA_FILTER_H_
#define DATA_FILTER_H_

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

void filter_data(sensor_data* data, uint8_t sz, float range_low, float range_high, float percent, uint8_t spike_width);

#endif // !DATA_FILTER_H_