#include <Arduino.h>
#include <Adafruit_NeoPixel.h>
#include <Adafruit_TestBed.h>
#include <Adafruit_ST7789.h> 
#include <Fonts/FreeSans12pt7b.h>

//setup the Neopixel connection
extern Adafruit_TestBed TB;

//setup display from the ST889 library
Adafruit_ST7789 display = Adafruit_ST7789(TFT_CS, TFT_DC, TFT_RST);

//create a canvas for the display
GFXcanvas16 canvas(240, 135);

void setup() 
{
  Serial.begin(115200);
  Serial.println("Hello, world!");

  pinMode(NEOPIXEL_POWER, OUTPUT);
  digitalWrite(NEOPIXEL_POWER, HIGH);
  TB.neopixelPin = PIN_NEOPIXEL;
  TB.neopixelNum = 1; 
  TB.begin();
  TB.setColor(WHITE);

  display.init(135, 240);           // Init ST7789 240x135
  display.setRotation(3);
  canvas.setFont(&FreeSans12pt7b);
  canvas.setTextColor(ST77XX_WHITE); 
  pinMode(TFT_BACKLITE, OUTPUT);
  digitalWrite(TFT_BACKLITE, HIGH);
}

uint8_t j = 0;

void loop() {
  if (j % 5 == 0) {
    canvas.fillScreen(ST77XX_BLACK);
    canvas.setCursor(0, 25);
    canvas.setTextColor(ST77XX_RED);
    canvas.println("Stringer's Feather TFT");
    canvas.setTextColor(ST77XX_YELLOW);
    canvas.println("ESP32-S3 TFT");
    canvas.setTextColor(ST77XX_GREEN); 
    canvas.print("Neopixel");
    canvas.setTextColor(ST77XX_WHITE);
    canvas.printf(" val %d\n",j);
    canvas.setTextColor(ST77XX_BLUE); 
    canvas.print("Power: ");
    canvas.setTextColor(ST77XX_WHITE);
    canvas.println("");

    display.drawRGBBitmap(0, 0, canvas.getBuffer(), 240, 135);
  }
  TB.setColor(TB.Wheel(j++));
  //Serial.println(j);
  delay(50);
  if (j > 255) {
    j=0;
  }
}