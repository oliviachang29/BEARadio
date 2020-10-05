/*
    Description: Read the characters entered by CardKB Unit and display them on the screen.
*/

// #define CARDKB_ADDR 0x5F // unnecessary because hd44780 lib auto detects
#define LCD_ADDR 0x27

#include <Wire.h>
#include <hd44780.h>            // main hd44780 header
#include <hd44780ioClass/hd44780_I2Cexp.h>  // i2c expander i/o class header

hd44780_I2Cexp lcd; // declare lcd object: auto locate & config exapander chip

// LCD geometry
// while 16x2 will work on most displays even if the geometry is different,
// for actual wrap testing of a particular LCD it is best to use the correct
// geometry.
const int LCD_COLS = 20;
const int LCD_ROWS = 4;

void setup()
{
  int status;
  // initialize LCD with number of columns and rows: 
  // hd44780 returns a status from begin() that can be used
  // to determine if initalization failed.
  // the actual status codes are defined in <hd44780.h>
  status = lcd.begin(LCD_COLS, LCD_ROWS);
  if(status) // non zero status means it was unsuccesful
  {
    // begin() failed so blink error code using the onboard LED if possible
    hd44780::fatalError(status); // does not return
  }

  // turn on automatic line wrapping
  // which automatically wraps lines to the next lower line and wraps back
  // to the top when at the bottom line
  // NOTE: 
  // noLineWrap() can be used to disable automatic line wrapping.
  // _write() can be called instead of write() to send data bytes
  // to the display bypassing any special character or line wrap processing.
  lcd.lineWrap();
  
  lcd.clear();
  // lcd.backlight();

  Serial.begin(115200);
  Wire.begin();
  pinMode(5, INPUT);
  digitalWrite(5, HIGH);
  
  lcd.setCursor(0, 0);
  lcd.blink();
}

void loop()
{
  Wire.requestFrom(CARDKB_ADDR, 1);
  while(Wire.available())
  {
    char c = Wire.read(); // receive a byte as characterif
    // handle backspace
    if (c == 8) {
      lcd.moveCursorLeft();
      lcd.print(' ');
      lcd.moveCursorLeft();
    } else if (c != 0) {
      lcd.print(c);
      Serial.println(c, HEX);
    }

  }
  // delay(10);
}
