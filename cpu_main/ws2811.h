#pragma once
#include "defines.h"
#include <FastLED.h>

#define LED_PIN     22
#define NUM_LEDS    30
#define LED_TYPE    WS2811
#define COLOR_ORDER RGB

struct CRGB leds[NUM_LEDS];

	void ws2811update()
	{
	}

	void ws2811init()
	{
		FastLED.addLeds<LED_TYPE, LED_PIN, COLOR_ORDER>(leds, NUM_LEDS);
		FastLED.setBrightness(255);
	}

	void ws2811on_boot()
	{
		for (int i = 0; i<NUM_LEDS; i++) {
			leds[i] = CRGB::Red;
			FastLED.show();
		}
		FastLED.delay(1000);
		for (int i = 0; i<NUM_LEDS; i++) {
			leds[i] = CRGB::Green;
			FastLED.show();
		}
		FastLED.delay(1000);
		for (int i = 0; i<NUM_LEDS; i++) {
			leds[i] = CRGB::Blue;
			FastLED.show();
		}
		FastLED.delay(1000);
		for (int i = 0; i<NUM_LEDS; i++) {
			leds[i] = CRGB::White;
			FastLED.show();
		}
		FastLED.delay(1000);
		for (int i = 0; i<NUM_LEDS; i++) {
			leds[i] = CRGB::Black;
			FastLED.show();
		}
		delay(500);
	}
