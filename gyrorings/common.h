#ifndef __COMMON__
#define __COMMON__

#include "settings.h"
#include <WiFi.h>
#include <WiFiMulti.h>
#include <AsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include <ESPmDNS.h>

#define MAX_COMMANDS 5
enum action_t {screenBrightness, modeDisco, modeWhite, nextSensitivity, gyroSensitivity};
typedef struct {
  action_t action;
/*  uint8_t x;
  uint8_t y;
  uint8_t r;
  uint8_t g; */
  uint8_t b;
} command_t ;

QueueHandle_t commandQ;
/* QueueHandle_t analysisQ;*/

#define NUM_LEDS (kMatrixWidth * kMatrixHeight)

/* typedef struct {
  uint8_t tempo;
  uint16_t barCount = 0;
  uint16_t beatCount = 0;
  uint16_t tatumCount = 0;
  uint16_t bars[MAX_BARS];
  uint16_t beats[MAX_BEATS];
  uint16_t tatums[MAX_TATUMS];
} analysis_t;

analysis_t analIn;
analysis_t analOut; */

#endif
