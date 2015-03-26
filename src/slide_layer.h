#pragma once
  
#include <pebble.h>  



typedef struct {
	Layer       *layer;
  
  BitmapLayer *static_bitmap_layer;
  BitmapLayer *anim_bitmap_layer;

	PropertyAnimation *anim;
	
	GBitmap* gbitmap_digit;
  
	uint8_t current_Digit;

} SlideLayer;


SlideLayer* slide_layer_create(GRect frame);

void slide_layer_destroy(SlideLayer *slide_layer);

Layer* slide_layer_get_layer(SlideLayer *slide_layer);

void slide_layer_animate_to(SlideLayer *slide_layer, uint8_t next_value);