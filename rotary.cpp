/* 
 * 
 * Rotary for Arduino TinyTetris
 * Pinout:
 *    A1: Channel A
 *    A0: Channel B
 *    A2: clic / push
 * 
 * Designed for https://github.com/AJRussell/Tiny-Tetris
 * 
 * Clumsy code by tobozo (c+) 2017
 * Apologies for coding horror
 * 
 */
 
#ifndef ROTARYCPP
#define ROTARYCPP

#include <Arduino.h>
#include <ClickEncoder.h> // https://github.com/0xPIT/encoder also available in the Library Manager
#include <TimerOne.h> // https://github.com/PaulStoffregen/TimerOne also available in the Library Manager

static int rotarypos = -1;

static volatile int Debounce = 0;
static volatile bool processKey = true;
static volatile int currentPos;

/* encoder vars */
static boolean BtnClick = false; // Encoder button was clicked
static int16_t last, value; // Encoder values cache

static ClickEncoder *encoder;


class Dpad {

  static const int DebounceMax = 10;
  
  public:

    static void init() {
      // for compat

      Timer1.initialize(1000); // rotary timer
      Timer1.attachInterrupt(timerIsr);
      
      last = encoder->getValue();
      encoder = new ClickEncoder(A1, A0, A2);
    }

    static void timerIsr() {
      encoder->service();
    }

    static int getPos() {
      if(rotarypos!=-1) {
        return rotarypos;
      }
      
      BtnClick = false;
      ClickEncoder::Button b = encoder->getButton();
      
      if (b == ClickEncoder::Clicked) {
        BtnClick = true;
        rotarypos = 4;//KEY_ROTATE;
      } else {
        value += encoder->getValue();
        if (value / 2 > last) {
          last = value / 2;
          rotarypos = 2;//KEY_RIGHT;
        }
        if (value / 2 < last) {
          last = value / 2;
          rotarypos = 1;//KEY_LEFT;
        }
      }
      return rotarypos;
    }

    static boolean DoDebounce() {
      rotarypos = -1;
      return false;
    }

    static int setAccel(int acceleration, int offset) {
      // kept for compat
      return acceleration;
    }
};

#endif
