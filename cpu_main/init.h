#pragma once
#include "i2ccoms.h"
#include "defines.h"


void startup()
{
	Serial.begin(115200);
	Serial.println("Serial Initialized @ 115200");
	Serial.println("I2C INIT Started");
	i2cinit();
	Serial.println("I2C INIT Complete");
	Serial.println("I2C Boot");
	i2con_boot();
	lcd.clear();
	lcd.setCursor(0, 0);
	lcd.print("WS2811 INIT Start");
	ws2811init();
	delay(1000);
	lcd.setCursor(0, 1);
	lcd.print("WS2811 INIT Finish");
	delay(1000);
	lcd.clear();
	lcd.setCursor(0, 0);
	lcd.print("Wire INIT Start");
	wireinit();
	delay(1000);
	lcd.setCursor(0, 1);
	lcd.print("Wire INIT Finish");
	delay(1000);
	lcd.clear();
	lcd.setCursor(1, 0);
	lcd.print("WS2811 Boot");
	ws2811on_boot();
	lcd.clear();
}
