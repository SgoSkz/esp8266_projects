/*
 * esp8266 simple WebSocket server
 * https://www.mischainti.org
 *
 * The server response with the
 * echo of the message you send
 * and send a broadcast every 5secs
 *
 */
 
#include <Arduino.h>
 
#include <ESP8266WiFi.h>
#include <WebSocketsServer.h>
 
const char *ssid     = "QoQing";
const char *password = "JiAvSjLn";
const uint8_t wsPort = 81;
unsigned long last = millis();
 
unsigned long messageInterval = 5000;
bool connected = false;
 
#define DEBUG_SERIAL Serial
 
WebSocketsServer webSocket = WebSocketsServer(wsPort);
 
void webSocketEvent(uint8_t num, WStype_t type, uint8_t * payload, size_t length) {
 
    switch(type) {
        case WStype_DISCONNECTED:
            DEBUG_SERIAL.printf("[%u] Disconnected!\n", num);
            break;
        case WStype_CONNECTED:
            {
                IPAddress ip = webSocket.remoteIP(num);
                DEBUG_SERIAL.printf("[%u] Connected from %d.%d.%d.%d url: %s\n", num, ip[0], ip[1], ip[2], ip[3], payload);
                 
                // send message to client
        webSocket.sendTXT(num, "Connected");
            }
            break;
        case WStype_TEXT:
        {
            DEBUG_SERIAL.printf("[%u] RECEIVE TXT: %s\n", num, payload);
            char buffer[255];
            snprintf_P(buffer, 255, "%s%s", "(ECHO MESSAGE) ", String((char*)payload));
            // send message to client
            webSocket.sendTXT(num, buffer);
  
            // send data to all connected clients
            // webSocket.broadcastTXT("message here");
        }
            break;
        case WStype_BIN:
            DEBUG_SERIAL.printf("[%u] get binary length: %u\n", num, length);
            hexdump(payload, length);
 
            // send message to client
            // webSocket.sendBIN(num, payload, length);
            break;
    }
}
 
void setup() {
    DEBUG_SERIAL.begin(115200);
    wifi_set_sleep_type(NONE_SLEEP_T);
//  DEBUG_SERIAL.setDebugOutput(true);
 
    DEBUG_SERIAL.println();
    DEBUG_SERIAL.println();
    DEBUG_SERIAL.println();
 
    for(uint8_t t = 4; t > 0; t--) {
        DEBUG_SERIAL.printf("[SETUP] BOOT WAIT %d...\n", t);
        DEBUG_SERIAL.flush();
        delay(1000);
    }
 
    WiFi.softAP(ssid, password);
 
    // while ( WiFi.status() != WL_CONNECTED ) {
    //   delay ( 500 );
    //   Serial.print ( "." );
    // }
 
    DEBUG_SERIAL.println("WebSocket complete uri is: ");
    DEBUG_SERIAL.print("ws://");
    DEBUG_SERIAL.print(WiFi.softAPIP());
    DEBUG_SERIAL.print(":");
    DEBUG_SERIAL.print(wsPort);
    DEBUG_SERIAL.println("/");
 
    webSocket.begin();
    webSocket.onEvent(webSocketEvent);
}
 
unsigned long lastUpdate = millis();
 
void loop() {
    webSocket.loop();
    // if (lastUpdate+messageInterval<millis()){
    //     DEBUG_SERIAL.println("[WSc] SENT: Simple broadcast client message!!");
    //     webSocket.broadcastTXT("Simple broadcast client message!!");
    //     lastUpdate = millis();
    // }
}