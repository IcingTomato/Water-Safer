#include <Arduino.h>
#include <Wire.h>
#include "rgb_lcd.h"
#include "grove_two_rgb_led_matrix.h"

rgb_lcd lcd;

GroveTwoRGBLedMatrixClass matrix;

const int wasteColorR = 255;
const int wasteColorG = 0;
const int wasteColorB = 0;
const int restColorR = 255;
const int restColorG = 255;
const int restColorB = 255;


const int pinSound = A0;
const int pinLed   = 7;
const int speakerPin = 3;

int thresholdValue = 450;  // determines loudness for sound sensor
const unsigned long waterUseAllowedDuration = 3000;  // how long you want to use water for before warning

bool monitoring = false;

void playTone(int tone, int duration) {
    for (long i = 0; i < duration * 1000L; i += tone * 2) {
        digitalWrite(speakerPin, HIGH);
        delayMicroseconds(tone);
        digitalWrite(speakerPin, LOW);
        delayMicroseconds(tone);
    }
}

uint64_t smile[] = {
 
  0xffff5e5e5e5effff,
  0xff5effffffff5eff,
  0x5eff5effff5eff5e,
  0x5effffffffffff5e,
  0x5eff5effff5eff5e,
  0x5effff5e5effff5e,
  0xff5effffffff5eff,
  0xffff5e5e5e5effff
};

uint64_t cry[] = {
 
  0xffff00000000ffff,
  0xff00ffffffff00ff,
  0x00ff00ffff00ff00,
  0x00ffffffffffff00,
  0x00ffff0000ffff00,
  0x00ff00ffff00ff00,
  0xff00ffffffff00ff,
  0xffff00000000ffff
};

uint64_t idle[] = {
 
  0xffff29292929ffff,
  0xff29ffffffff29ff,
  0x29ff29ffff29ff29,
  0x29ffffffffffff29,
  0x29ff29292929ff29,
  0x29ffffffffffff29,
  0xff29ffffffff29ff,
  0xffff29292929ffff
};

void setup() 
{
    pinMode(pinLed, OUTPUT);

    Wire.begin();
    // set up the LCD's number of columns and rows:
    lcd.begin(16, 2);
    
    lcd.setRGB(restColorR, restColorG, restColorB);
    lcd.setCursor(0, 0);
    lcd.print("Water conserved     ");
    delay(1000);
}

void loop() 
{
    int sensorValue = analogRead(pinSound);
    if (sensorValue > thresholdValue) {
      lcd.setCursor(0, 0);
      lcd.setRGB(255, 251, 140);
      lcd.print("Monitoring use  ");
      int i=0;
      matrix.displayFrames(smile+i*8, 200, false, 1);
      delay(waterUseAllowedDuration);
      if (analogRead(pinSound) > thresholdValue - 200) {
        lcd.setRGB(wasteColorR, wasteColorG, wasteColorB);
        lcd.setCursor(0, 0);
        lcd.print("Excess water use");
        lcd.setCursor(0, 1);
        lcd.print("Try to conserve water");
        playTone(1700, 1000);
        int i=0;
        matrix.displayFrames(cry+i*8, 200, false, 1);
        delay(5000);
        lcd.clear();
      }
    } else {
      lcd.setRGB(restColorR, restColorG, restColorB);
      lcd.setCursor(0, 0);
      lcd.print("Water conserved");
      int i=0;
      matrix.displayFrames(smile+i*8, 200, false, 1);
    //   delay(3000);
    }
}