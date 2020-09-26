#include <Arduino.h>
#include <EasyButton.h>
#include <Adafruit_NeoPixel.h>
#ifdef __AVR__
 #include <avr/power.h>
#endif

#define LED_PIN   0
#define RELAY_PIN 1
#define BUTTON_PIN 2

const unsigned long powerOnTime = 600000;

bool powerOn = false;
unsigned long timerStart = 0;

Adafruit_NeoPixel led(1, LED_PIN, NEO_GRBW + NEO_KHZ800);
EasyButton button(BUTTON_PIN);

void setup() {
#if defined(__AVR_ATtiny85__) && (F_CPU == 16000000)
  clock_prescale_set(clock_div_1);
#endif
  pinMode(RELAY_PIN, OUTPUT);
  digitalWrite(RELAY_PIN, HIGH);
  button.begin();
  button.onPressed(startOnTime);
  button.onSequence(2, 900, turnOff);
  button.enableInterrupt(buttonInterrupt);
  led.begin();
  led.show();
  led.setBrightness(255);
}


void loop() {
  if (powerOn){
    updateLED(millis() - timerStart);
    digitalWrite(RELAY_PIN, LOW);
  }
  else {
    digitalWrite(RELAY_PIN, HIGH);
    led.setPixelColor(0, 0, 0, 0);
  }
  led.show();
  if (timerStart!=0 && millis()-timerStart > powerOnTime) turnOff();
}


void updateLED(unsigned long timeLeft){
    unsigned long color = timeLeft/(powerOnTime/50000);
    led.setPixelColor(0, led.gamma32(led.ColorHSV(color)));
}

void startOnTime()
{
  powerOn = true;
  timerStart = millis();
}

void turnOff()
{
  powerOn = false;
  timerStart = 0;
}

void buttonInterrupt()
{
  button.read();
}
