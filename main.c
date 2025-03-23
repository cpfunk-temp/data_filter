#include <stdlib.h>
#include <math.h>
#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include "find_objects.h"


void print_objects(obj_data* objs, uint8_t obj_count)
{
    uint8_t i;
    for (i = 0; i < obj_count; i++)
    {
        printf("Angular width:%d Width:%.3f Angle:%d Distance:%.1f Number:%d\r\n", objs[i].delta, objs[i].width, objs[i].bisector, objs[i].dist, i + 1);
    }
}

int main(void)
{	
	uint8_t obj_count = 0;
    void* new_p;
	obj_data* obj_found = NULL;
	
    new_p = getObjects(obj_found, &obj_count, 1.5);
    if (new_p != NULL) obj_found = new_p;

    print_objects(obj_found, obj_count);

	free_objs(&obj_found, &obj_count);
	return 0;
}