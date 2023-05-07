Use the ESP32 sketch data uploader to load the index.html to LittleFS on the ESP32. See https://github.com/lorol/arduino-esp32fs-plugin

Copy secrets.h.template to secrets.h and add your wifi SSID and passwords

Update settings.h as required e.g. led pins, matrix dimensions

When running the ESP32 will attempt to create a wifi access point and serve a simple control page (e.g. plain white/disco switch)
