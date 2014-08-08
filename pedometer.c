//Created By A 14 Year Old Named Andrew Jazbec
//To use this template for anything else than a pedometer just change ACCEL_AXIS_X on line 49


//INTRUCTIONS
//Hold Select To clear the number of steps

#include <pebble.h>

#define NUM_STEPS_PKEY 1
#define NUM_STEPS_DEFAULT 0

Window *window;
TextLayer *text_layer;

static int steps = NUM_STEPS_DEFAULT;

static void update_text1() {
  static char body_text[50];
  snprintf(body_text, sizeof(body_text), "%u", steps);
  text_layer_set_text(text_layer, body_text);
}

//Long Click Press
void select_long_click_handler(ClickRecognizerRef recognizer, void *context) {
  steps = 0;
  update_text1();
}

//Long Click Rlease
void select_long_click_release_handler(ClickRecognizerRef recognizer, void *context) {
  //No contents needed
}

//Click Config
static void click_config_provider(void *context) {
  const uint16_t repeat_interval_ms = 50;
  window_long_click_subscribe(BUTTON_ID_SELECT, 1000, select_long_click_handler, select_long_click_release_handler);
}

void shake(void)
{
  steps=steps+1;
  update_text1();
}

void accel_tap_handler(AccelAxisType axis, int32_t direction) {
 
  if (axis == ACCEL_AXIS_X)
  {  
    shake();   
  }

}

static void window_load(Window *me) {
  Layer *layer = window_get_root_layer(me);
  
  text_layer = text_layer_create(GRect(0, 20, 144, 154));
	text_layer_set_font(text_layer, fonts_get_system_font(FONT_KEY_BITHAM_42_BOLD));
	text_layer_set_text_alignment(text_layer, GTextAlignmentCenter);
  layer_add_child(layer, text_layer_get_layer(text_layer));
  
  update_text1();
}

static void window_unload(Window *window) {
  text_layer_destroy(text_layer);
}

void handle_init(void) {
  
  window = window_create();
  window_set_click_config_provider(window, click_config_provider);
  window_set_window_handlers(window, (WindowHandlers) {
    .load = window_load,
    .unload = window_unload,
  });
  
  steps = persist_exists(NUM_STEPS_PKEY) ? persist_read_int(NUM_STEPS_PKEY) : NUM_STEPS_DEFAULT;
  
  window_stack_push(window, true /* Animated */);

	// App Logging!
	APP_LOG(APP_LOG_LEVEL_DEBUG, "Just pushed a window!");
  
  accel_tap_service_subscribe(accel_tap_handler);
}

void handle_deinit(void) {
  persist_write_int(NUM_STEPS_PKEY, steps);

	// Destroy the window
	window_destroy(window);
}

int main(void) {
	handle_init();
	app_event_loop();
	handle_deinit();
}
