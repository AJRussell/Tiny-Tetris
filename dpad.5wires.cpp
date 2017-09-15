/* 
 * 
 * Dpad for Arduino TinyTetris 
 * Using pullup from the arduino and 5 wires
 * 
 * Designed for https://github.com/AJRussell/Tiny-Tetris
 * 
 * Clumsy code by tobozo (c+) 2016
 * Apologies for coding horror
 * 
 */
 
#ifndef DPADCPP
#define DPADCPP

#include <Arduino.h>

#ifndef KEYPAD_PIN
#define KEYPAD_PIN  A0
#endif

// this is for digital + pullup buttons
/*
  const int dpad[5] = {
    4, //KEY_MIDDLE 0
    5, //KEY_LEFT 1
    6, //KEY_RIGHT 2
    7, //KEY_DOWN 3
    8  //KEY_ROTATE 4
  };
  void setup() {
    Serial.begin(9600);
    for(byte i=1;i< 5;i++) {
      pinMode(dpad[i], INPUT);
      digitalWrite(dpad[i], HIGH);// enable pullup
      Serial.print("Enabling pullup to D"); Serial.println(dpad[i]);
    }
    
  }
  void loop() {
    for(byte i=1;i< 5;i++) {
      Serial.print("Port "); Serial.print(dpad[i]); Serial.print(" -> "); Serial.print(digitalRead(dpad[i])); Serial.print("\t");
    }
    Serial.println();
  }
 */
const int dpad[5] = {
  // do not change the order, only change the pin values
  4, //KEY_MIDDLE 0 (not used but kept for compatibility with other dpad implementations)
  8, //KEY_LEFT 1
  6, //KEY_RIGHT 2
  5, //KEY_DOWN 3
  7  //KEY_ROTATE 4
};

static int dpadwarp[5] = { 0,0,0,0,0 };
static volatile int Debounce = 0;
static volatile bool processKey = true;
static volatile int currentPos;

class Dpad
{

  static const int DebounceMax = 10;
  
  public:

    static void init() {
      for(byte i=1;i< 5;i++) {
        pinMode(dpad[i], INPUT);
        digitalWrite(dpad[i], HIGH);// enable pullup
      }
    }
    
    static int getPos() {
      delay(100);
      for(byte i=1;i< 5;i++) {
        int p = digitalRead(dpad[i]);
        if (p == LOW) {
          return i;
        }
      }
      return -1;
    }

    static boolean DoDebounce() {
      Debounce++;
      if(Debounce > DebounceMax) {
        return true;
      }
      return false;
    }

    static int setAccel(int acceleration, int offset) {
      if(processKey) {
        dpadwarp[currentPos] = millis();
      }
      if(millis() < dpadwarp[currentPos] + offset) {
        processKey = false;
      } else {
        processKey = true;
        acceleration = acceleration + 70;
        if (acceleration > offset) {
          acceleration = offset;
        }
      }
      return acceleration;
    }

};

#endif



 
