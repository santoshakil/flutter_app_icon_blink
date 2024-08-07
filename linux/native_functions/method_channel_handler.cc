// method_channel_handler.cc

#include "method_channel_handler.h"
#include <flutter_linux/flutter_linux.h>
#include <gtk/gtk.h>
#ifdef GDK_WINDOWING_X11
#include <gdk/gdkx.h>
#include <X11/Xlib.h>
#include <X11/Xutil.h>
#endif
#include <iostream>

static gboolean blink_step(gpointer data)
{
    static int step = 0;
    GtkWindow *window = GTK_WINDOW(data);
    GdkDisplay *display = gdk_display_get_default();
    GdkWindow *gdk_window = gtk_widget_get_window(GTK_WIDGET(window));

    const char *display_name = gdk_display_get_name(display);
    std::cout << "Display name: " << display_name << std::endl;
    if (g_strcmp0(display_name, "wayland") == 0)
    {
        std::cout << "Running under Wayland" << std::endl;
        return FALSE; // Or handle Wayland-specific logic here
    }
    else if (GDK_IS_X11_WINDOW(gdk_window))
    {
        std::cout << "Running under x11" << std::endl;
        Window x11_window = gdk_x11_window_get_xid(gdk_window);
        std::cout << "---------" << std::endl; // Debug print

        XWMHints *hints = XAllocWMHints();
        if (step % 2 == 0)
        {
            hints->flags = XUrgencyHint;
        }
        else
        {
            hints->flags = 0;
        }
        XSetWMHints(GDK_DISPLAY_XDISPLAY(display), x11_window, hints);
        XFree(hints);
        XFlush(GDK_DISPLAY_XDISPLAY(display));
    }
    else
    {
        std::cerr << "Not an X11 window" << std::endl;
        return FALSE;
    }

    step++;
    if (step >= 6)
    {                 // 3 cycles of on/off
        return FALSE; // Stop the timeout
    }
    return TRUE; // Continue the timeout
}

static void blink_app_icon(GtkWindow *window)
{
#ifdef GDK_WINDOWING_X11
    g_timeout_add(1000, blink_step, window); // Call blink_step every 1000 milliseconds
#endif
}

static void method_call_handler(FlMethodChannel *channel, FlMethodCall *method_call, gpointer user_data)
{
    g_autoptr(FlMethodResponse) response = nullptr;
    const gchar *method = fl_method_call_get_name(method_call);

    if (strcmp(method, "getPlatformVersion") == 0)
    {
        g_autoptr(FlValue) result = fl_value_new_string("Linux");
        response = FL_METHOD_RESPONSE(fl_method_success_response_new(result));
    }
    else if (strcmp(method, "blinkAppIcon") == 0)
    {
        GtkWindow *window = GTK_WINDOW(user_data);
        blink_app_icon(window);
        response = FL_METHOD_RESPONSE(fl_method_success_response_new(nullptr));
    }
    else
    {
        response = FL_METHOD_RESPONSE(fl_method_not_implemented_response_new());
    }

    fl_method_call_respond(method_call, response, nullptr);
}

void initialize_method_channel(FlMethodChannel *channel, gpointer user_data)
{
    fl_method_channel_set_method_call_handler(channel, method_call_handler, g_object_ref(user_data), g_object_unref);
}
