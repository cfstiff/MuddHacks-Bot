#include <SD.h>
#include "fallLeft1.h"
#include "fallLeft2.h"
#include "fallOnHead1.h"
#include "fallOnHead2.h"
#include "fallOnHead3.h"
#include "fallRight1.h"
#include "fallRight2.h"
#include "sleep1.h"
#include "sleep2.h"
#include "sleep3.h"
#include "nom1.h"
#include "nom2.h"
#include "nom3.h"
#include "nom4.h"
#include "nom5.h"
#include "sleepnoz.h"
#include "sleep1z.h"
#include "sleep2z.h"
#include <Time.h>

/***************************************************
  This is a example sketch demonstrating graphic drawing
  capabilities of the SSD1351 library for the 1.5"
  and 1.27" 16-bit Color OLEDs with SSD1351 driver chip
  Pick one up today in the adafruit shop!
  ------> http://www.adafruit.com/products/1431
  ------> http://www.adafruit.com/products/1673

  If you're using a 1.27" OLED, change SSD1351HEIGHT in Adafruit_SSD1351.h
   to 96 instead of 128
  These display s use SPI to communicate, 4 or 5 pins are required to
  interface
  Adafruit invests time and resources providing this open source code,
  please support Adafruit and open-source hardware by purchasing
  products from Adafruit!
  Written by Limor Fried/Ladyada for Adafruit Industries.
  BSD license, all text above must be included in any redistribution
  The Adafruit GFX Graphics core library is also required
  https://github.com/adafruit/Adafruit-GFX-Library
  Be sure to install it!
 ****************************************************/

// You can use any (4 or) 5 pins
#define sclk 0
#define mosi 1
#define dc   8
#define cs   9
#define rst  7

// Color definitions
#define BLACK           0x0000
#define BLUE            0x001F
#define RED             0xF800
#define GREEN           0x07E0
#define CYAN            0x07FF
#define MAGENTA         0xF81F
#define YELLOW          0xFFE0
#define WHITE           0xFFFF

#define BACKGROUND_COLOR WHITE

#include <Adafruit_GFX.h>
#include <Adafruit_SSD1351.h>
#include <SPI.h>
#include <avr/pgmspace.h>
#include "restingImages.h"
#include <Wire.h>

// Option 2: must use the hardware SPI pins
// (for UNO thats sclk = 13 and sid = 11) and pin 10 must be
// an output. This is much faster - also required if you want
// to use the microSD card (see the image drawing example)
Adafruit_SSD1351 tft = Adafruit_SSD1351(cs, dc, rst);

const int MPU_addr = 0x68; // I2C address of the MPU-6050
int16_t AcX, AcY, AcZ, Tmp, GyX, GyY, GyZ;
int16_t absX, absY, absZ;
int initSleep = 0;
int finishSleep = 0;
#define SLEEP_CONST 5
#define MAX_HEALTH 100
boolean sleep = false;
boolean prevSleep = false;
int health = MAX_HEALTH;
void setup(void) {
  Serial.begin(9600);
  pinMode(cs, OUTPUT);
  digitalWrite(cs, HIGH);
  tft.begin();
  tft.fillScreen(BLACK);
  Wire.begin();
  Wire.beginTransmission(MPU_addr);
  Wire.write(0x6B);  // PWR_MGMT_1 register
  Wire.write(0);     // set to zero (wakes up the MPU-6050)
  Wire.endTransmission(true);
  pinMode(A1, INPUT);
  pinMode(13, OUTPUT);
  pinMode(4, OUTPUT);
  pinMode(5, OUTPUT);
  pinMode(6, OUTPUT);


}

