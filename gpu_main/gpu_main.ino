/*
 Name:		gpu_main.ino
 Created:	03.12.2017 16:23:04
 Author:	Kai
*/



/*
The 8x8x8 RGB LED Cube

Latest
V12 04/17/2013

Release Notes:
V11
- Fixed bug with BAM timing
- Moved Blank pin setup to ISR so pins are dead until they are written to

V12
bitwise operation to set pins LOW was incorrect
should be PORTx &= ~(1<<pinNumber);

*/

#include <SPI.h>// SPI Library used to clock data out to the shift registers
#include <Wire.h> //I2C library for communication
#include "animations.h"

#define latch_pin 2// can use any pin you want to latch the shift registers
#define blank_pin 3// same, can use any pin you want for this, just make sure you pull up via a 1k to 5V
#define data_pin 51// used by SPI, must be pin 51
#define clock_pin 52// used by SPI, must be 52

//****setup****setup****setup****setup****setup****setup****setup****setup****setup****setup****setup****setup****setup
void setup() {

	SPI.setBitOrder(MSBFIRST);//Most Significant Bit First
	SPI.setDataMode(SPI_MODE0);// Mode 0 Rising edge of data, keep clock low
	SPI.setClockDivider(SPI_CLOCK_DIV2);//Run the data in at 16MHz/2 - 8MHz

	Serial.begin(115200);// if you need it?
	Serial.print("Serial Initialized @ 115200");
	noInterrupts();// kill interrupts until everybody is set up

				   //We use Timer 1 to refresh the cube
	TCCR1A = B00000000;//Register A all 0's since we're not toggling any pins
	TCCR1B = B00001011;//bit 3 set to place in CTC mode, will call an interrupt on a counter match
					   //bits 0 and 1 are set to divide the clock by 64, so 16MHz/64=250kHz
	TIMSK1 = B00000010;//bit 1 set to call the interrupt on an OCR1A match
	OCR1A = 30; // you can play with this, but I set it to 30, which means:
				//our clock runs at 250kHz, which is 1/250kHz = 4us
				//with OCR1A set to 30, this means the interrupt will be called every (30+1)x4us=124us, 
				// which gives a multiplex frequency of about 8kHz

				// here I just set up the anode array, this is what's written to the anode shift register, to enable each level
	anode[0] = B00000001;
	anode[1] = B00000010;
	anode[2] = B00000100;
	anode[3] = B00001000;
	anode[4] = B00010000;
	anode[5] = B00100000;
	anode[6] = B01000000;
	anode[7] = B10000000;

	// don't hate on how I assigned the values to this register! haha

	//finally set up the Outputs
	pinMode(latch_pin, OUTPUT);//Latch
	pinMode(data_pin, OUTPUT);//MOSI DATA
	pinMode(clock_pin, OUTPUT);//SPI Clock
	pinMode(blank_pin, OUTPUT);//Output Enable  important to do this last, so LEDs do not flash on boot up
	SPI.begin();//start up the SPI library
	interrupts();//let the show begin, this lets the multiplexing start
}//***end setup***end setup***end setup***end setup***end setup***end setup***end setup***end setup***end setup***end setup


void loop() {//***start loop***start loop***start loop***start loop***start loop***start loop***start loop***start loop***start loop

			 //Each animation located in a sub routine
			 // To control an LED, you simply:
			 // LED(level you want 0-7, row you want 0-7, column you want 0-7, red brighness 0-15, green brighness 0-15, blue brighness 0-15);

	rainVersionTwo();
	folder();
	sinwaveTwo();
	wipe_out();
	clean();
	bouncyvTwo();
	color_wheelTWO();
	clean();
	harlem_shake();




}//***end loop***end loop***end loop***end loop***end loop***end loop***end loop***end loop***end loop***end loop***end loop***end loop


