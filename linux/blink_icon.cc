#include <flutter_linux/flutter_linux.h>
#include <gtk/gtk.h>
#include <iostream>
#include <thread>
#include <chrono>

GtkStatusIcon *tray_icon;
GtkStatusIcon *tray_icon_blink;
bool blinking = false;

void change_icon(const gchar *icon_name) {
  gtk_status_icon_set_from_icon_name(tray_icon, icon_name);
}

void blink_app_icon(gint interval) {
  while (blinking) {
    gtk_status_icon_set_visible(tray_icon, TRUE);
    gtk_status_icon_set_visible(tray_icon_blink, FALSE);
    std::this_thread::sleep_for(std::chrono::seconds(interval));
    gtk_status_icon_set_visible(tray_icon, FALSE);
    gtk_status_icon_set_visible(tray_icon_blink, TRUE);
    std::this_thread::sleep_for(std::chrono::seconds(interval));
  }
}

void start_blinking(gint interval) {
  blinking = true;
  std::thread(blink_app_icon, interval).detach();
}

void stop_blinking() {
  blinking = false;
  gtk_status_icon_set_visible(tray_icon, TRUE);
  gtk_status_icon_set_visible(tray_icon_blink, FALSE);
}

static void handle_method_call(
    FlMethodChannel* channel,
    FlMethodCall* method_call,
    gpointer user_data) {
  FlMethodResponse* response = nullptr;
  const gchar* method = fl_method_call_get_name(method_call);
  if (strcmp(method, "startBlinking") == 0) {
    gint interval = fl_value_get_int(fl_method_call_get_args(method_call));
    start_blinking(interval);
    response = FL_METHOD_RESPONSE(fl_method_success_response_new(nullptr));
  } else if (strcmp(method, "stopBlinking") == 0) {
    stop_blinking();
    response = FL_METHOD_RESPONSE(fl_method_success_response_new(nullptr));
  } else {
    response = FL_METHOD_RESPONSE(fl_method_not_implemented_response_new());
  }

  fl_method_channel_respond(channel, method_call, response, nullptr);
}

void plugin_register_with_registrar(FlPluginRegistrar* registrar) {
  FlMethodChannel* channel = fl_method_channel_new(
      fl_plugin_registrar_get_messenger(registrar),
      "blink_icon",
      FL_METHOD_CODEC(fl_standard_method_codec_new()));
  fl_method_channel_set_method_call_handler(channel, handle_method_call, nullptr, nullptr);
  tray_icon = gtk_status_icon_new_from_icon_name("my-app-icon");
  tray_icon_blink = gtk_status_icon_new_from_icon_name("my-app-icon-blink");
}

extern "C" void blink_icon_plugin_register_with_registrar(FlPluginRegistrar* registrar) {
  plugin_register_with_registrar(registrar);
}
