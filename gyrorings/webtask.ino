#include "common.h"
#include "FS.h"
#include <LittleFS.h>
#include <HTTPClient.h>
#include "secrets.h"

void handleNotFound(AsyncWebServerRequest *request)
{
  if (request->method() == HTTP_OPTIONS)
  {
    request->send(200);
  }
  else
  {
    String message = "File Not Found\n\n";
    message += "URI: ";
    message += request->url();
    message += "\nMethod: ";
    message += (request->method() == HTTP_GET) ? "GET" : "POST";
    message += "\nArguments: ";
    message += request->args();
    message += "\n";

    for (uint8_t i = 0; i < request->args(); i++)
    {
      message += " " + request->argName(i) + ": " + request->arg(i) + "\n";
    }

    request->send(404, "text/plain", message);
  }
}

/* void handleTest(AsyncWebServerRequest *request)
  {
  if (request->method() == HTTP_OPTIONS)
  {
    request->send(200);
  }
  else
  {
    String message = "Test OK\n\n";
    message += "URI: ";
    message += request->url();
    message += "\nMethod: ";
    message += (request->method() == HTTP_GET) ? "GET" : "POST";
    message += "\nArguments: ";
    message += request->args();
    message += "\n";

    for (uint8_t i = 0; i < request->args(); i++)
    {
      message += " " + request->argName(i) + ": " + request->arg(i) + "\n";
    }

    request->send(200, "text/plain", message);
  }
  } */

bool loadFromLittleFS(AsyncWebServerRequest *request, String path)
{
  String dataType = "text/html";

  Serial.print("Requested page -> ");
  Serial.println(path);
  if (LittleFS.exists(path))
  {
    File dataFile = LittleFS.open(path, "r");
    if (!dataFile)
    {
      handleNotFound(request);
      return false;
    }

    AsyncWebServerResponse *response = request->beginResponse(LittleFS, path, dataType);
    Serial.print("Real file path: ");
    Serial.println(path);

    request->send(response);

    dataFile.close();
  }
  else
  {
    handleNotFound(request);
    return false;
  }
  return true;
}

void handleRoot(AsyncWebServerRequest *request)
{
  if (request->method() == HTTP_OPTIONS)
  {
    request->send(200);
  }
  else
  {
    loadFromLittleFS(request, "/index.html");
  }
}

void wsHandler(AsyncWebSocket *server, AsyncWebSocketClient *client, AwsEventType type, void *arg, uint8_t *data, size_t len)
{
  command_t c;
  switch (type)
  {
  case WS_EVT_CONNECT:
    Serial.println("Websocket client connection received");
    break;
  case WS_EVT_DISCONNECT:
    Serial.println("Client disconnected");
    break;
  case WS_EVT_DATA:
    Serial.print("Socket data: ");
    Serial.println((char)data[0]);
    switch ((char)data[0])
    {
    case 'B':
      c = {screenBrightness, data[1]};
      xQueueSend(commandQ, &c, portMAX_DELAY);
      break;
    case 'D':
      c = {modeDisco, 0};
      xQueueSend(commandQ, &c, portMAX_DELAY);
      break;
    case 'W':
      c = {modeWhite, 0};
      xQueueSend(commandQ, &c, portMAX_DELAY);
      break;
    case 'N':
      c = {nextSensitivity, data[1]};
      xQueueSend(commandQ, &c, portMAX_DELAY);
      break;
    case 'G':
      c = {gyroSensitivity, data[1]};
      xQueueSend(commandQ, &c, portMAX_DELAY);
      break;
    }
    break;
  }
}

WiFiMulti wifiMulti;
HTTPClient http;

void webTask(void *params)
{
  uint8_t data;
  command_t cmd;

  Serial.print("Web task on core ");
  Serial.println(xPortGetCoreID());

  // Set web server port number to 80
  AsyncWebServer server(80);
  AsyncWebSocket ws("/ws");

  // Connect to Wi-Fi network with SSID and password
  // Serial.print("Setting AP (Access Point)…");
  // Remove the password parameter, if you want the AP (Access Point) to be open
  WiFi.softAP(SECRETSSID, SECRETPASSWORD);

  IPAddress IP = WiFi.softAPIP();

  http.setReuse(true);

  // Activate mDNS this is used to be able to connect to the server
  // with local DNS hostmane esp8266.local
  // initialize mDNS service
  esp_err_t err = mdns_init();
  if (err)
  {
    printf("MDNS Init failed: %d\n", err);
    return;
  }

  // set hostname
  mdns_hostname_set("glasses");
  // set default instance
  mdns_instance_name_set("glasses");

  if (!LittleFS.begin())
  {
    Serial.println("LittleFS Mount Failed");
    return;
  }

  server.on("/", handleRoot);
  /* server.on("/test", handleTest);
    server.on("/tetris", handleTetris);
    server.on("/pixel", handlePixel);
    server.on("/screen", handleScreen); */
  server.onNotFound(handleNotFound);
  ws.onEvent(wsHandler);
  server.addHandler(&ws);

  // Alow cross-origin requests
  DefaultHeaders::Instance().addHeader("Access-Control-Allow-Origin", "*");
  server.begin();

  for (;;)
  {
    vTaskDelay(10);
  }
}
