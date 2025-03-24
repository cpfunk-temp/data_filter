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

float moving_avg(float avg, uint16_t i, float data)
{
  return avg + (data - avg) / i;
}

void filter_data(sensor_data* data, uint8_t sz, float range_low, float range_high, float percent, uint8_t spike_width)
{
  //find max and min data values
  float max_dist = findMax_dist(data, sz);
  float min_dist = findMin_dist(data, sz);
  float dist_range = max_dist - min_dist;
  float cutoff = range_high - (range_high - range_low) * percent;
  

  uint8_t i;
  uint8_t start_i = 0;
  uint8_t is_bump;
  float avg;
  float delta;
  float prev_delta = 0;
  float prev_dist = range_high;
  for (i = 0; i < sz; i++)
  {
    //subtitute values near range_high
    if (data[i].dist > cutoff)
    {
      data[i].dist = range_high;
    }

    //check for change in values greater than percent change
    if (i != sz - 1)
    {
      delta = data[i].dist - prev_dist;
    }
    else
    {
      delta = range_high - prev_dist;
    }

    if (fabs(delta) > dist_range * percent)
    {
      //substitute values between deltas with value of current moving average
      uint8_t j;
      for (j = start_i; j < i; j++)
      {
        data[j].dist = avg;
      }

      //if the is range corresponds to a bump rather than a dip
      //and (i - start_i) is smaller than minimum width
      //then these values correspond to a spike
      is_bump = prev_delta < 0 && delta > 0;
      if (is_bump && i - start_i <= spike_width)
      {
        //set start index to 1 less than start_i
        uint8_t st_i = start_i - (start_i != 0);
        
        //check if value is at edge of sensor data
        if (st_i == 0)
        {
           data[st_i].dist = range_high;
        }
        else if (i == sz - 1)
        {
          data[i].dist = range_high;
        }

        //interpolate inbetween values
        float m = (data[i].dist - data[st_i].dist) / (i - st_i);
        for (j = st_i; j <= i; j++)
        {
          data[j].dist = m * j + data[st_i].dist;
        }
      }

      //reset avg to zero
      avg = 0;
      //update start index
      start_i = i;
      //update prev_delta
      prev_delta = delta;
    }

    //collect moving average
    avg = moving_avg(avg, (i - start_i) + 1, data[i].dist);

    prev_dist = data[i].dist;
  }
}