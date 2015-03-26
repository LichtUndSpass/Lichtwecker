#include "FastLED.h"

// Wie viele LEDs sind im LED Streifen?
#define NUM_LEDS 120
// An welchem Datenpin hängt der LED Streifen
#define DATA_PIN 3

CRGB leds[NUM_LEDS];

void setup() {
      FastLED.addLeds<WS2812B, DATA_PIN, GRB>(leds, NUM_LEDS);
}

uint8_t animation = 0;
uint8_t farbe = 0;

void loop() {
   for(int led = 0; led < NUM_LEDS; led++) 
   {
      leds[led] = CRGB(0x000000);    // LED ausschalten
   }
    
   if (farbe == 0) leds[animation] = CRGB(0xffffff);
   if (farbe == 1) leds[animation] = CRGB(0xff0000);
   if (farbe == 2) leds[animation] = CRGB(0x00ff00);
   if (farbe == 3) leds[animation] = CRGB(0x0000ff);
   if (farbe > 3)  farbe = 0;

   FastLED.show();
   delay(50);

   animation++;
   if (animation >= NUM_LEDS) {
     animation = 0;
     farbe++;
   }
}
