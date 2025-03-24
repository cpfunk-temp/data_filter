#include "find_objects.h"

float obj_width(short delta_angle, float ping_dist)
{
    return fabs(2*ping_dist*tan(abs(delta_angle)*M_PI/360.0f));
}

sensor_data* add_dataPoint(sensor_data* data_buffer, uint8_t* const data_buffer_sz, float dist, uint8_t angle)
{
	void* new_p;
	(*data_buffer_sz)++;
	uint8_t alloc_sz =  8 * (1 + *data_buffer_sz / 8) ;
	if (*data_buffer_sz % 8 == 1) //allocate memory in chunks of 8 elements
	{		
		new_p = realloc(data_buffer, alloc_sz * sizeof(sensor_data));
		if (new_p != NULL)
		{
			data_buffer = new_p;
		}
	}
	(data_buffer + *data_buffer_sz - 1)->dist = dist;
	(data_buffer + *data_buffer_sz - 1)->angle = angle;
	
	return data_buffer;
}

void print_data(sensor_data* data, uint8_t sz)
{
	uint8_t i;
	for (i = 0; i < sz; i++)
	{
		printf("%d\t%f\n", data[i].angle, data[i].dist);
	}
}

void free_dataPoints(sensor_data** data_buffer, uint8_t* const data_buffer_sz)
{
	free(*data_buffer);
	*data_buffer = NULL;
	*data_buffer_sz = 0;
}

sensor_data* get_dataPoints(sensor_data* data_buffer, uint8_t* const data_buffer_sz)
{
	void* new_p;
	uint8_t angle;
	float dist;

	while (scanf("%d %f", &angle, &dist) != EOF)
	{
		new_p = add_dataPoint(data_buffer, data_buffer_sz, dist, angle);
		if (new_p != NULL) data_buffer = new_p;
	}

	return data_buffer;
}

obj_data* add_obj(obj_data* objects, uint8_t* const size, obj_data* new_obj)
{
	void* new_p;
	(*size)++;
	uint8_t alloc_sz = 4 * (1 + *size / 4);
	if (*size % 4 == 1) //allocate memory in chunks of 4 elements
	{		
		new_p = realloc(objects, alloc_sz * sizeof(obj_data));
		if (new_p != NULL) objects = new_p;
	}
	*(objects + *size - 1) = *new_obj;
	
	return objects;
}

void free_objs(obj_data** objects, uint8_t* const size)
{
	free(*objects);
	*objects = NULL;
	*size = 0;
}

obj_data* getObjects(obj_data* obj_list, uint8_t* const obj_list_sz, float cm_ping_cutoff)
{

	obj_data object;
	//array to txt_buffer sensor data and angles
	uint8_t data_buffer_sz = 0; //ALWAYS initialize data_buffer_sz to zero, it will be resized by add_dataPoint
	sensor_data* data_buffer = NULL; //ALWAYS initialize data_buffer to NULL (otherwise realloc() won't work)

	//get data from input
	void* new_p;
	new_p = get_dataPoints(data_buffer, &data_buffer_sz);
	if (new_p != NULL) data_buffer = new_p;

	//filter data
	filter_data(data_buffer, data_buffer_sz, 0.0, 2.0, 0.20, 4);

	//print filtered data
	//print_data(data_buffer, data_buffer_sz);

	//analyze data from input
	uint8_t start_i = 0;
	uint8_t i;
	short right_edge = 0;
	short left_edge = 0;
    for (i = 0; i < data_buffer_sz; i++)
    {
        //check for ping distance below cutoff
        if (!start_i && data_buffer[i].dist < cm_ping_cutoff) //right edge detection
        {
            start_i = i;
        }
		else if (start_i && data_buffer[i].dist > cm_ping_cutoff) //left edge detection
		{
			right_edge = data_buffer[start_i].angle; 
			left_edge = data_buffer[i].angle;

			object.delta = abs(left_edge - right_edge);
			object.bisector = right_edge + object.delta/2;
			object.dist = data_buffer[(start_i + i)/2].dist;
			object.width = obj_width(object.delta, object.dist);

			new_p = add_obj(obj_list, obj_list_sz, &object);
			if (new_p != NULL) obj_list = new_p;
			
			//reset start index to zero
			start_i = 0;
		}
	}
	
	//make sure values in data_buffer are freed
	free_dataPoints(&data_buffer, &data_buffer_sz);
	
	//return pointer to list of objects found
	return obj_list;
}
