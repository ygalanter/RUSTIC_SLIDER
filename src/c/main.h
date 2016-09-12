#pragma once
#include "pebble.h"  


// array of resources IDs for numbers
int NUM_NUM[] = {RESOURCE_ID_NUM_0, RESOURCE_ID_NUM_1, RESOURCE_ID_NUM_2, RESOURCE_ID_NUM_3, RESOURCE_ID_NUM_4,
                 RESOURCE_ID_NUM_5, RESOURCE_ID_NUM_6, RESOURCE_ID_NUM_7, RESOURCE_ID_NUM_8, RESOURCE_ID_NUM_9};

int h1 = -1, h2 = -1, m1 = -1, m2 = -1; // to persit previous time


// positions of layers of numbers HHMM

#if PBL_RECT
static GRect NUM_COORDS[] =  {
  {{0,55},{36, 56}},  
  {{36,55},{36, 56}},  
  {{72,55},{36, 56}},  
  {{108,55},{36, 56}}
 };
#else 
static GRect NUM_COORDS[] =  {
  {{16,62},{36, 56}},  
  {{54,62},{36, 56}},  
  {{92,62},{36, 56}},  
  {{130,62},{36, 56}}
 };

#endif

