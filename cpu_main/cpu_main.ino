/*
 Name:		cpu_main.ino
 Created:	03.12.2017 16:22:27
 Author:	Kai
*/

#include "defines.h"
#include "ws2811.h"
#include "wirecoms.h"
#include "i2ccoms.h"
#include "init.h"
#include <FastLED.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>


const int rel[10] = { 30,31,32,33,34,35,36,37,26,27 };  // Array rel[] muss mit 0 bis 9 angesprochen werden. Entsprechend der Relais 1 bis 10
const int fb_rel[10] = { 61,62,63,64,65,66,67,68,69,60 };
float volt[10];
int dip[8];
byte recv[10];
byte bvolt[10];



void setup() {
	pinMode(debug_pin, INPUT);
	pinMode(close_con_rel, OUTPUT);
	pinMode(open_con_rel, OUTPUT);
	pinMode(gpu_rel, OUTPUT);
	pinMode(gpustat_data, OUTPUT);
	pinMode(peristat_data, OUTPUT);
	for (int i = 0; i<10; i++) {
		pinMode(fb_rel[i], INPUT);
	}
	for (int i = 0; i<10; i++) {
		pinMode(rel[i], OUTPUT);
	}
	startup();
	//digitalWrite(open_con_rel, HIGH);
	//digitalWrite(open_con_rel, LOW);
	//digitalWrite(gpu_rel, HIGH);
	//timer_setup();
}


/*
ISR(TIMER3_COMPA_vect) {

}



ISR(TIMER4_COMPA_vect) {

}
*/

void loop() {
	delay(5000);
}


/*
void rel_off(int r) {
	if (r == 0) {
		for (int count = 0; count<10; count++) {
			digitalWrite(rel[count], LOW);
			delay(1000);
		}
	}
	else {
		digitalWrite(rel[r - 1], LOW);
		delay(1000);
	}
}

void rel_on(int r) {
	if (r == 0) {
		for (int count = 0; count<10; count++) {
			digitalWrite(rel[count], HIGH);
			delay(1000);
		}
	}
	else {
		digitalWrite(rel[r - 1], HIGH);
		delay(1000);
	}
}


void update_feedback() {
	float sense[10];
	for (int i = 0; i<10; i++) {
		sense[i] = analogRead(fb_rel[i]);
		volt[i] = sense[i] * (5.0 / 1024);
		if (volt[i]<1) {
			volt[i] = 0;
		}
	}
	for (int count = 0; count<10; count++) {
		bvolt[count] = volt[count];
	}
}


void timer_setup() {
	TCCR3A = 0;
	TCCR3B = 0;
	TCNT3 = 0;
	OCR3A = 31250;
	TCCR3B |= (1 << WGM12); // CTC mode
	TCCR3B |= (1 << CS12);     // 256 prescaler 
	TIMSK3 |= (1 << OCIE1A);
	TCCR4A = 0;
	TCCR4B = 0;
	TCNT4 = 0;
	OCR4A = 31250;
	TCCR4B |= (1 << WGM12);   // CTC mode
	TCCR4B |= (1 << CS12);  // 256 prescaler
	TIMSK4 |= (1 << OCIE1A);
	interrupts();
}

*/