// Set Arduino Board to "Adafruit ESP32 Feather V2" - all defaults...

#include <SPI.h>
#include <Wire.h>
#include "time.h"

#include "Adafruit_ThinkInk.h"
#include <U8g2_for_Adafruit_GFX.h>
#include <Adafruit_SCD30.h>


// these pins will be defined for us 
#if defined(ADAFRUIT_FEATHER_ESP32_V2)
#define PIN_NEOPIXEL 0
#define NEOPIXEL_I2C_POWER 2
#endif

#if defined(ESP8266)
#define EPD_CS     0
#define EPD_DC     15
#define SRAM_CS    16
#define EPD_RESET  -1  // can set to -1 and share with microcontroller Reset!
#define EPD_BUSY   -1  // can set to -1 to not use a pin (will wait a fixed delay)
#define VREF_PIN   35

#elif defined( ESP32 )
#define EPD_CS     15
#define EPD_DC     33
#define SRAM_CS    32
//#define EPD_RESET  17  
#define EPD_RESET  -1  // can set to -1 and share with microcontroller Reset!
#define EPD_BUSY   37  
#define VREF_PIN   35

#else

#define EPD_CS     9
#define EPD_DC     10
#define SRAM_CS    6
#define EPD_RESET  -1  // can set to -1 and share with microcontroller Reset!
#define EPD_BUSY   -1  // can set to -1 to not use a pin (will wait a fixed delay)
#define VREF_PIN   -1

#endif

#define TRANSISTOR_GATE_PIN 12

// take the average of a few readings by waiting for the sensor to take a few readings
#define TAKE_AVERAGE_CO2

#define c2f( a )      (((a) * 1.8000) + 32)
#define kTempOffsetC  5.72

#define BATTERY_VID_THRESHOLD 50   // percent
#define LOW_BATTERY_THRESHOLD 10   // percent

/* Uncomment the following line if you are using 1.54" tricolor EPD */
//Adafruit_IL0373 display(152, 152 ,EPD_DC, EPD_RESET, EPD_CS, SRAM_CS, EPD_BUSY);

/* Uncomment the following line if you are using 2.15" tricolor EPD */
//Adafruit_IL0373 display(212, 104 , EPD_DC, EPD_RESET, EPD_CS, SRAM_CS, EPD_BUSY);

/* Uncomment the following line if you are using 2.7" tricolor EPD */
//Adafruit_IL91874 display(264, 176 ,EPD_DC, EPD_RESET, EPD_CS, SRAM_CS);
//Adafruit_SSD1306 oled = Adafruit_SSD1306( 128, 32, &Wire );


ThinkInk_213_Mono_BN display(EPD_DC, EPD_RESET, EPD_CS, SRAM_CS, EPD_BUSY);


U8G2_FOR_ADAFRUIT_GFX u8g2Fonts;  // Select u8g2 font from here: https://github.com/olikraus/u8g2/wiki/fntlistall

// hack
#ifndef GxEPD_BLACK
#define GxEPD_BLACK EPD_BLACK
#endif 

// light mode
#define FORECOLOR EPD_BLACK
#define BACKCOLOR EPD_WHITE

// darkmode: white text with black background - this would be better is the case aperature didn't reveal any white (which creates a frame).
//#define FORECOLOR EPD_WHITE
//#define BACKCOLOR EPD_BLACK


