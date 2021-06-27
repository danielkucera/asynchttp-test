#include <Arduino.h>
#ifdef ESP32
#include <WiFi.h>
#include <AsyncTCP.h>
#elif defined(ESP8266)
#include <ESP8266WiFi.h>
#include <ESPAsyncTCP.h>
#endif
#include <ESPAsyncWebServer.h>
#include <WiFiManager.h> // https://github.com/tzapu/WiFiManager

AsyncWebServer server(80);

void notFound(AsyncWebServerRequest *request) {
    request->send(404, "text/plain", "Not found");
}

void setup() {

    Serial.begin(115200);
    WiFi.mode(WIFI_STA);

    WiFiManager wm;

    int res = wm.autoConnect("AutoConnectAP","password"); // password protected ap

    if(!res) {
        Serial.println("Failed to connect");
        // ESP.restart();
    } 
    else {
        //if you get here you have connected to the WiFi    
        Serial.println("connected...yeey :)");
    }


    Serial.print("IP Address: ");
    Serial.println(WiFi.localIP());

    server.on("/download_flash", HTTP_GET, [](AsyncWebServerRequest *request)
          {

            AsyncWebServerResponse *response = request->beginResponse("application/octet-stream", 10*1024*1024, [](uint8_t *buffer, size_t maxLen, size_t index) -> size_t
              {
                delay(300);

                Serial.printf("handle download: index:0x%X maxLen:%d\n", index, maxLen);
                return maxLen;
              });

            response->addHeader("Content-Disposition", "attachment; filename=\"flash.bin\"");
            request->send(response);
          });


    server.onNotFound(notFound);

    server.begin();
}

void loop() {
}