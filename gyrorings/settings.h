#ifndef __SETTINGS__
#define __SETTINGS__

#define BRIGHTNESS 32
#define LED_PIN 22
#define COLOR_ORDER GRB
#define CHIPSET WS2812B

#ifndef LED_BUILTIN
#define LED_BUILTIN 2
#endif

#define UPDATES_PER_SECOND 50

// Params for width and height
const uint8_t kMatrixWidth = 2;
const uint8_t kMatrixHeight = 12;

#define SDA_PIN 18
#define SCL_PIN 19

#endif