/*static const uint8_t PROGMEM s_folabs_logo_inverted[] =
  {
  0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
  0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
  0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xef, 0xff,
  0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
  0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
  0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
  0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xef, 0xff,
  0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
  0xff, 0xff, 0x01, 0xf7, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
  0xff, 0xff, 0xff, 0xff, 0xff, 0xfc, 0x66, 0x7f, 0xff, 0xff, 0xff, 0xff,
  0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xf9, 0xe7, 0x9f,
  0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
  0xff, 0xf3, 0xe7, 0xdf, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
  0xff, 0xff, 0xff, 0xff, 0xff, 0xf7, 0xe3, 0xef, 0xff, 0xff, 0xff, 0xff,
  0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xef, 0xc3, 0xef,
  0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
  0xff, 0xef, 0x81, 0xf7, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
  0xff, 0xff, 0xff, 0xff, 0xee, 0x01, 0xc0, 0xf7, 0xff, 0xff, 0xff, 0xff,
  0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xc1, 0x46, 0x77,
  0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
  0xff, 0xcc, 0xc9, 0x07, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
  0xff, 0xff, 0xff, 0xff, 0xff, 0xee, 0x0d, 0x01, 0xbf, 0xff, 0xff, 0xff,
  0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xef, 0x03, 0x67,
  0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
  0xff, 0xf7, 0x83, 0xef, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
  0xff, 0xff, 0xff, 0xff, 0xff, 0xf7, 0xc7, 0xdf, 0xff, 0xff, 0xff, 0xff,
  0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xf3, 0xcf, 0xbf,
  0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
  0xff, 0xfc, 0xef, 0x7f, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
  0xff, 0xff, 0xff, 0xff, 0xff, 0xdf, 0x00, 0xff, 0xff, 0xff, 0xff, 0xff,
  0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xcf, 0xff,
  0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
  0xff, 0xff, 0xef, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
  0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xef, 0xff, 0xff, 0xff, 0xff, 0xff,
  0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
  0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
  0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
  0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xef, 0xff, 0xff, 0xff, 0xff, 0xff,
  0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
  0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
  0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
  0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
  0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff
  };
*/

static const uint8_t PROGMEM s_folabs_logo[] =
{
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x10, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x10, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x10, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x30, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x20, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x03, 0x10, 0x80, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x0c, 0x30, 0x40,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x08, 0x38, 0x20, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x08, 0x7c, 0x10, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x10, 0xfc, 0x98,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x11, 0xf2, 0xfe, 0x40, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x33, 0x36, 0xf8, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x3e, 0xb9, 0x88,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x11, 0xfe, 0x3f, 0x08, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x10, 0x7e, 0x08, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x10, 0x3c, 0x10,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x08, 0x1c, 0x10, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x0c, 0x18, 0x20, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x06, 0x18, 0x60,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x03, 0x99, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xfe, 0x08, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x10, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x10, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
};


enum alignmentType {LEFT, RIGHT, CENTER};

//static long s_sleepDurationSecs = 60; // for CO2 readings - it also takes several seconds for the sensor to do it's thing before we get a reading (so more like add 15 seconds)
static long s_sleepDurationSecs = 120;  // every two minutes to stretch the battery... (two days at 1 minute intervals)

static const int kMaxGraphPoints = 160;
static const int kGraphWidth     = 160;
static const int kGraphHeight    = 32 - 5;
static float     s_average_co2   = 0;

RTC_DATA_ATTR size_t  rtc_graph_count = 0;
RTC_DATA_ATTR float   rtc_graph[kMaxGraphPoints] = {0};

Adafruit_SCD30 scd30;


/////////////////////////////////////////////////////////////////////////////////////


void drawString(int x, int y, String text, alignmentType alignment) 
{
  int16_t  x1, y1; //the bounds of x,y and w and h of the variable 'text' in pixels.
  uint16_t w, h;
  display.setTextWrap( false );
  display.getTextBounds(text, x, y, &x1, &y1, &w, &h);
  if (alignment == RIGHT)  x = x - w;
  if (alignment == CENTER) x = x - w / 2;
  u8g2Fonts.setCursor(x, y + h);
  u8g2Fonts.print(text);
}


void draw_single_raindrop(int x, int y, int scale) 
{
  display.fillCircle(x, y, scale / 2, FORECOLOR);
  display.fillTriangle(x - scale / 2, y, x, y - scale * 1.2, x + scale / 2, y , FORECOLOR);
}


