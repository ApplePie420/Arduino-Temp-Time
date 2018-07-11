//Code written by Radovan "N3ttX" Behýl, (c) 2018
//Free-to-use and free-to-share license only for non-commercial use. Always credit the author and respective libraries authors.
//-----LIBRARIES-----
//RTClib  (https://www.elecrow.com/wiki/index.php?title=Tiny_RTC)
//U8glib  (https://github.com/olikraus/u8glib)
//LM75A   (https://github.com/QuentinCG/Arduino-LM75A-Temperature-Sensor-Library)
//-----PARTS-----
//I2C OLED display with 128x64 resolution
//TinyRTC DS1307 based RTC module (I2C)
//LM75A temperature sensor module
//
//Every device is connected to I2C BUS and there is no need to have schematic diagram as long as you connect all pins properly (Vcc, GND, SCL, SDA) 

//Include libraries
#include <RTClib.h>
#include <U8glib.h>
#include <LM75A.h>
#include <Wire.h>

//Declare Instances
LM75A temp;
U8GLIB_SSD1306_128X64 oled(U8G_I2C_OPT_NONE);
RTC_DS1307 rtc;

//Declare global variables
DateTime currentTime;
float currentTemp = 0;

void setup() {
  Wire.begin();
  rtc.begin();
  rtc.adjust(DateTime(__DATE__, __TIME__));     //adjust RTC everytime on startup
  oled.setColorIndex(1);                        //tell the OLED to set pixels ON (white)
}

void loop() {
  currentTime = rtc.now();                      //get current time from RTC in DateTime format
  currentTemp = temp.getTemperatureInDegrees(); //get current temperature from LM75A
  
  oled.firstPage();                             //tell the OLED to start drawing
  do {
    drawOled();                                 //our function, where everything is drawn
  } while(oled.nextPage());                     //do it while writing is not done
  delay(1000);
}

void drawOled() {
  //Draw the frame, helper lines
  oled.drawRFrame(0,0,128,64,3);
  oled.drawLine(0, 24, 128, 24);
  oled.drawLine(64, 28, 64, 64);
  oled.drawLine(0, 28, 128, 28);

  //Draw the temperature box in middle, scaled by 2,5 (since we dont need to go to 128 degrees, 48 is enough)
  oled.drawBox(0, 25, currentTemp*2.5, 3);

  //Draw the temperature with "°C" symbol made of "C" character and two circles (as "°" symbol)
  oled.setFont(u8g_font_courB18);
  oled.setPrintPos(15,20);
  oled.print(currentTemp);
  oled.drawCircle(94, 6, 3);
  oled.drawCircle(94, 6, 2);
  oled.setPrintPos(98,20);
  oled.print("C");

  //Drawt the date, month and day on first line and year on second line
  oled.setFont(u8g_font_9x15);
  if(currentTime.day() >= 10) {
    oled.setPrintPos(15,45);
  } else {
    oled.setPrintPos(25, 45);
  }
  oled.print(currentTime.day());
  oled.setPrintPos(32, 45);
  oled.print(".");
  oled.setPrintPos(42, 45);
  oled.print(currentTime.month());
  oled.setPrintPos(15, 58);
  oled.print(currentTime.year());

  //Draw the time, again, hours and minutes on first line and seconds on second line
  oled.setPrintPos(75,45);
  oled.print(currentTime.hour());
  oled.setPrintPos(90,45);
  oled.print(":");
  oled.setPrintPos(97, 45);
  oled.print(currentTime.minute());
  oled.setPrintPos(86, 58);
  oled.print(currentTime.second());

  //Draw the temperature box legend
  oled.setFont(u8g_font_baby);
  oled.setPrintPos(2, 35);
  oled.print("O");
  oled.setPrintPos(117, 35);
  oled.print("48");
}

