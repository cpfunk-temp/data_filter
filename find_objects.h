#ifndef FIND_OBJECTS_H_
#define FIND_OBJECTS_H_

#include <stdlib.h>
#include <math.h>
#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include "data_filter.h"

void free_objs(obj_data** objects, uint8_t* const size);

obj_data* getObjects(obj_data* obj_list, uint8_t* const obj_list_sz, float cm_ping_cutoff);

#endif //!FIND_OBJECTS_H_
