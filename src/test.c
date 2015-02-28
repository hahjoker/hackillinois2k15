/*
 * main.c
 * Creates a Window, TextLayer to display output and increments the 
 * persisted value on every launch.
 */

#include <pebble.h>

#define PERSIST_KEY_LAUNCHES 10
#define PERSIST_KEY_TICK 0
static Window *s_main_window;
static TextLayer *s_output_layer;
static int s_uptime =0;
static bool up =true;
static char s_uptime_buffer[32];
static void tick_handler(struct tm *tick_time, TimeUnits units_changed){
  



  if(s_uptime == 10)
    {
    up=false;
  }
  if(s_uptime == 0){
    up=true;
  }

  if (persist_exists(PERSIST_KEY_TICK)){
    s_uptime=persist_read_int(PERSIST_KEY_TICK);
  }
  snprintf(s_uptime_buffer,sizeof(s_uptime_buffer),"Uptime: %ds  %d",s_uptime, (int)persist_read_int(PERSIST_KEY_TICK));
  text_layer_set_text(s_output_layer,s_uptime_buffer);
  text_layer_set_text_alignment(s_output_layer, GTextAlignmentRight);
  if(up==true)
  {
    s_uptime++;
  }
  else{
    s_uptime--;
  } 
  persist_write_int(PERSIST_KEY_TICK,s_uptime);
  
    
}
static void update_launch_count() {
  // Count number of launches
  int num_launches = 0;
  // Check to see if a count already exists
  if (persist_exists(PERSIST_KEY_LAUNCHES)) {
    // Load stored count
    num_launches = persist_read_int(PERSIST_KEY_LAUNCHES);
  }

  // Display to user
  static char s_launches_buffer[16];
  snprintf(s_launches_buffer, sizeof(s_launches_buffer), "Launches: %d %d", num_launches,(int)persist_read_int(PERSIST_KEY_LAUNCHES));
  text_layer_set_text(s_output_layer, s_launches_buffer);

  // Increment launch count and store
  num_launches++;
  persist_write_int(PERSIST_KEY_LAUNCHES, num_launches);
}

static void main_window_load(Window *window) {
  Layer *window_layer = window_get_root_layer(window);
  GRect window_bounds = layer_get_bounds(window_layer);

  // Create output TextLayer
  s_output_layer = text_layer_create(GRect(0, 0, window_bounds.size.w, window_bounds.size.h));
  text_layer_set_text_alignment(s_output_layer, GTextAlignmentCenter);
  layer_add_child(window_layer, text_layer_get_layer(s_output_layer));

  //update_launch_count();
 tick_timer_service_subscribe(SECOND_UNIT, tick_handler);  
}

static void main_window_unload(Window *window) {
  // Destroy output TextLayer
  text_layer_destroy(s_output_layer);
}

static void init(void) {
  // Create main Window
  s_main_window = window_create();
  window_set_window_handlers(s_main_window, (WindowHandlers) {
    .load = main_window_load,
    .unload = main_window_unload,
  });
  window_stack_push(s_main_window, true);
}

static void deinit(void) {
  // Destroy main Window
  window_destroy(s_main_window);
}

int main(void) {
  init();
  app_event_loop();
  deinit();
}