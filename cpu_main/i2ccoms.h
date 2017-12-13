#pragma once
#include "defines.h"
#include <Wire.h>
#include <LiquidCrystal_I2C.h>


// set the LCD address to 0x3F for a 20 chars 4 line display
// Set the pins on the I2C chip used for LCD connections:
//                    addr, en,rw,rs,d4,d5,d6,d7,bl,blpol
LiquidCrystal_I2C lcd(0x3F,2,1,0,4,5,6,7,3,POSITIVE);

void i2cinit()
{
	lcd.begin(20,4);
}

void i2cupdate()
{
}

 void i2con_boot()
{
	lcd.setBacklight(HIGH);
	lcd.setCursor(1, 0);
	lcd.print("8x8x8 RGB LED Cube");
	delay(1000);
	lcd.setCursor(2, 1);
	lcd.print("Boot Sequence...");
	delay(2000);
}

 void lcdmenu()
 {

 }