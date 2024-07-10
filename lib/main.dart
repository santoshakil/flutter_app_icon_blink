import 'package:flutter/material.dart';
import 'package:flutter/services.dart';

const _channel = MethodChannel('com.example.flutter_app_icon_blink/method_channel');

void main() async {
  WidgetsFlutterBinding.ensureInitialized();
  try {
    final version = await _channel.invokeMethod('getPlatformVersion');
    debugPrint('Platform version: $version');
  } catch (e) {
    debugPrint('Error: $e');
  }
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
      home: const Scaffold(
        body: Center(
          child: Text('...'),
        ),
      ),
    );
  }
}
