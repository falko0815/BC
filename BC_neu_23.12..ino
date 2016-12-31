#include <SPI.h>
#include <OneWire.h>
#include <DallasTemperature.h>
#include "Adafruit_GFX.h"    // Adafruit Grafik-Bibliothek
#include "Adafruit_ST7735.h" // Adafruit ST7735-Bibliothek
#define ONE_WIRE_BUS 2 /* Digitalport Pin 2 definieren */
OneWire ourWire(ONE_WIRE_BUS); /* Ini oneWire instance */
DallasTemperature sensors(&ourWire);/* Dallas Temperature Library für Nutzung der oneWire Library vorbereiten */

#include <Wire.h>


// TFT-Display
#define CS   10 // Arduino-Pin an Display CS   
#define DC   9  // Arduino-Pin an Display A0
#define RST  8  // Arduino Reset-Pin

Adafruit_ST7735 tft = Adafruit_ST7735(CS, DC, RST);  // Display-Bibliothek Setup


boolean night_mode=true;

void setup(void) {
  
  sensors.begin();/* Inizialisieren der Dallas Temperature library */
  
  tft.initR(INITR_BLACKTAB);     // ST7735-Chip initialisieren
  
  display_show();
}


float temp=0.0;
float hum=1000;
float pressure=1000;
float min_temp=1000;
float max_temp=-1000;
float min_humidity=1000;
float max_humidity=-1000;
float min_pressure=1000000;
float max_pressure=-1000;
int analogInput = 0;
float vout = 0.0;
float vin = 0.0;
float R1 = 97000.0; // Widerstandswert R1 (100K) - siehe Schaltskizze!
float R2 = 10000.0; // Widerstandswert R2 (10K) - siehe Schaltskizze!
int values = 0;
float v = 0.0;

int tempct=0;

void loop() {

     // Werte am analogen Pin lesen
     values = analogRead(analogInput); //Messwerte am analogen Pin als "values" definieren
     vout = (values * 5.0) / 1024.0; // Messwerte in Volt umrechnen = Spannung am Ausgang des
    // Spannungsteilers mit Zuleitung zu Pin A0
    vin = vout / (R2 / (R1 + R2)); // Berechnen, welche Spannung am Eingang des Spannungsteilers
    // anliegt. Das entspricht der Spannung der zu untersuchenden Batterie


  
  float t;
  sensors.requestTemperatures(); // Temp abfragen
  t = (sensors.getTempCByIndex(0));
  if(isnan(t)){}
  else if((int)t!=(int)temp){
   show_temp(temp,true);
   temp=t;
   if(min_temp>temp)min_temp=temp;
   if(max_temp<temp)max_temp=temp;
   show_temp(temp,false);
  }
  tempct++;
  
 
  float p= vin;
  if(p!=pressure){
   show_pressure(pressure,true);
   pressure=p;
   if(min_pressure>pressure)min_pressure=pressure;
   if(max_pressure<pressure)max_pressure=pressure;
   show_pressure(pressure,false);
  }
  
  delay(100);  
 
}

void show_temp(float temp,boolean clear){
  
  int clearcolor=night_mode?ST7735_BLACK:ST7735_WHITE;
  int textcolor=night_mode?ST7735_WHITE:ST7735_BLACK;
  
  byte xs=12;
  byte ys=66;
  String htemp=String((float)temp);
  //byte xss=(temp<10?:temp
  
  set_text(xs,ys-50,htemp,clear?clearcolor:textcolor,3);
  set_text(xs+(3*htemp.length()*6),ys-50,"",(clear?clearcolor:textcolor),2);
  tft.write(247); // das °-Zeichen  
  tft.print("C");
  set_text(xs+11,ys-11,"max ",ST7735_RED,1);
  set_text(xs+41,ys-11,String((float)max_temp),clear?clearcolor:ST7735_RED,1);
  tft.write(247);tft.print("C");
  set_text(xs+11,ys-1,"min ",ST7735_BLUE,1);
  set_text(xs+41,ys-1,String((float)min_temp),clear?clearcolor:ST7735_BLUE,1);
  tft.write(247);tft.print("C");
}

void show_hum(float hum,boolean clear){

  int clearcolor=night_mode?ST7735_BLACK:ST7735_WHITE;
  int textcolor=night_mode?ST7735_WHITE:ST7735_BLACK;

  byte xs=12;
  byte ys=96;
  
  set_text(xs,ys,String((int)hum)+"%",clear?clearcolor:textcolor,3);
  set_text(xs+81,ys+1,String((int)max_humidity)+"%",clear?clearcolor:ST7735_GREEN,1);
  set_text(xs+81,ys+11,String((int)min_humidity)+"%",clear?clearcolor:ST7735_YELLOW,1);
}

void show_pressure(float pressure,boolean clear){
  
  int clearcolor=night_mode?ST7735_BLACK:ST7735_WHITE;
  int textcolor=night_mode?ST7735_WHITE:ST7735_BLACK;
  
  byte xs=12;
  byte ys=90;
  
  set_text(xs,ys,String((float)pressure)+" Volt",clear?clearcolor:textcolor,2);
  
  if(max_pressure>=100000)xs-=8; 
  set_text(xs+11,ys+30,"max ",ST7735_RED,1);
  set_text(xs+41,ys+30,String((float)max_pressure)+" Volt",clear?clearcolor:ST7735_RED,1);
  set_text(xs+11,ys+40,"min ",ST7735_BLUE,1);
  set_text(xs+41,ys+40,String((float)min_pressure)+" Volt",clear?clearcolor:ST7735_BLUE,1);//
}


void set_text(int x,int y,String text,int color,int size){
  
  tft.setTextSize(size);
  tft.setCursor(x,y);
  tft.setTextColor(color);
  tft.print(text);
}

void display_show(){

  tft.fillScreen(night_mode?ST7735_BLACK:ST7735_WHITE); 
  
  set_text(2,4,"Temperatur",ST7735_WHITE,1);  
  set_text(14,147,"Batteriespannung",ST7735_WHITE,1);
  
}


