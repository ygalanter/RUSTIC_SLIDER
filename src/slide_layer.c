#include <pebble.h>
#include "slide_layer.h"

// Animation duration & delay  
#define ANIMATION_DURATION 1000
#define ANIMATION_DELAY 0  
  
// Animation initial horizontal pozition: -1 = from left, 0 = center, 1 = from right
#define ANIM_START_X  1
// Animation initial vertical pozition: -1 = from top, 0 = center, 1 = from bottom
#define ANIM_START_Y  1  
// you can combine both  
  
  
// array of resources IDs for numbers
int NUM_NUM[] = {RESOURCE_ID_NUM_0, RESOURCE_ID_NUM_1, RESOURCE_ID_NUM_2, RESOURCE_ID_NUM_3, RESOURCE_ID_NUM_4,
                 RESOURCE_ID_NUM_5, RESOURCE_ID_NUM_6, RESOURCE_ID_NUM_7, RESOURCE_ID_NUM_8, RESOURCE_ID_NUM_9};

// animation stop callback 
void on_animation_stopped(Animation *anim, bool finished, SlideLayer *slide_layer)
{
   // destroying old bitmap in static layer if it exists
   GBitmap *old_bitmap = (GBitmap *)bitmap_layer_get_bitmap(slide_layer->static_bitmap_layer);
   if (old_bitmap) gbitmap_destroy(old_bitmap);
  
   // saving new bitmap in static layer
   bitmap_layer_set_bitmap(slide_layer->static_bitmap_layer, slide_layer->gbitmap_digit);
}  
  
SlideLayer* slide_layer_create(GRect frame) {
  
  SlideLayer* slide_layer = malloc(sizeof(SlideLayer)); // allocating memory for side_layer items

	slide_layer->layer = layer_create(frame); // creating main layer
  slide_layer->current_Digit = -1;
  
  // creating bitmap layers
  GRect bound = GRect(0, 0, frame.size.w, frame.size.h);
  slide_layer->static_bitmap_layer = bitmap_layer_create(bound);
  layer_add_child(slide_layer->layer, bitmap_layer_get_layer(slide_layer->static_bitmap_layer));
  slide_layer->anim_bitmap_layer = bitmap_layer_create(bound);
  layer_add_child(slide_layer->layer, bitmap_layer_get_layer(slide_layer->anim_bitmap_layer));
  
  return slide_layer;                    

}

void slide_layer_destroy(SlideLayer *slide_layer) {
  
  if (slide_layer->gbitmap_digit) gbitmap_destroy(slide_layer->gbitmap_digit);
  bitmap_layer_destroy(slide_layer->static_bitmap_layer);
  bitmap_layer_destroy(slide_layer->anim_bitmap_layer);
  layer_destroy(slide_layer->layer);
  free(slide_layer);

}

Layer* slide_layer_get_layer(SlideLayer *slide_layer){
  return slide_layer->layer;
}

void slide_layer_animate_to(SlideLayer *slide_layer, uint8_t next_value){
  
  if (slide_layer->current_Digit != next_value) {
    
    slide_layer->current_Digit = next_value;
    
    slide_layer->gbitmap_digit = gbitmap_create_with_resource(NUM_NUM[next_value]);
    bitmap_layer_set_bitmap(slide_layer->anim_bitmap_layer, slide_layer->gbitmap_digit);
    
    // creating and setting animation
    GRect finish = layer_get_bounds(slide_layer->layer);
    GRect start = GRect(finish.size.w * ANIM_START_X, finish.size.h * ANIM_START_Y, finish.size.w, finish.size.h);
    slide_layer->anim = property_animation_create_layer_frame(bitmap_layer_get_layer(slide_layer->anim_bitmap_layer), &start, &finish);
  
    AnimationHandlers handlers = {
        .stopped = (AnimationStoppedHandler) on_animation_stopped
    };
    animation_set_handlers((Animation*) slide_layer->anim, handlers, slide_layer);
    
    animation_set_curve((Animation*) slide_layer->anim, AnimationCurveLinear);
    animation_set_duration((Animation*) slide_layer->anim, ANIMATION_DURATION);
    animation_set_delay((Animation*) slide_layer->anim, ANIMATION_DELAY);
    animation_schedule((Animation*) slide_layer->anim);
    
  }
}