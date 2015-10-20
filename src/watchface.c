#include <pebble.h>

Window *window;
TextLayer *text_layer;

#define THE_HOUR 11

static void handle_tick(struct tm *tick_time, TimeUnits units_changed) {
	
	static char timeText[] = "Just\nMarried!"; // Needs to be static because it's used by the system later.

	time_t now;
	struct tm *tm;
	
	now = time(NULL);
	tm = localtime(&now);
	
	if(tm->tm_hour == THE_HOUR) {
		text_layer_set_font(text_layer, fonts_get_system_font(FONT_KEY_BITHAM_42_BOLD));
		snprintf(timeText, sizeof(timeText), " JA! ");
	} else if (tm->tm_hour > THE_HOUR) {
		text_layer_set_font(text_layer, fonts_get_system_font(FONT_KEY_BITHAM_30_BLACK));
		snprintf(timeText, sizeof(timeText), timeText);
	} else {
		text_layer_set_font(text_layer, fonts_get_system_font(FONT_KEY_BITHAM_42_BOLD));
		strftime(timeText, sizeof(timeText), "%H:%M", tm);		
	}

	text_layer_set_text(text_layer, timeText);
}

void handle_init(void) {
	
	window = window_create();
	text_layer = text_layer_create(GRect(0, 54, 144-0 /* width */, 168-0 /* height */));

	window_set_background_color(window, GColorBlack);
	
	text_layer_set_text(text_layer, "Hi!");
	text_layer_set_background_color(text_layer, GColorBlack);
	text_layer_set_text_color(text_layer, GColorWhite);
	text_layer_set_font(text_layer, fonts_get_system_font(FONT_KEY_BITHAM_42_BOLD));
	text_layer_set_text_alignment(text_layer, GTextAlignmentCenter);
	
	Layer *window_layer = window_get_root_layer(window);
	layer_add_child(window_layer, text_layer_get_layer(text_layer));

	window_stack_push(window, true);
	
	tick_timer_service_subscribe(MINUTE_UNIT, handle_tick);
	
}

void handle_deinit(void) {
	tick_timer_service_unsubscribe();
	text_layer_destroy(text_layer);
	window_destroy(window);
}

int main(void) {
	handle_init();
	app_event_loop();
	handle_deinit();
}