void loop() {
  if (health == 0) {
    tft.setCursor(36, 36);
    tft.setTextColor(MAGENTA);
    tft.setTextSize(1);
    tft.setTextWrap(true);
    tft.print("Your buddy has died. Get some sleep!");
    return;
  }

  while (isNight()) {
    if (prevSleep == false) {
      prevSleep = true;
      initSleep = millis();

      //Displays animation
      tft.drawBitmap(0, 0, sleep1, 128, 128, BACKGROUND_COLOR);
      delay(100);
      tft.drawBitmap(0, 0, sleep2, 128, 128, BACKGROUND_COLOR);
      delay(100);
      //tft.drawBitmap(0, 0, sleep2Inverse, 128, 128, BACKGROUND_COLOR);
      delay(100);
      tft.drawBitmap(0, 0, sleep3, 128, 128, BACKGROUND_COLOR);
      delay(100);
      //tft.drawBitmap(0, 0, sleep3Inverse, 128, 128, BACKGROUND_COLOR);
      delay(100);
      tft.fillScreen(BLACK);
      tft.drawBitmap(0, 0, sleepNoZ, 128, 128, BACKGROUND_COLOR);
    }
    else {
      tft.drawBitmap(0, 0, sleepNoZ, 128, 128, BACKGROUND_COLOR);

    }
  }
  while (!isNight()) {
    if (health == 0) {
      tft.fillScreen(BLACK);
      tft.setCursor(36, 36);
      tft.setTextColor(MAGENTA);
      tft.setTextSize(1);
      tft.setTextWrap(true);
      tft.print("Your buddy has died. Get some sleep!");
      return;
    }
    else if (prevSleep == true) {
      prevSleep = false;
      finishSleep = millis();
      resting();
      int hoursSlept = (finishSleep - initSleep) / 60000;
      if  (hoursSlept < 1) {
        health = 0;
      }
      else {
        int minCompare = health + hoursSlept * SLEEP_CONST;
        health = min(minCompare, MAX_HEALTH);
      }
    }
    else {
      resting();
      feed();
      //rotateBox();
    }
  }
  tft.fillScreen(BLACK);

}

void resting() {
  tft.drawBitmap(0, 0, restingInverse4, 128, 128, BLACK);
  tft.drawBitmap(0, 0, resting1, 128, 128, BACKGROUND_COLOR);
  delay(100);
  tft.drawBitmap(0, 0, restingInverse1, 128, 128, BLACK);
  tft.drawBitmap(0, 0, resting2, 128, 128, BACKGROUND_COLOR);
  delay(100);
  tft.drawBitmap(0, 0, restingInverse2, 128, 128, BLACK);
  tft.drawBitmap(0, 0, resting3, 128, 128, BACKGROUND_COLOR);
  delay(100);
  tft.drawBitmap(0, 0, restingInverse3, 128, 128, BLACK);
  tft.drawBitmap(0, 0, resting4, 128, 128, BACKGROUND_COLOR);
  delay(100);




}