ISR(TIMER1_COMPA_vect) {//***MultiPlex BAM***MultiPlex BAM***MultiPlex BAM***MultiPlex BAM***MultiPlex BAM***MultiPlex BAM***MultiPlex BAM

						//This routine is called in the background automatically at frequency set by OCR1A
						//In this code, I set OCR1A to 30, so this is called every 124us, giving each level in the cube 124us of ON time
						//There are 8 levels, so we have a maximum brightness of 1/8, since the level must turn off before the next level is turned on
						//The frequency of the multiplexing is then 124us*8=992us, or 1/992us= about 1kHz


	PORTD |= 1 << blank_pin;//The first thing we do is turn all of the LEDs OFF, by writing a 1 to the blank pin
							//Note, in my bread-boarded version, I was able to move this way down in the cube, meaning that the OFF time was minimized
							//do to signal integrity and parasitic capcitance, my rise/fall times, required all of the LEDs to first turn off, before updating
							//otherwise you get a ghosting effect on the previous level

							//This is 4 bit 'Bit angle Modulation' or BAM, There are 8 levels, so when a '1' is written to the color brightness, 
							//each level will have a chance to light up for 1 cycle, the BAM bit keeps track of which bit we are modulating out of the 4 bits
							//Bam counter is the cycle count, meaning as we light up each level, we increment the BAM_Counter
	if (BAM_Counter == 8)
		BAM_Bit++;
	else
		if (BAM_Counter == 24)
			BAM_Bit++;
		else
			if (BAM_Counter == 56)
				BAM_Bit++;

	BAM_Counter++;//Here is where we increment the BAM counter

	switch (BAM_Bit) {//The BAM bit will be a value from 0-3, and only shift out the arrays corresponding to that bit, 0-3
					  //Here's how this works, each case is the bit in the Bit angle modulation from 0-4, 
					  //Next, it depends on which level we're on, so the byte in the array to be written depends on which level, but since each level contains 64 LED,
					  //we only shift out 8 bytes for each color
	case 0:
		for (shift_out = level; shift_out<level + 8; shift_out++)
			SPI.transfer(red0[shift_out]);
		for (shift_out = level; shift_out<level + 8; shift_out++)
			SPI.transfer(green0[shift_out]);
		for (shift_out = level; shift_out<level + 8; shift_out++)
			SPI.transfer(blue0[shift_out]);
		break;
	case 1:
		for (shift_out = level; shift_out<level + 8; shift_out++)
			SPI.transfer(red1[shift_out]);
		for (shift_out = level; shift_out<level + 8; shift_out++)
			SPI.transfer(green1[shift_out]);
		for (shift_out = level; shift_out<level + 8; shift_out++)
			SPI.transfer(blue1[shift_out]);
		break;
	case 2:
		for (shift_out = level; shift_out<level + 8; shift_out++)
			SPI.transfer(red2[shift_out]);
		for (shift_out = level; shift_out<level + 8; shift_out++)
			SPI.transfer(green2[shift_out]);
		for (shift_out = level; shift_out<level + 8; shift_out++)
			SPI.transfer(blue2[shift_out]);
		break;
	case 3:
		for (shift_out = level; shift_out<level + 8; shift_out++)
			SPI.transfer(red3[shift_out]);
		for (shift_out = level; shift_out<level + 8; shift_out++)
			SPI.transfer(green3[shift_out]);
		for (shift_out = level; shift_out<level + 8; shift_out++)
			SPI.transfer(blue3[shift_out]);
		//Here is where the BAM_Counter is reset back to 0, it's only 4 bit, but since each cycle takes 8 counts,
		//, it goes 0 8 16 32, and when BAM_counter hits 64 we reset the BAM
		if (BAM_Counter == 120) {
			BAM_Counter = 0;
			BAM_Bit = 0;
		}
		break;
	}//switch_case

	SPI.transfer(anode[anodelevel]);//finally, send out the anode level byte

	PORTD |= 1 << latch_pin;//Latch pin HIGH
	PORTD &= ~(1 << latch_pin);//Latch pin LOW
	PORTD &= ~(1 << blank_pin);//Blank pin LOW to turn on the LEDs with the new data

	anodelevel++;//inrement the anode level
	level = level + 8;//increment the level variable by 8, which is used to shift out data, since the next level woudl be the next 8 bytes in the arrays

	if (anodelevel == 8)//go back to 0 if max is reached
		anodelevel = 0;
	if (level == 64)//if you hit 64 on level, this means you just sent out all 63 bytes, so go back
		level = 0;
	pinMode(blank_pin, OUTPUT);//moved down here so outputs are all off until the first call of this function
}//***MultiPlex BAM END***MultiPlex BAM END***MultiPlex BAM END***MultiPlex BAM END***MultiPlex BAM END***MultiPlex BAM END***MultiPlex BAM END

	



