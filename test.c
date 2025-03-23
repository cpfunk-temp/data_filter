#include <stdlib.h>
#include <math.h>
#include <stdio.h>
#include <string.h>
#include <stdint.h>

typedef struct {
	float dist;
	uint8_t angle;
} sensor_data;

sensor_data* add_dataPoint(sensor_data* data_buffer, uint8_t* const data_buffer_sz, float dist, uint8_t angle)
{
	void* new_p;
	(*data_buffer_sz)++;
	uint8_t alloc_sz =  8 * (1 + (*data_buffer_sz) / 8);
	if (*data_buffer_sz % 8 == 1) //allocate memory in chunks of 8 elements
	{
		new_p = realloc(data_buffer, alloc_sz * sizeof(sensor_data));
		if (new_p == NULL)
		{
		}
		else
		{
			data_buffer = new_p;
		}

	}
	(data_buffer + (*data_buffer_sz) - 1)->dist = dist;
	(data_buffer + (*data_buffer_sz) - 1)->angle = angle;
	
	return data_buffer;
}

int main(void)
{	
	uint8_t angle;
	float dist;

	uint8_t data_buffer_sz = 0;
	sensor_data* data_buffer = NULL;
	void* new_p;

	while (scanf("%d %f", &angle, &dist) != EOF)
	{
		new_p = add_dataPoint(data_buffer, &data_buffer_sz, dist, angle);
		if (new_p == NULL) perror("allocation for sensor value buffer failed");
		else
		{
			data_buffer = new_p;
		}
	}

	//printdata
	for (uint8_t i = 0; i < data_buffer_sz; i++)
	{
		printf("%d, %f\n", (data_buffer + i)->angle, (data_buffer + i)->dist);
	}

	return 0;
}