
#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include "credentials.h"
#include "index.h"
#include "bitmaps.h"

#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels
#define OLED_RESET     16 // Reset pin # (or -1 if sharing Arduino reset pin)
#define SCREEN_ADDRESS 0x3D ///< See datasheet for Address; 0x3D for 128x64, 0x3C for 128x32
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

const char* ssid     = USER;
const char* password = PASS;

const unsigned char img1 [] PROGMEM = IMG1;
const unsigned char img2 [] PROGMEM = IMG2;
ESP8266WebServer server(80);

void handleRoot();
void setup() {
  Serial.begin(115200);

  // SSD1306_SWITCHCAPVCC = generate display voltage from 3.3V internally
  if(!display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS)) {
    Serial.println(F("SSD1306 allocation failed"));
    for(;;); // Don't proceed, loop forever
  }
  display.clearDisplay();
  display.display();
  delay(2000); // Pause for 2 seconds

  Serial.print("Connecting to ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  // Print local IP address and start web server
  Serial.println("");
  Serial.println("WiFi connected.");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
  server.begin();
  drawText("doorbell.\ndavidmcphee.ca");

  server.on("/", handleRoot);

  server.on("/msg", [](){ 
    if(!server.hasArg("Message") || server.hasArg("Message") == NULL){
      server.send(200, "text/html", homePage);
    }else{
      
      drawText(String(server.arg("Message"))); 
      server.send(200, "text/html", homePage);
    }
  });

  server.on("/sleep", [](){
    drawText("Anne is   Sleeping!"); 
    server.send(200, "text/html", homePage);
  });

  server.on("/awake", [](){
    drawText("Anne is   Awake!");
    server.send(200, "text/html", homePage);
  });

  server.on("/img1", [](){
    display.clearDisplay();
    display.drawBitmap(0, 0, img1, 128, 64, 1);
    display.display();
    delay(2000);
    server.send(200, "text/html", homePage);
  });

  server.on("/img2", [](){
    display.clearDisplay();
    display.drawBitmap(0, 0, img2, 128, 64, 1);
    display.display();
    delay(2000);
    server.send(200, "text/html", homePage);
  });

  
}
void loop() {
  server.handleClient();
}

void drawText(String str) {
  display.clearDisplay();

  display.setTextSize(2);      // Normal 1:1 pixel scale
  display.setTextColor(SSD1306_WHITE); // Draw white text
  display.setCursor(0, 0);     // Start at top-left corner
  display.println(str);         // Use full 256 char 'Code Page 437' font

  display.display();
  delay(2000);
}


void handleRoot() {
  server.send(200, "text/html", homePage);
} 
