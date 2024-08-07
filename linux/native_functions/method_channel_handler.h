// method_channel_handler.h

#ifndef METHOD_CHANNEL_HANDLER_H
#define METHOD_CHANNEL_HANDLER_H

#include <flutter_linux/flutter_linux.h>

#ifdef __cplusplus
extern "C"
{
#endif

    // Function to initialize the method channel
    void initialize_method_channel(FlMethodChannel *channel, gpointer user_data);

#ifdef __cplusplus
}
#endif

#endif // METHOD_CHANNEL_HANDLER_H
