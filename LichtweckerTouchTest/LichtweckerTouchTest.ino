#include <ADCTouch.h>
#include "FastLED.h"

//------------------------------------------------
// LEDs
//------------------------------------------------

// Wie viele LEDs sind im LED Streifen?
#define NUM_LEDS 120
// An welchem Datenpin hängt der LED Streifen
#define DATA_PIN 3

CRGB leds[NUM_LEDS];

//------------------------------------------------
// Touch
//------------------------------------------------

int touchRef[4];    // Referenzwerte für Touchauswertung
int touchValue[4];  // Touchwerte

// Bit Werte für Tasten
#define T_UP    1
#define T_RIGHT 2
#define T_DOWN  4
#define T_LEFT  8

int touchStatus = 0;

#define maxNoTouch 40  // groesster Wert der als nicht gedrueckt gesehen wird
#define minTouch   60  // kleinster Wert der als gedrueckt angesehen wird


void setup() {
  // Serielle Schnittstelle einschalten, um debugnachrichten senden zu können
  Serial.begin(115200);

  // LED Streifen initialisieren
  FastLED.addLeds<WS2812B, DATA_PIN, GRB>(leds, NUM_LEDS);

  // Touchtasten initialisieren
  for(int i = 0; i < 4; i++) {
    touchRef[i] = ADCTouch.read(A0+i, 500);
  }
}

uint8_t istPosition = 0;
uint8_t sollPosition = 0;

void getTouch(void) {
    // Touchtasten auswerten
  touchStatus = 0;  // Tasten zurücksetzen
  for(int i = 0; i < 4; i++) {
    // Sensorwert einlesen
    int tmp = ADCTouch.read(A0+i,4);
    // Tiefpassfilter gegen Störungen
    touchValue[i] = (7*touchValue[i] + tmp)/8;

    // Referenzwert korrigieren
    if (touchValue[i] < touchRef[i]) touchRef[i]--;
    if ((touchValue[i] > touchRef[i]) && 
      ((touchValue[i]-touchRef[i]) < maxNoTouch)) touchRef[i]++;

    // Taste gedrückt?
    if ((touchValue[i]-touchRef[i]) > minTouch) touchStatus |= _BV(i);
  }
}

void loop() {
  // Alle LEDs ausschalten
  for(int led = 0; led < NUM_LEDS; led++) 
  {
    leds[led] = CRGB(0x000000);    // LED ausschalten
  }

  leds[istPosition]    = CRGB(0xffffff);
  leds[istPosition+60] = CRGB(0xffffff);

  // Muss der LED Punkt wandern on die soll Position zu erreichen?
  if (istPosition < sollPosition) istPosition++;
  if (istPosition > sollPosition) istPosition--;
  
  // Touchtasten auslesen
  getTouch();

  // Neue Position für den LED Punkt setzen
  if (touchStatus == T_UP)    sollPosition = 15*2;
  if (touchStatus == T_RIGHT) sollPosition = 15*1;
  if (touchStatus == T_DOWN)  sollPosition = 15*0;
  if (touchStatus == T_LEFT)  sollPosition = 15*3;

  // LED anzeigen
  FastLED.show();
  delay(50);

  // Debugnachrichten ausgeben
  Serial.print(touchValue[0]-touchRef[0]);
  Serial.print("\t");
  Serial.print(touchValue[1]-touchRef[1]);
  Serial.print("\t");
  Serial.print(touchValue[2]-touchRef[2]);
  Serial.print("\t");
  Serial.print(touchValue[3]-touchRef[3]);
  if (touchStatus == T_UP)    Serial.print("\tHoch");
  if (touchStatus == T_RIGHT) Serial.print("\tRechts");
  if (touchStatus == T_DOWN)  Serial.print("\tRunter");
  if (touchStatus == T_LEFT)  Serial.print("\tLinks");
  Serial.println("");
}


