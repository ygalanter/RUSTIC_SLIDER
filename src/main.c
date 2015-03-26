#include <pebble.h>
#include "main.h"
#include "slide_layer.h"
  
Window *window;  

static TextLayer *text_layer_date;
char buffer_date[] = "SEP 31";

static TextLayer *text_layer_dow;
char buffer_dow[] = "SAT   ";

SlideLayer *slide_layer[4];


void tick_handler(struct tm *tick_time, TimeUnits units_changed) {
  
  strftime(buffer_date, sizeof("SEP 31"), "%b %d", tick_time);
  text_layer_set_text(text_layer_date, buffer_date);
  
  strftime(buffer_dow, sizeof("SAT"), "%a", tick_time);
  text_layer_set_text(text_layer_dow, buffer_dow);
  
    if (!clock_is_24h_style()) {
    
        if( tick_time->tm_hour > 11 ) {   // YG Jun-25-2014: 0..11 - am 12..23 - pm
            strcat(buffer_dow, " PM" );
            if( tick_time->tm_hour > 12 ) tick_time->tm_hour -= 12;
        } else {
            strcat(buffer_dow, " AM" );
            if( tick_time->tm_hour == 0 ) tick_time->tm_hour = 12;
        }        
 
    }; 
 
    slide_layer_animate_to(slide_layer[0], tick_time->tm_hour / 10);
    slide_layer_animate_to(slide_layer[1], tick_time->tm_hour % 10);
    slide_layer_animate_to(slide_layer[2], tick_time->tm_min / 10);
    slide_layer_animate_to(slide_layer[3], tick_time->tm_min % 10);
     
}    



static void window_load(Window *window) {
  Layer *window_layer = window_get_root_layer(window);
 
  for (int i=0; i<4; i++){
    slide_layer[i] = slide_layer_create(NUM_COORDS[i]);
    layer_add_child(window_layer, slide_layer_get_layer(slide_layer[i]));
  }
  
  text_layer_date = text_layer_create(GRect(0, -5, 144, 60));
  text_layer_set_background_color(text_layer_date, GColorDarkGray);
  text_layer_set_text_color(text_layer_date, GColorRajah);
  text_layer_set_text_alignment(text_layer_date, GTextAlignmentCenter);
  text_layer_set_font(text_layer_date, fonts_load_custom_font(resource_get_handle(RESOURCE_ID_DIGITAL_SEVEN_MONO_50)));
  layer_add_child(window_layer, text_layer_get_layer(text_layer_date));
  
  text_layer_dow = text_layer_create(GRect(0, 110, 144, 60));
  text_layer_set_background_color(text_layer_dow, GColorDarkGray);
  text_layer_set_text_color(text_layer_dow, GColorRajah);
  text_layer_set_text_alignment(text_layer_dow, GTextAlignmentCenter);
  text_layer_set_font(text_layer_dow, fonts_load_custom_font(resource_get_handle(RESOURCE_ID_DIGITAL_SEVEN_MONO_50)));
  layer_add_child(window_layer, text_layer_get_layer(text_layer_dow));
    
  
}

static void window_unload(Window *window) {
  
  for (int i=0; i<4; i++){
    slide_layer_destroy(slide_layer[i]);
  }
  
  text_layer_destroy(text_layer_date);
  text_layer_destroy(text_layer_dow);
  
}


static void init(void) {
  setlocale(LC_ALL, "");

  // creating window
  window = window_create();
  window_set_window_handlers(window, (WindowHandlers) {
    .load = window_load,
    .unload = window_unload,
  });
  
  // subscribing to timer
  tick_timer_service_subscribe(MINUTE_UNIT, (TickHandler) tick_handler);
  
  window_stack_push(window, true);

}  


static void deinit(void) {
  tick_timer_service_unsubscribe();
  window_destroy(window);
}
  
int main(void) {
  init();
  app_event_loop();
  deinit();
}