# doorbellOLED
This is a project that uses the ESP8266 to control a 128x64 OLED display via I2C remotely. 
Upload this repo to the ESP board using the Arduino IDE. Make sure to change the ssid and 
pass variables in the credentials.h to match your own. 

The IP to connect to will display on the serial monitor. The domain for the ESP will be 
printed on the OLED, but that will have to be changed to not doorbell.davidmcphee.ca.

How to add more buttons:

1. Text
Open webServerOLED and find the void setup() section.
Add this template at the bottom of the section.

server.on("/replaceThis", [](){
    drawText("Custom Text");
    server.send(200, "text/html", homePage);
  });

"/replaceThis" is the command sent to the server. 
"Custom Text" is the custom message.
homePage is the html page served after the request has been made. 
A new page can be linked but must be created as a new .h file and
the line: 
#include "newPage.h"
must be added.
Note for creating HTML pages they must start with:
const char newPage[] PROGMEM = R"=====(
and end with:
)=====";

2. Image
Go to davidmcphee.ca/image2oled.html and create your image.
Take the output code and add it to the bitmaps.h file. The bitmap 
must start with:
#define IMGx {\
where x is the image number. 
Next go to the webServerOLED page and add this template after the #define
rows:
const unsigned char imgx [] PROGMEM = IMGx;
and find the void setup() section.
Add this template to the bottom:

server.on("/imgx", [](){
    display.clearDisplay();
    display.drawBitmap(0, 0, imgx, 128, 64, 1);
    display.display();
    delay(2000);
    server.send(200, "text/html", homePage);
  });
A custom returned page can be added same as in 1.