void draw_pressure_icon(int x, int y, int scale) 
{
  display.fillTriangle( x - scale / 2, y,   x, y + scale * 1.2,    x + scale / 2, y , FORECOLOR);
}


void Blink(byte PIN, int DELAY_MS, byte loops)
{
  for (byte i = 0; i < loops; i++)
  {
    digitalWrite(PIN, LOW);
    delay(DELAY_MS);
    digitalWrite(PIN, HIGH);
    delay(DELAY_MS);
  }
}


void BeginSleep()
{
  // turn off the led if on
  pinMode( LED_BUILTIN, INPUT );
  digitalWrite( LED_BUILTIN, HIGH );

  DisableInternalPower();

  Serial.println( "Starting deep-sleep period..." );
  
  // put microcontroller to sleep, wake up after specified time
  ESP.deepSleep( s_sleepDurationSecs * 1e6 );
}


void EnableInternalPower() 
{
#if defined(NEOPIXEL_POWER)
  pinMode(NEOPIXEL_POWER, OUTPUT);
  digitalWrite(NEOPIXEL_POWER, HIGH);
#endif

#if defined(NEOPIXEL_I2C_POWER)
  pinMode(NEOPIXEL_I2C_POWER, OUTPUT);
  digitalWrite(NEOPIXEL_I2C_POWER, HIGH);
#endif

#if defined(ARDUINO_ADAFRUIT_FEATHER_ESP32S2)
  // turn on the I2C power by setting pin to opposite of 'rest state'
  pinMode(PIN_I2C_POWER, INPUT);
  delay(1);
  bool polarity = digitalRead(PIN_I2C_POWER);
  pinMode(PIN_I2C_POWER, OUTPUT);
  digitalWrite(PIN_I2C_POWER, !polarity);
  pinMode(NEOPIXEL_POWER, OUTPUT);
  digitalWrite(NEOPIXEL_POWER, HIGH);
#endif

  pinMode(TRANSISTOR_GATE_PIN, OUTPUT);
  digitalWrite(TRANSISTOR_GATE_PIN, HIGH);
}


void DisableInternalPower() 
{
#if defined(NEOPIXEL_POWER)
  pinMode(NEOPIXEL_POWER, OUTPUT);
  digitalWrite(NEOPIXEL_POWER, LOW);
#endif

#if defined(NEOPIXEL_I2C_POWER)
  pinMode(NEOPIXEL_I2C_POWER, OUTPUT);
  digitalWrite(NEOPIXEL_I2C_POWER, LOW);
#endif

#if defined(ARDUINO_ADAFRUIT_FEATHER_ESP32S2)
  // turn on the I2C power by setting pin to rest state (off)
  pinMode(PIN_I2C_POWER, INPUT);
  pinMode(NEOPIXEL_POWER, OUTPUT);
  digitalWrite(NEOPIXEL_POWER, LOW);
#endif

  pinMode(TRANSISTOR_GATE_PIN, OUTPUT);
  digitalWrite(TRANSISTOR_GATE_PIN, LOW);
}


void set_sensor_interval( long interval )
{
  if( !scd30.setMeasurementInterval( interval  ) )
  {
    // try again
    delay( 1 );
    if( !scd30.setMeasurementInterval( interval  ) )
      Serial.println("Failed to set measurement interval");
  }

  Serial.print( "Measurement Interval: "); 
  Serial.print( scd30.getMeasurementInterval() ); 
  Serial.println( " secs" );  
}


void setup(void)
{
  Serial.begin( 115200 );

  pinMode( LED_BUILTIN, OUTPUT );
  EnableInternalPower();
  delay(500);

  u8g2Fonts.begin(display);                  // connect u8g2 procedures to Adafruit GFX
  u8g2Fonts.setFontMode(1);                  // use u8g2 transparent mode (this is default)
  u8g2Fonts.setFontDirection(0);             // left to right (this is default)
  u8g2Fonts.setForegroundColor(FORECOLOR); // apply Adafruit GFX color
  u8g2Fonts.setBackgroundColor(BACKCOLOR); // apply Adafruit GFX color

  if( !scd30.begin() ) 
  {
    Serial.println("Failed to find SCD30 chip");
//    DisableInternalPower();
    BeginSleep();
//    while( 1 ) 
//      delay(10);
  }
  
  // we don't want to wait long for a measurement... 
  set_sensor_interval( 2 );

  while( !scd30.dataReady() ) 
    delay( 1 );
  
  DisplayCO2();
  BeginSleep();
}


