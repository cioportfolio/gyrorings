#include "common.h"

/*
  #define SW_PIN 2
  #define X_PIN 0
  #define Y_PIN 1
  #define JOY_SENS 25
  #define JOY_LO (512 - JOY_SENS)
  #define JOY_HI (512 + JOY_SENS)
*/

void loop()
{
  vTaskDelay(10);
}

void setup()
{
  static uint8_t params;
  TaskHandle_t webHandle = NULL;
  TaskHandle_t displayHandle = NULL;

  Serial.begin(115200);
  pinMode(LED_BUILTIN, OUTPUT);

//  pinMode(SW_PIN, INPUT_PULLUP);

  commandQ = xQueueCreate(MAX_COMMANDS, sizeof(command_t));
  if (commandQ == NULL)
  {
    Serial.println("Could not create command queue");
  }

  xTaskCreatePinnedToCore(displayTask, "DISPLAY", 10240, &params, 10, &displayHandle, 1);
  xTaskCreatePinnedToCore(webTask, "WEB", 10240, &params, 5, &webHandle, 0);
}
