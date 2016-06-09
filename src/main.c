#include <pebble.h>
#include "main.h"
#include <pebble-slide-layer/pebble-slide-layer.h>
  
  
Window *window;  

static TextLayer *text_layer_date;
char buffer_date[] = "SEP 31";

static TextLayer *text_layer_dow;
char buffer_dow[] = "SAT   ";

SlideLayer *slide_layer[4];

// changes slide animation direction of all 4 layers
static void change_animation_direction(int direction) {
    slide_layer_set_animation_direction(direction, slide_layer[0]);
    slide_layer_set_animation_direction(direction, slide_layer[1]);
    slide_layer_set_animation_direction(direction, slide_layer[2]);
    slide_layer_set_animation_direction(direction, slide_layer[3]);
}

// handle configuration change
static void in_recv_handler(DictionaryIterator *iterator, void *context) {
  Tuple *t = dict_read_first(iterator);
  
  while (t)  {
    
    if (t->key == MESSAGE_KEY_DIRECTION)    {
      
        // correction for previous definition in HTML config
        int direction;
      
        switch (t->value->int8) {
          case SLIDE_TO_TOP_LEFT: direction = SLIDE_TO_BOTTOM_LEFT; break;
          case SLIDE_TO_BOTTOM_LEFT: direction = SLIDE_TO_TOP_LEFT; break;
          case SLIDE_TO_TOP: direction = SLIDE_TO_BOTTOM; break;
          case SLIDE_TO_BOTTOM: direction = SLIDE_TO_TOP; break;
          case SLIDE_TO_TOP_RIGHT: direction = SLIDE_TO_BOTTOM_RIGHT; break;
          case SLIDE_TO_BOTTOM_RIGHT: direction = SLIDE_TO_TOP_RIGHT; break;
          default: direction = t->value->int8; break;
        }
      
         persist_write_int(MESSAGE_KEY_DIRECTION, direction);
         change_animation_direction(direction);
   }    
    
    t = dict_read_next(iterator);
  
  }
}



// handle time change
void tick_handler(struct tm *tick_time, TimeUnits units_changed) {
  
  strftime(buffer_date, sizeof("SEP 31"), "%b %d", tick_time);
  text_layer_set_text(text_layer_date, buffer_date);
  
  strftime(buffer_dow, sizeof("SAT"), "%a", tick_time);
  text_layer_set_text(text_layer_dow, buffer_dow);
  
    if (!clock_is_24h_style()) { //test
    
        if( tick_time->tm_hour > 11 ) {   // YG Jun-25-2014: 0..11 - am 12..23 - pm
            strcat(buffer_dow, " PM" );
            if( tick_time->tm_hour > 12 ) tick_time->tm_hour -= 12;
        } else {
            strcat(buffer_dow, " AM" );
            if( tick_time->tm_hour == 0 ) tick_time->tm_hour = 12;
        }        
 
    }; 
 
    // only animating to next digit if the digit changed
    if (h1 != tick_time->tm_hour / 10) {
      h1 = tick_time->tm_hour / 10;
      slide_layer[0]->gbitmap = gbitmap_create_with_resource(NUM_NUM[h1]);
      slide_layer_animate(slide_layer[0]);
    }
  
    if (h2 != tick_time->tm_hour % 10) {
      h2 = tick_time->tm_hour % 10;
      slide_layer[1]->gbitmap = gbitmap_create_with_resource(NUM_NUM[h2]);
      slide_layer_animate(slide_layer[1]);
    }
  
    if (m1 != tick_time->tm_min / 10) {
      m1 = tick_time->tm_min / 10;
      slide_layer[2]->gbitmap = gbitmap_create_with_resource(NUM_NUM[m1]);
      slide_layer_animate(slide_layer[2]);
    }
  
    if (m2 != tick_time->tm_min % 10) {
      m2 = tick_time->tm_min % 10;
      slide_layer[3]->gbitmap = gbitmap_create_with_resource(NUM_NUM[m2]);
      slide_layer_animate(slide_layer[3]);
    } 
}    



static void window_load(Window *window) {
  Layer *window_layer = window_get_root_layer(window);
  GRect bounds = layer_get_bounds(window_layer);
 
  #ifdef PBL_RECT
    text_layer_date = text_layer_create(GRect(0, 4, bounds.size.w, 40));
  #else
    text_layer_date = text_layer_create(GRect(0, 21, bounds.size.w, 40));
  #endif
  
  
  #ifndef PBL_COLOR
    text_layer_set_background_color(text_layer_date, GColorBlack);
  #else
    text_layer_set_background_color(text_layer_date, GColorDarkGray);
  #endif
  text_layer_set_text_color(text_layer_date, GColorWhite);
  text_layer_set_text_alignment(text_layer_date, GTextAlignmentCenter);
  text_layer_set_font(text_layer_date, fonts_load_custom_font(resource_get_handle(RESOURCE_ID_CELLA_30)));
  layer_add_child(window_layer, text_layer_get_layer(text_layer_date));
  
  text_layer_dow = text_layer_create(GRect(0, 120, bounds.size.w, 40));
  #ifndef PBL_COLOR
    text_layer_set_background_color(text_layer_dow, GColorBlack);
  #else
    text_layer_set_background_color(text_layer_dow, GColorDarkGray);
  #endif
  text_layer_set_text_color(text_layer_dow, GColorWhite);
  text_layer_set_text_alignment(text_layer_dow, GTextAlignmentCenter);
  text_layer_set_font(text_layer_dow, fonts_load_custom_font(resource_get_handle(RESOURCE_ID_CELLA_30)));
  layer_add_child(window_layer, text_layer_get_layer(text_layer_dow));
  
  for (int i=0; i<4; i++){
    slide_layer[i] = slide_layer_create(NUM_COORDS[i]);
    layer_add_child(window_layer, slide_layer_get_layer(slide_layer[i]));
  }
    
  // setting animation direction remembered in persistant storage
  change_animation_direction(persist_read_int(MESSAGE_KEY_DIRECTION));
  
}

static void window_unload(Window *window) {
  
  for (int i=0; i<4; i++){
    slide_layer_destroy(slide_layer[i]);
  }
  
  text_layer_destroy(text_layer_date);
  text_layer_destroy(text_layer_dow);
  
}


static void init(void) {
  
  char *sys_locale = setlocale(LC_ALL, "");
  // we're not supporting chinese yet
  if (strcmp("zh_CN", sys_locale) || strcmp("zh_TW", sys_locale) == 0) {
    setlocale(LC_ALL, "en_US");
  }

  // creating window
  window = window_create();
  #ifndef PBL_COLOR
    window_set_background_color(window, GColorBlack);
  #else
    window_set_background_color(window, GColorDarkGray);
  #endif
  
  window_set_window_handlers(window, (WindowHandlers) {
    .load = window_load,
    .unload = window_unload,
  });
  
  // subscribing to JS messages
  app_message_register_inbox_received((AppMessageInboxReceived) in_recv_handler);
  app_message_open(APP_MESSAGE_INBOX_SIZE_MINIMUM, APP_MESSAGE_OUTBOX_SIZE_MINIMUM);

  
  // subscribing to timer
  tick_timer_service_subscribe(MINUTE_UNIT, (TickHandler) tick_handler);
  
  window_stack_push(window, true);

}  


static void deinit(void) {
  app_message_deregister_callbacks();
  tick_timer_service_unsubscribe();
  window_destroy(window);
}
  
int main(void) {
  init();
  app_event_loop();
  deinit();
}