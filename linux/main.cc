#include "my_application.h"
// ? Import the method channel handler header file -----------------------------
#include "native_functions/method_channel_handler.h"
// ? --------------------------------------------------------------------------

int main(int argc, char **argv)
{
  g_autoptr(MyApplication) app = my_application_new();
  return g_application_run(G_APPLICATION(app), argc, argv);
}
