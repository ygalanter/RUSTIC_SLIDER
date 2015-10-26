#pragma once
#include "pebble.h"  


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

