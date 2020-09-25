#include <Arduino.h>
#include <EasyButton.h>
#include <Adafruit_NeoPixel.h>

#define LED_PIN   0
#define RELAY_PIN 1
#define BUTTON_PIN 2

const unsigned long powerOnTime = 10 * 60 * 1000;

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
  button.onPressed(buttonPressed);
  button.onSequence(2, 900, doubleClick);
  button.enableInterrupt(buttonISR);
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
    led.show();
  }
  if (timerStart!=0 && millis()-timerStart > powerOnTime){
    timerStart = 0;
    powerOn = false;
  }
}


void updateLED(unsigned long timeLeft){
    long pixelHue = map(timeLeft, 0, powerOnTime, 0, 50000);
    led.setPixelColor(0, led.gamma32(led.ColorHSV(pixelHue)));
    led.show();
}

void buttonPressed()
{
  powerOn = true;
  timerStart = millis();
}

void doubleClick()
{
  powerOn = false;
  timerStart = 0;
}

void buttonISR()
{
  button.read();
}
