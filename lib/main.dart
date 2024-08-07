import 'package:flutter/material.dart';
import 'package:flutter/services.dart';

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

void main() async {
  WidgetsFlutterBinding.ensureInitialized();
  await checkMethodChennel();
  runApp(const MyApp());
}

class MyApp extends StatelessWidget {
  const MyApp({super.key});

  @override
  Widget build(BuildContext context) {
    return MaterialApp(
      title: 'Flutter Demo',
      debugShowCheckedModeBanner: false,
      theme: ThemeData(useMaterial3: true),
      darkTheme: ThemeData.dark(useMaterial3: true),
      home: Scaffold(
        body: Center(
          child: FilledButton(
            child: const Text('Blink'),
            onPressed: () async {
              await blinkAppIcon();
            },
          ),
        ),
      ),
    );
  }
}