void rotateBox() {
  tft.fillScreen(BLACK);
  Wire.beginTransmission(MPU_addr);
  Wire.write(0x3B);  // starting with register 0x3B (ACCEL_XOUT_H)
  Wire.endTransmission(false);
  Wire.requestFrom(MPU_addr, 14, true); // request a total of 14 registers
  AcX = Wire.read() << 8 | Wire.read(); // 0x3B (ACCEL_XOUT_H) & 0x3C (ACCEL_XOUT_L)
  AcY = Wire.read() << 8 | Wire.read(); // 0x3D (ACCEL_YOUT_H) & 0x3E (ACCEL_YOUT_L)
  AcZ = Wire.read() << 8 | Wire.read(); // 0x3F (ACCEL_ZOUT_H) & 0x40 (ACCEL_ZOUT_L)
  Tmp = Wire.read() << 8 | Wire.read(); // 0x41 (TEMP_OUT_H) & 0x42 (TEMP_OUT_L)
  GyX = Wire.read() << 8 | Wire.read(); // 0x43 (GYRO_XOUT_H) & 0x44 (GYRO_XOUT_L)
  GyY = Wire.read() << 8 | Wire.read(); // 0x45 (GYRO_YOUT_H) & 0x46 (GYRO_YOUT_L)
  GyZ = Wire.read() << 8 | Wire.read(); // 0x47 (GYRO_ZOUT_H) & 0x48 (GYRO_ZOUT_L)
  Serial.print("AcX = "); Serial.print(AcX);
  Serial.print(" | AcY = "); Serial.print(AcY);
  Serial.print(" | AcZ = "); Serial.println(AcZ);

  absX = abs(AcX);
  absY = abs(AcY);
  absZ = abs(AcZ);

  if (absZ > absY && absZ >= absX)
  {
    if (AcZ >= 0)
    {
      tft.drawBitmap(0, 0, fallLeft2, 128, 128, BACKGROUND_COLOR);
      for (int x = 128; x >= 0; x--) {
        tft.drawFastVLine(x, 0, 128, BLACK);
      }
      tft.setRotation(3);
      tft.drawBitmap(0, 0, fallOnHead1, 128, 128, BACKGROUND_COLOR);
      delay(500);
      for (int x = 0; x <= 128; x++) {
        tft.drawFastVLine(0, x, 128, BLACK);
      }
      tft.setRotation(1);

      Serial.println("Falls Left");
    }
    else
    {
      Serial.println("Falls Right");
      tft.drawBitmap(0, 0, fallRight2, 128, 128, BACKGROUND_COLOR);
      for (int x = 0; x <= 128; x++) {
        tft.drawFastVLine(x, 0, 128, BLACK);
      }
      tft.setRotation(1);
      tft.drawBitmap(0, 0, fallOnHead1, 128, 128, BACKGROUND_COLOR);
      delay(500);
      for (int x = 128; x >= 00; x--) {
        tft.drawFastVLine(0, x, 128, BLACK);
      }
      tft.setRotation(3);
    }
  }
  else if (absX > absY && absX > absZ)
  {
    tft.fillCircle(64, 64, 32, BLACK);
    Serial.println("Ball");
  }
  else if (absY > absX && absY > absZ)
  {
    if (AcY <= 0)
    {
      Serial.println("Resting");
    }
    else
    {
      tft.setRotation(2);
      tft.drawBitmap(0, 0, fallOnHead3, 128, 128, BACKGROUND_COLOR);
      for (int x = 0; x <= 128; x++) {
        tft.drawFastVLine(0, x, 128, BLACK);
      }

      tft.drawBitmap(0, 0, fallOnHead1, 128, 128, BACKGROUND_COLOR);
      delay(500);
      for (int x = 0; x <= 128; x++) {
        tft.drawFastVLine(0, x, 128, BLACK);
      }
      Serial.println("Head");
    }
  }
  else
  {
    Serial.println("Resting");
  }








  //Serial.print(" | Tmp = "); Serial.print(Tmp/340.00+36.53);  //equation for temperature in degrees C from datasheet
  //Serial.print(" | GyX = "); Serial.print(GyX);
  //Serial.print(" | GyY = "); Serial.print(GyY);
  //Serial.print(" | GyZ = "); Serial.println(GyZ);
  delay(333);
}

void feed() {
  int buttonpress = analogRead(A1);
  if (buttonpress > 200) {
    digitalWrite(13, HIGH);
    digitalWrite(4, HIGH);
    delay(100);
    digitalWrite(4, LOW);
    digitalWrite(5, HIGH);
    delay(100);
    digitalWrite(6, HIGH);
    digitalWrite(5, LOW);
    delay(100);
    digitalWrite(6, LOW);
    int healthNormal = health + health / MAX_HEALTH;
    health = min(healthNormal, MAX_HEALTH);
    tft.drawBitmap(0, 0, nom1, 128, 128, BACKGROUND_COLOR);
    delay(100);
    tft.drawBitmap(0, 0, nom1Inverse, 128, 128, BLACK);
    tft.drawBitmap(0, 0, nom2, 128, 128, BACKGROUND_COLOR);
    delay(100);
    tft.drawBitmap(0, 0, nom2Inverse, 128, 128, BLACK);
    tft.drawBitmap(0, 0, nom3, 128, 128, BACKGROUND_COLOR);
    delay(100);
    tft.drawBitmap(0, 0, nom3Inverse, 128, 128, BLACK);
    tft.drawBitmap(0, 0, nom4, 128, 128, BACKGROUND_COLOR);
    delay(100);
    tft.drawBitmap(0, 0, nom4Inverse, 128, 128, BLACK);
    tft.drawBitmap(0, 0, nom5, 128, 128, BACKGROUND_COLOR);
    delay(100);
    tft.drawBitmap(0, 0, nom5Inverse, 128, 128, BLACK);



  } else {
    digitalWrite(13, LOW);

  }

}

boolean isNight() {
  int lightbitch = analogRead(A3);
  // Serial.println(lightbitch);
  if (lightbitch > 800) {
    Serial.println("day");
    return false;
  }
  else {

    return true;
    Serial.println("night");
  }


}