void DisplayCO2()
{
  Serial.println("Updating display...");

#ifdef TAKE_AVERAGE_CO2
  // wait a little while so the device can take more than one measurement... (interval is set to 2 seconds)
  delay( 6 ); // try for 3 readings
#endif

  scd30.read();
  s_average_co2 = scd30.CO2;
 
  display.begin();
  display.clearBuffer();
  display.fillScreen( BACKCOLOR );

  display.drawBitmap( 0, -1, s_folabs_logo, 128, 32, FORECOLOR );

  u8g2Fonts.setFont( u8g2_font_helvB10_tf );
  drawString( 40, 12, "Far Out Labs", LEFT );

// draw a graph of the CO2 over the last while...
  drawCO2( 85, 65, s_average_co2 );
  addCO2Point( s_average_co2 );
  drawCO2Graph( 40, 65 + 25, kGraphWidth, kGraphHeight );
  
  drawBattery( 200, 22 );
  drawTempAndHumidity( 42, 32, c2f( scd30.temperature - kTempOffsetC ), scd30.relative_humidity );

  // send to e-paper display
  display.display();
  Blink( LED_BUILTIN, 100, 1 );
}


void drawCO2( int x, int y, float co2 )
{
  // draw the CO2
  char smallbuf[32];

  int offset = 0;
  
  // keep the string "centered"
  if( co2 >= 1000 )
    offset = -10;

  u8g2Fonts.setFont( u8g2_font_helvB24_tf );
  u8g2Fonts.setCursor( x + offset, y + 12 );
  sprintf( smallbuf, "%.0f ", co2 );
  u8g2Fonts.print( smallbuf );  
  
  u8g2Fonts.setFont( u8g2_font_helvB10_tf );
  u8g2Fonts.print("CO");

  // move down slightly so we can write the 2 as subscript
  u8g2Fonts.setCursor( u8g2Fonts.getCursorX(), u8g2Fonts.getCursorY() + 5 );
  u8g2Fonts.print("2");
  
  u8g2Fonts.setCursor( u8g2Fonts.getCursorX(), u8g2Fonts.getCursorY() - 5 );
  u8g2Fonts.setFont( u8g2_font_helvR10_tf );
  u8g2Fonts.print(" ppm");

}


void drawTempAndHumidity( int x, int y, float tempF, uint8_t humidity )
{
  // there are three zones on one line
  char smallbuf[32];
  u8g2Fonts.setForegroundColor( FORECOLOR );
  u8g2Fonts.setFont( u8g2_font_helvR08_tf );
  
  if( tempF >= 100 )
      u8g2Fonts.setForegroundColor( EPD_RED );
  else
    u8g2Fonts.setForegroundColor( FORECOLOR );

  u8g2Fonts.setFont( u8g2_font_helvB12_tf );
  sprintf( smallbuf, "%.1f°F", tempF );
  drawString( x, y, smallbuf, LEFT );

  draw_single_raindrop( x + 115, y + 4, 10 );

  if( humidity == 100 )
    u8g2Fonts.setForegroundColor( EPD_RED );
  else
    u8g2Fonts.setForegroundColor( FORECOLOR );

  sprintf( smallbuf, "%3d%%", humidity );
  drawString( x + 125, y, smallbuf, LEFT );   
}



