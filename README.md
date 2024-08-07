# Blink the App Icon in Flutter Desktop for Linux machines using Method Channel'.'

## Getting Started

### Modify the `linux/main.cc` file to include the following line'.'

```cpp
// ? Import the method channel handler header file -----------------------------
#include "native_functions/method_channel_handler.h"
```

### Modify the `linux/CMakeLists.txt` file to include the following lines'.'

```txt
#// ? add this line to x11 -----------------------------------------------------
pkg_check_modules(X11 REQUIRED IMPORTED_TARGET x11) 
```

```txt
  #// ? add this line to import mmethod_channel_handler.cc file -------------------
  "native_functions/method_channel_handler.cc"   
```

```txt
#// ? merge this line to GTK & X11 ----------------------------------------------
target_link_libraries(${BINARY_NAME} PRIVATE PkgConfig::GTK PkgConfig::X11)
```

### Modify the `linux/my_application.cc` file to include the following lines'.'

```cpp
// Import these libraries ----------------------------------------------------------------
#include "flutter/generated_plugin_registrant.h" // import it if it's not already imported
#include <iostream>
#include <stdlib.h>
#include "native_functions/method_channel_handler.h"
#include <X11/Xlib.h>
#include <X11/Xutil.h>
```

Add these lines inside the `my_application_activate` function

```cpp
  // ? Initialize the method channel -------------------------------------------------------
  g_autoptr(FlPluginRegistrar) registrar = fl_plugin_registry_get_registrar_for_plugin(FL_PLUGIN_REGISTRY(view), "com.example.flutter_app_icon_blink");
  g_autoptr(FlStandardMethodCodec) codec = fl_standard_method_codec_new();
  g_autoptr(FlMethodChannel) channel = fl_method_channel_new(
      fl_plugin_registrar_get_messenger(registrar),
      "com.example.flutter_app_icon_blink/method_channel",
      FL_METHOD_CODEC(codec));

  initialize_method_channel(channel, window);
```

Add these lines inside the `my_application_startup` function

```cpp
  // ? Set the GDK_BACKEND environment variable to x11 -------------------------------------
  setenv("GDK_BACKEND", "x11", 1);
```

### Dart Codes'.'

```dart
const _channel = MethodChannel('com.example.flutter_app_icon_blink/method_channel');
Future<void> checkMethodChennel() async {
  try {
    final version = await _channel.invokeMethod('getPlatformVersion');
    debugPrint('Platform version: $version');
  } catch (e) {
    debugPrint('Error: $e');
  }
}

Future<void> blinkAppIcon() async {
  // ? 5 Sec delay for testing purpose
  // ? In this time you can unfocus the app and see the app icon blinking / notifcation
  await Future.delayed(const Duration(seconds: 5)); // ? ignore in production
  try {
    debugPrint('Blinking app icon');
    await _channel.invokeMethod('blinkAppIcon');
    debugPrint('Blinked app icon');
  } catch (e) {
    debugPrint('Error: $e');
  }
}
```

### Run the following command to build `.rpm` release build file'.'

```bash
flutter_distributor release --name=dev --jobs=release-dev-linux-rpm
```

### To understand the code of the `linux/native_functions/method_channel_handler.cc` file? Use any AI'.'
