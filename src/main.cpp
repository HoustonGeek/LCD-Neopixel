//+------------------------------------------------------------------------------------
//
// Arduino LEDs
//
// File: main.cpp
//
// Description:
//
//  Draws effects on an addressable RGB strip and prints stats to the TFT display
//
// History:       Jan-09-2023     mstringer   add FASTLed code
//
//+-----------------------------------------------------------------------------------

#include <Arduino.h>
#include <Adafruit_NeoPixel.h>
#include <Adafruit_TestBed.h>
#include <Adafruit_ST7789.h> 
#include <Fonts/FreeSans9pt7b.h>
#define FASTLED_INTERNAL
#include <FastLED.h>

#define NUM_LEDS  30
#define LED_PIN   5

CRGB g_LEDs[NUM_LEDS] = {0};      //buffer for LastLED

//setup the Neopixel connection
extern Adafruit_TestBed TB;

//setup display from the ST889 library
Adafruit_ST7789 display = Adafruit_ST7789(TFT_CS, TFT_DC, TFT_RST);

//create a canvas for the display
GFXcanvas16 canvas(240, 135);

uint8_t j = 0;
// FramsPerSecond
//
// Tracks a weighted average for FPS
double FramesPerSecond(double seconds)
{
  static double framesPerSecond;

  framesPerSecond = (framesPerSecond * .9 + (1.0 / seconds * .1));
  return framesPerSecond;
}

// DrawScreen
//
// Updates the screen
void DrawScreen(int val)
{
  canvas.flush();
  canvas.fillScreen(ST77XX_BLACK);
  canvas.setCursor(0, 25);
  canvas.setTextColor(ST77XX_RED);
  canvas.println("Stringer's Feather TFT");
  canvas.setTextColor(ST77XX_YELLOW);
  canvas.println("Adafruit ESP32-S3TFT");
  canvas.setTextColor(ST77XX_GREEN); 
  canvas.print("Neopixel val: ");
  canvas.setTextColor(ST77XX_WHITE);
  canvas.printf("%d\n",j);
  canvas.setTextColor(ST77XX_BLUE); 
  canvas.print("FPS: ");
  canvas.setTextColor(ST77XX_WHITE);
  canvas.printf("%03d",val);
  canvas.print(" LEDs: P");
  canvas.printf("%02d",LED_PIN);
  canvas.print(" #:");
  canvas.printf("%02d",NUM_LEDS);

  display.drawRGBBitmap(0, 0, canvas.getBuffer(), 240, 135);
  TB.setColor(TB.Wheel(j++));
  delay(50);
  }

void setup() 
{
  //Serial.begin(115200);
  //Serial.println("Hello, world!");
  //power the external 
  //pinMode(LED_PIN, OUTPUT);

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
  
  FastLED.addLeds<WS2812B, LED_PIN, GRB>(g_LEDs, NUM_LEDS);   //Add LED sctrip
}


void loop() 
{
  
  double fps = 0;

  uint8_t initialHue = 0;
  const uint8_t deltaHue = 16;
  const uint8_t hueDensity = 4;

  for (;;)
  {
    //Handle FPS
    double dStart = millis() / 1000.0;

    //Draw screen
    DrawScreen(fps);

    //LED code from here
    fill_rainbow(g_LEDs, NUM_LEDS, initialHue += hueDensity, deltaHue);
    FastLED.show();


    double dEnd = millis() / 1000.0;
    fps = FramesPerSecond(dEnd - dStart);

  }
}