//#########################################################################################
void drawRSSI( int x, int y, int rssi ) 
{
  int WIFIsignal = 0;
  int xpos = 1;
  int x_offset = 0;

//  Serial.println( "rssi = " + String( rssi ) );
  if( rssi > -20 )
    x_offset = -3; // better center the graph when we have 5 bars
    
  for( int _rssi = -100; _rssi <= rssi; _rssi = _rssi + 20 ) 
  {
    if (_rssi <= -20)  WIFIsignal = 20; //            <-20dbm displays 5-bars
    if (_rssi <= -40)  WIFIsignal = 16; //  -40dbm to  -21dbm displays 4-bars
    if (_rssi <= -60)  WIFIsignal = 12; //  -60dbm to  -41dbm displays 3-bars
    if (_rssi <= -80)  WIFIsignal = 8;  //  -80dbm to  -61dbm displays 2-bars
    if (_rssi <= -100) WIFIsignal = 4;  // -100dbm to  -81dbm displays 1-bar
    display.fillRect( x + x_offset + xpos * 6, y - WIFIsignal, 5, WIFIsignal, FORECOLOR );
    xpos++;
  }
  display.fillRect( x + x_offset, y - 1, 5, 1, FORECOLOR );
  u8g2Fonts.setFont( u8g2_font_tom_thumb_4x6_tf );
  drawString( x + 17,  y, String( rssi ) + "dBm", CENTER );
}


//#########################################################################################
void drawBattery( int x, int y )
{
  uint8_t percentage = 100;
  float voltage = analogRead( VREF_PIN ) / 4096.0 * 7.46;

  if( voltage > 1 ) 
  { 
    // Only display if there is a valid reading
    int16_t color = FORECOLOR;
    
    percentage = 2836.9625 * pow( voltage, 4 ) - 43987.4889 * pow( voltage, 3 ) + 255233.8134 * pow( voltage, 2 ) - 656689.7123 * voltage + 632041.7303;
    if( voltage >= 4.20 ) 
      percentage = 100;
      
    if( voltage <= 3.50 ) 
      percentage = 0;

    Serial.println( "Voltage = " + String( voltage ) + " " + String( percentage ) + "%" );

    // do not draw the battery if it is more than 1/2 full
//    if( percentage > BATTERY_VID_THRESHOLD )
//      return;

    if( percentage <= LOW_BATTERY_THRESHOLD )
      color = EPD_RED;

    display.drawRect( x + 15, y - 12, 19, 9, FORECOLOR );
    display.fillRect( x + 13, y - 10, 2, 5, FORECOLOR );
    display.fillRect( x + 17, y - 10, 15 * percentage / 100.0, 5, color );
//    u8g2Fonts.setFont( u8g2_font_tom_thumb_4x6_tf );
//    drawString( x + 10, y - 11, String( percentage ) + "%", RIGHT );
//    drawString( x + 13, y + 5,  String( voltage, 2 ) + "v", CENTER );
  }
}


//#########################################################################################
void drawBatteryV( int x, int y )
{
  uint8_t percentage = 100;
//  float voltage = analogRead( VREF_PIN ) / 4096.0 * 7.46;
  float voltage = 3.9;
  if( voltage > 1 ) 
  { 
    // Only display if there is a valid reading
    Serial.println( "Voltage = " + String( voltage ) );

    int16_t color = FORECOLOR;
    
    percentage = 2836.9625 * pow( voltage, 4 ) - 43987.4889 * pow( voltage, 3 ) + 255233.8134 * pow( voltage, 2 ) - 656689.7123 * voltage + 632041.7303;
    if( voltage >= 4.20 ) 
      percentage = 100;
      
    if( voltage <= 3.50 ) 
      percentage = 0;

    if( percentage < LOW_BATTERY_THRESHOLD )
      color = EPD_RED;
      
    display.drawRect( x - 12, y + 15, 10, 19, FORECOLOR );
    display.fillRect( x - 10, y + 34, 5,   2, FORECOLOR );
    display.fillRect( x - 10, y + 17, 6, 15 * percentage / 100.0, color );
    u8g2Fonts.setFont( u8g2_font_tom_thumb_4x6_tf );
    drawString( x - 11, y + 10, String( percentage ) + "%", RIGHT );
    drawString( x + 5,  y + 13, String( voltage, 2 ) + "v", CENTER );
  }
}


