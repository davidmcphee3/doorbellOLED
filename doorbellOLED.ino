//open source includes
#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>

//other file includes
#include "credentials.h"
#include "index.h"
#include "bitmaps.h"

//OLED intialization
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64 
#define OLED_RESET     16 
#define SCREEN_ADDRESS 0x3D 
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);
ESP8266WebServer server(80);

//login infor from credentials.h
const char* ssid     = USER;
const char* password = PASS;

//bitmaps from bitmaps.h
const unsigned char img1 [] PROGMEM = IMG1;
const unsigned char img2 [] PROGMEM = IMG2;

//for scroll text
String msg2;
int x, minX;
int y = display.width();
bool isText = true;

void setup() {
  Serial.begin(9600);

  //confirm OLED is configured properly 
  if(!display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS)) {
    Serial.println(F("SSD1306 allocation failed"));
    for(;;);
  }

  //initialize display
  display.setTextWrap(false);
  display.clearDisplay();
  display.display();
  delay(2000);

  //connect to wifi, monitor via serial
  Serial.print("Connecting to ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected.");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());

  //start server
  server.begin();

  //Client request handlers
  server.on("/", handleRoot);

  //Message handlers
  server.on("/msg", [](){
    configText(server.arg("Message"));
  });

  server.on("/sleep", [](){
    configText("Baby Anne is Sleeping!");
  });

  server.on("/awake", [](){
    configText("Baby Anne is Awake!");
  });

  //Quick Image handlers
  server.on("/img1", [](){
    configImg(img1);
  });

  server.on("/img2", [](){
    configImg(img2);
  });
  
}

void loop() {
  //handle incoming client requests
  server.handleClient();
  //check if text or image to be displayed
  if(isText){
    display.clearDisplay();
    display.setCursor(0,0);
    display.setTextColor(SSD1306_WHITE); // Draw white text
    display.setTextSize(1);
    display.print("  glad you're here"); // Header text
    display.setTextSize(3);
    display.setCursor(y,30);
    display.print(msg2); // Scrolling text
    display.display();
    y=y-1; // scroll speed
    if(y < minX) y= display.width();
  }
}

void handleRoot() {
  server.send(200, "text/html", homePage);
}

void configText(String msg) {
  msg2 = msg;
  minX = -18 * msg.length();
  isText = true;
  server.send(200, "text/html", homePage);
}

void configImg(const unsigned char* img){
  display.clearDisplay();
  display.drawBitmap(0, 0, img, 128, 64, 1);
  display.display();
  isText = false;
  server.send(200, "text/html", homePage);
}
