#include <stdlib.h>
#include <math.h>
#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include "find_objects.h"
#include "data_filter.h"

typedef struct
{
  float delta;
  uint8_t i;
} delta;

int delta_comp(const void *a, const void *b)
{
  delta* delta_a = (delta*)a;
  delta* delta_b = (delta*)b;
  return fabs(delta_b->delta) - fabs(delta_a->delta); 
}

int i_comp(const void *a, const void *b)
{
  delta* delta_a = (delta*)a;
  delta* delta_b = (delta*)b;
  return delta_a->i - delta_b->i;
}

float findMax_dist(sensor_data* data, uint8_t size)
{
	float maxVal = data->dist;
	data++;
	
	uint8_t i;
	for (i = 1; i < size; i++)
	{
		if (data->dist > maxVal)
		{
			maxVal = data->dist;
		}
		data++;
	}

	return maxVal;
}

float findMin_dist(sensor_data* data, uint8_t size)
{
	float minVal = data->dist;
	data++;
	
	uint8_t i;
	for (i = 1; i < size; i++)
	{
		if (data->dist < minVal)
		{
			minVal = data->dist;
		}
		data++;
	}

	return minVal;
}

sensor_data* filter_data(sensor_data* data, uint8_t sz, uint8_t desired_range, uint8_t min_width)
{
  //find max and min data values
  float max_dist = findMax_dist(data, sz);
  float min_dist = findMin_dist(data, sz);
  float dist_range = max_dist - min_dist; 
  float qtr_dist = dist_range / 4.0;
  if ((dist_range / desired_range) > 2 || (dist_range / desired_range) < 0.5)
  {
    printf("no objects captured in data\n");
    return data;
  }

  //find deltas
  delta* deltas = calloc(sz, sizeof(delta));
  if (deltas == NULL)
  {
    printf("calloc in filter_data failed\n");
    return data;
  }

  uint8_t i;
  for (i = 0; i < sz - 1; i++)
  {
    deltas[i].delta = data[i].dist - data[i + 1].dist;
    deltas[i].i = i;
  }

  //sort by largest deltas
  qsort(deltas, sz, sizeof(deltas[0]), delta_comp);

  //get all deltas greater than 2 quarters
  void* new_ptr;
  uint8_t detla_sz;
  for (i = 0; i < sz; i++)
  {
    if (fabs(deltas[i].delta) < 2 * qtr_dist)
    {
      detla_sz = i;
      new_ptr = realloc(deltas, sizeof(delta) * detla_sz);
      if (new_ptr != NULL) deltas = new_ptr;
      break;
    }
  }

  //sort deltas by index
  qsort(deltas, sz, sizeof(deltas[0]), i_comp);

  uint8_t chunk_count;


  sensor_data** data_chunks = calloc(chunk_count, sizeof(sensor_data*));

  //break sensor data array into chunks based on deltas
  for (i = 0; i < detla_sz; i++)
  {
    
  }

  free(deltas);
  return data; 
}