void drawDottedLineV( int16_t x, int16_t y, int16_t h )
{
  int number_of_dashes = h / 2; // calculate this based on the line length divided by 2

  // Draw dotted grid lines
  for( int j = 0; j < number_of_dashes; j++ ) 
  {     
      display.drawFastVLine( x, y + (j * 2), 1, FORECOLOR );
  }
}



void drawCO2Graph( int x, int y, int width, int height )
{
  int graphTop    = y;
  int graphBottom = y + height;
  int graphWidth  = width - 1;
  int lastH       = graphBottom - 1;

  float minCO2 = 40000;
  float maxCO2 = 0;

  // find min/max to normalize data to 0..1
  for ( int i = 0; i < rtc_graph_count; i++ )
  {
    if( rtc_graph[i] > maxCO2 )
      maxCO2 = rtc_graph[i];
      
    if( rtc_graph[i] < minCO2 )
      minCO2 = rtc_graph[i];
  }  

  // draw some divider lines to split the display into thirds (i.e. roughly 2 hour chunks)
  float divider = width / 3;
  drawDottedLineV( x + divider, y, height );
  drawDottedLineV( x + divider * 2, y, height );

  float range = maxCO2 - minCO2;
  if( range == 0.0f )
    range = 0.15;  // prevent divide by zero - note this value is chosen to almost center a random pressure value...

  float delta_width = width / kMaxGraphPoints;
  for ( int i = 0; i < rtc_graph_count; i++ )
  {
    float r = (rtc_graph[i] - minCO2) / range;
    int   h = (graphBottom - 1) - (r * (height - 1));
    if( i == 0 )
      lastH = h;  // start at the first point instead of at zero

    // draw graph backwards (newest data last)
    display.drawLine( x + graphWidth - (i * delta_width), lastH, x + graphWidth - (i * delta_width), h, FORECOLOR );
    lastH = h;
  }

  // frame the graph
  display.drawRect( x, y, width, height, FORECOLOR );

  // cheat a bit and draw outside our frame
  char smallbuf[32];
  sprintf( smallbuf, "%0.0f", maxCO2 );
  u8g2Fonts.setFont( u8g2_font_p01type_tn );
  drawString( x - 19, y - 3, smallbuf, LEFT );

  sprintf( smallbuf, "%0.0f", minCO2 );
  u8g2Fonts.setFont( u8g2_font_p01type_tn );
  drawString( x - 19, y + height - 11, smallbuf, LEFT );

  float graphSeconds = rtc_graph_count * s_sleepDurationSecs;
  sprintf( smallbuf, "%.1f hrs", graphSeconds / 3600.0f );
  u8g2Fonts.setFont( u8g2_font_tom_thumb_4x6_tf );
  drawString( x + width + 4, y + height - 9, smallbuf, LEFT );
}


void addCO2Point( float co2 )
{
    // see if there's room to move stuff without truncating
    if( rtc_graph_count )
    {
        if( rtc_graph_count < kMaxGraphPoints - 1 )
        {
            // copy only what we have, which will be faster than copying the entire buffer every add
            memmove( &rtc_graph[1], &rtc_graph[0], rtc_graph_count * sizeof( float ) );
        }
        else
        {
            // the buffer is full so we need to move a shortened version of the buffer (which effectively truncates it)
            memmove( &rtc_graph[1], &rtc_graph[0], (rtc_graph_count - 1) * sizeof( float ) );
        }
    }
    
    // stop buffering if we have a big enough window, this can grow but never shrink
    if( rtc_graph_count < (kMaxGraphPoints - 1) )
        ++rtc_graph_count;

    // put the actual data in there now
    memcpy( rtc_graph, &co2, sizeof( float ) );
}


void loop()
{
}


// EOF
