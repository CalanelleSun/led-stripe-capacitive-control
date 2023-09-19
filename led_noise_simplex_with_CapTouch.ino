#include <FastLED.h>
#include "palettes.h"
#include <CapacitiveSensor.h>

CapacitiveSensor   cs_8_9 = CapacitiveSensor(8,9); //8-send,9-receive(cap.sensor connect here) 
CapacitiveSensor   cs_10_11 = CapacitiveSensor(10,11); //10-send,11-receive(cap.sensor connect here) 

#define NUM_LEDS 15
#define LED_PIN 5
#define BRIGHTNESS  100
#define LED_TYPE    WS2812B
#define COLOR_ORDER GRB
CRGB leds[NUM_LEDS];


uint8_t   noiseData[NUM_LEDS];
word touchtimeColor = 1000; // total1 treshhold
word touchtimeBright = 200; // total2 treshhold
byte colorSetup = 0; 
byte debounce = 1260;
byte brightControl = 100;

CRGBPalette16 paletic = nunoftheabove_gp;

uint8_t   octaveVal   = 1;
uint16_t  xVal        = 0;
int       scaleVal    = 120; //distance from the noise
uint16_t  timeVal     = 0;
uint8_t   sensorRes   = 60;

void setup() {
  delay( 500 ); // power-up safety delay
  pinMode(8,OUTPUT);
  pinMode(10,OUTPUT);
  pinMode(9,INPUT);
  pinMode(11,INPUT);
  FastLED.addLeds<LED_TYPE, LED_PIN, COLOR_ORDER>(leds, NUM_LEDS).setCorrection( TypicalLEDStrip );
  FastLED.setBrightness(  BRIGHTNESS );
  Serial.begin(115200);   
}


void loop() {

  long start = millis();
  long total1 =  cs_8_9.capacitiveSensor(sensorRes);
  long total2 =  cs_10_11.capacitiveSensor(sensorRes);

  EVERY_N_MILLISECONDS(100){
      Serial.println(total2);
    }

  if (total1  > touchtimeColor){
    switch(colorSetup){
      case 0: 
        paletic = Sunset_Real_gp;
        colorSetup++;
        Serial.print("sunset");
        Serial.print("\t");
        delay(debounce); 
        break;
      case 1: 
        paletic = rainfall_gp;
        colorSetup++;
        Serial.print("rainfall");
        Serial.print("\t");
        delay(debounce); 
        break;
      case 2: 
        paletic = Abstract_2_gp;
        colorSetup++;
        Serial.print("abstrakt2");
        Serial.print("\t");
        delay(debounce); 
        break;
      case 3: 
        paletic = Abstract_3_gp;
        colorSetup++;
        Serial.print("abstrakt3");
        Serial.print("\t");
        delay(debounce);  
        break;
      case 4: 
        paletic = purplefly_gp;
        colorSetup++;
        Serial.print("purplefly");
        Serial.print("\t"); 
        delay(debounce); 
        break;
      case 5: 
        paletic = wiki_knutux_gp;
        colorSetup++;
        Serial.print("wiki knutux");
        Serial.print("\t"); 
        delay(debounce); 
        break;
      case 6: 
        paletic = nunoftheabove_gp;
        colorSetup = 0;
        Serial.print("nunoftheabove_gp");
        Serial.print("\t"); 
        delay(debounce); 
        break;
    }
  }
  
  if (total2  > touchtimeBright){
      if (brightControl >= 250){
        brightControl = 50;
      } 
      FastLED.setBrightness(brightControl += 25);
      Serial.print("\n Brightness:");
      Serial.println(brightControl);
      delay(debounce);
      
  }


  timeVal = millis() / 30;
  EVERY_N_MILLISECONDS(15){
    xVal++;
  }

  memset(noiseData, 0, NUM_LEDS); // puts thirty(NUM_LEDS) zeros to the noiseData string
  fill_raw_noise8(noiseData, NUM_LEDS, octaveVal, xVal, scaleVal, timeVal);
  
  for (int i = 0; i < NUM_LEDS; i++) {
      leds[i] = ColorFromPalette(paletic, noiseData[i], noiseData[NUM_LEDS - i - 1]);
  }
  blur1d(leds, NUM_LEDS,172);
  FastLED.show();

  // delay(10); // arbitrary delay to limit data to serial port 
}
