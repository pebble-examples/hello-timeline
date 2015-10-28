#include <pebble.h>

static Window *s_window;
static TextLayer *s_text_layer;

static char s_text_buffer[64];

enum {
  AppKeyReady,
  AppKeyText,
  AppKeyMinutes,
  AppKeyQuit,
} AppKey;

static void request_pin(uint32_t minutes) {
  DictionaryIterator *iter;
  app_message_outbox_begin(&iter);
  dict_write_uint32(iter, AppKeyMinutes, minutes);
  dict_write_end(iter);
  app_message_outbox_send();
}

static void check_launch_args() {
  uint32_t launch_arg = launch_get_args();

  // make launch_arg 5 so we send a pin that's in future
  if (launch_arg == 0) {
    launch_arg = 5;
  }

  // request a pin in `launch_arg` minutes
  request_pin(launch_arg);

  snprintf(s_text_buffer, sizeof(s_text_buffer), "Requesting a pin to be sent in %d minutes...", (int)launch_arg);
  text_layer_set_text(s_text_layer, s_text_buffer);
}

static void in_received_handler(DictionaryIterator *iter, void *context) {
  Tuple *tuple = dict_find(iter, AppKeyReady);
  if (tuple) {
    check_launch_args();
  }

  tuple = dict_find(iter, AppKeyText);
  if (tuple) {
    snprintf(s_text_buffer, sizeof(s_text_buffer), "%s", tuple->value->cstring);
    text_layer_set_text(s_text_layer, s_text_buffer);
  }

  tuple = dict_find(iter, AppKeyQuit);
  if (tuple) {
    window_stack_pop_all(false);
  }
}

static void window_load(Window *window) {
  Layer *window_layer = window_get_root_layer(window);
  GRect bounds = layer_get_bounds(window_layer);

  s_text_layer = text_layer_create(bounds);
  text_layer_set_text(s_text_layer, "Requesting a pin to be sent to your timeline...");
  text_layer_set_text_alignment(s_text_layer, GTextAlignmentCenter);
  text_layer_set_background_color(s_text_layer, GColorClear);
  text_layer_set_text_color(s_text_layer, GColorCeleste);
  text_layer_set_font(s_text_layer, fonts_get_system_font(FONT_KEY_GOTHIC_24_BOLD));
  layer_add_child(window_layer, text_layer_get_layer(s_text_layer));

#if defined(PBL_ROUND)
  text_layer_enable_screen_text_flow_and_paging(s_text_layer, 3);
#endif
}

static void window_unload(Window *window) {
  text_layer_destroy(s_text_layer);
}

static void init(void) {
  s_window = window_create();
  window_set_background_color(s_window, GColorCobaltBlue);
  window_set_window_handlers(s_window, (WindowHandlers) {
    .load = window_load,
    .unload = window_unload,
  });
  window_stack_push(s_window, false);

  app_message_register_inbox_received(in_received_handler);
  app_message_open(256, 256);
}

static void deinit(void) {
  window_destroy(s_window);
}

int main(void) {
  init();
  app_event_loop();
  deinit();
}
