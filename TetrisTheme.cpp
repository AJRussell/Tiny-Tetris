/* 
 * 
 * TetrisTheme for Arduino Tetris 
 * 
 * Based on the code from https://github.com/lukecyca/TetrisThemeArduino
 * Designed for https://github.com/vilaca/Handheld-Color-Console/tree/master/ArduinoTetris
 * 
 * Clumsy code by tobozo (c+) 2016
 * Apologies for coding horror
 * 
 */

#ifndef TETRISTHEMECPP
#define TETRISTHEMECPP

#include <Arduino.h>

#ifndef PIEZO_PIN
#define PIEZO_PIN  (3)
#endif

// A rest
#define _R     (0)

// Note frequencies based on http://www.phy.mtu.edu/~suits/notefreqs.html
#define _C0    (16.35)
#define _CS0   (17.32)
#define _D0    (18.35)
#define _DS0   (19.45)
#define _E0    (20.60)
#define _F0    (21.83)
#define _FS0   (23.12)
#define _G0    (24.50)
#define _GS0   (25.96)
#define _A0    (27.50)
#define _AS0   (29.14)
#define _B0    (30.87)
#define _C1    (32.70)
#define _CS1   (34.65)
#define _D1    (36.71)
#define _DS1   (38.89)
#define _E1    (41.20)
#define _F1    (43.65)
#define _FS1   (46.25)
#define _G1    (49.00)
#define _GS1   (51.91)
#define _A1    (55.00)
#define _AS1   (58.27)
#define _B1    (61.74)
#define _C2    (65.41)
#define _CS2   (69.30)
#define _D2    (73.42)
#define _DS2   (77.78)
#define _E2    (82.41)
#define _F2    (87.31)
#define _FS2   (92.50)
#define _G2    (98.00)
#define _GS2   (103.83)
#define _A2    (110.00)
#define _AS2   (116.54)
#define _B2    (123.47)
#define _C3    (130.81)
#define _CS3   (138.59)
#define _D3    (146.83)
#define _DS3   (155.56)
#define _E3    (164.81)
#define _F3    (174.61)
#define _FS3   (185.00)
#define _G3    (196.00)
#define _GS3   (207.65)
#define _A3    (220.00)
#define _AS3   (233.08)
#define _B3    (246.94)
#define _C4    (261.63)
#define _CS4   (277.18)
#define _D4    (293.66)
#define _DS4   (311.13)
#define _E4    (329.63)
#define _F4    (349.23)
#define _FS4   (369.99)
#define _G4    (392.00)
#define _GS4   (415.30)
#define _A4    (440.00)
#define _AS4   (466.16)
#define _B4    (493.88)
#define _C5    (523.25)
#define _CS5   (554.37)
#define _D5    (587.33)
#define _DS5   (622.25)
#define _E5    (659.25)
#define _F5    (698.46)
#define _FS5   (739.99)
#define _G5    (783.99)
#define _GS5   (830.61)
#define _A5    (880.00)
#define _AS5   (932.33)
#define _B5    (987.77)
#define _C6    (1046.50)
#define _CS6   (1108.73)
#define _D6    (1174.66)
#define _DS6   (1244.51)
#define _E6    (1318.51)
#define _F6    (1396.91)
#define _FS6   (1479.98)
#define _G6    (1567.98)
#define _GS6   (1661.22)
#define _A6    (1760.00)
#define _AS6   (1864.66)
#define _B6    (1975.53)
#define _C7    (2093.00)
#define _CS7   (2217.46)
#define _D7    (2349.32)
#define _DS7   (2489.02)
#define _E7    (2637.02)
#define _F7    (2793.83)
#define _FS7   (2959.96)
#define _G7    (3135.96)
#define _GS7   (3322.44)
#define _A7    (3520.00)
#define _AS7   (3729.31)
#define _B7    (3951.07)
#define _C8    (4186.01)
#define _CS8   (4434.92)
#define _D8    (4698.63)
#define _DS8   (4978.03)
#define _E8    (5274.04)
#define _F8    (5587.65)
#define _FS8   (5919.91)
#define _G8    (6271.93)
#define _GS8   (6644.88)
#define _A8    (7040.00)
#define _AS8   (7458.62)
#define _B8    (7902.13)

// beats per minute
#define BPM   (120.0)
// Time (in microseconds) to spend on each note while simulating polyphony
// If this is too small, low frequency notes will be inaudible.
#define POLY_DELTA (14400)
 

static const float lead_notes[] = {
  // part 1
  _E5, _B4, _C5, _D5, _C5, _B4, _A4, _A4, _C5, _E5, _D5, _C5, _B4, _B4, _C5, _D5, _E5, _C5, _A4, _A4, _R,
  _D5, _F5, _A5, _G5, _F5, _E5, _C5, _E5, _D5, _C5, _B4, _B4, _C5, _D5, _E5, _C5, _A4, _A4, _R,

  // part 2
  _E4, _C4, _D4, _B3, _C4, _A3, _GS3, _B3,
  _E4, _C4, _D4, _B3, _C4, _E4, _A4, _A4, _GS4, _R

};

static const float lead_times[] = {
  // part 1
  1.0, 0.5, 0.5, 1.0, 0.5, 0.5, 1.0, 0.5, 0.5, 1.0, 0.5, 0.5, 1.0, 0.5, 0.5, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0,
  1.5, 0.5, 1.0, 0.5, 0.5, 1.5, 0.5, 1.0, 0.5, 0.5, 1.0, 0.5, 0.5, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0,

  // part 2
  2.0, 2.0, 2.0, 2.0, 2.0, 2.0, 2.0, 2.0,
  2.0, 2.0, 2.0, 2.0, 1.0, 1.0, 1.0, 1.0, 3.0, 1.0

};

static const float bass_notes[] = {
  // part 1
  _E2, _E3, _E2, _E3, _E2, _E3, _E2, _E3, _A1, _A2, _A1, _A2, _A1, _A2, _A1, _A2, _GS1, _GS2, _GS1, _GS2, _GS1, _GS2, _GS1, _GS2, _A1, _A2, _A1, _A2, _A1, _B2, _C3, _E3,
  _D2, _D3, _D2, _D3, _D2, _D3, _D2, _D3, _C2, _C3, _C2, _C3, _C2, _C3, _C2, _C3, _B1, _B2, _B1, _B2, _B1, _B2, _B1, _B2, _A1, _A2, _A1, _A2, _A1, _A2, _A1, _A2,

  // part 2
  _A1, _E2, _A1, _E2, _A1, _E2, _A1, _E2, _GS1, _E2, _GS1, _E2, _GS1, _E2, _GS1, _E2, _A1, _E2, _A1, _E2, _A1, _E2, _A1, _E2, _GS1, _E2, _GS1, _E2, _GS1, _E2, _GS1, _E2,
  _A1, _E2, _A1, _E2, _A1, _E2, _A1, _E2, _GS1, _E2, _GS1, _E2, _GS1, _E2, _GS1, _E2, _A1, _E2, _A1, _E2, _A1, _E2, _A1, _E2, _GS1, _E2, _GS1, _E2, _GS1, _E2, _GS1, _E2


};
/*
// all values are 0.5 so let's hardcode it and save some memory
static const float bass_times[] = {
  // part 1
  0.5,  0.5,  0.5,  0.5,  0.5,  0.5, 0.5, 0.5, 0.5, 0.5, 0.5, 0.5, 0.5, 0.5, 0.5, 0.5, 0.5, 0.5, 0.5, 0.5, 0.5, 0.5, 0.5, 0.5, 0.5, 0.5, 0.5, 0.5, 0.5, 0.5, 0.5, 0.5,
  0.5,  0.5,  0.5,  0.5,  0.5,  0.5, 0.5, 0.5, 0.5, 0.5, 0.5, 0.5, 0.5, 0.5, 0.5, 0.5, 0.5, 0.5, 0.5, 0.5, 0.5, 0.5, 0.5, 0.5, 0.5, 0.5, 0.5, 0.5, 0.5, 0.5, 0.5, 0.5,

  // part 2
  0.5,  0.5,  0.5,  0.5,  0.5,  0.5, 0.5, 0.5, 0.5, 0.5, 0.5, 0.5, 0.5, 0.5, 0.5, 0.5, 0.5, 0.5, 0.5, 0.5, 0.5, 0.5, 0.5, 0.5, 0.5, 0.5, 0.5, 0.5, 0.5, 0.5, 0.5, 0.5,
  0.5,  0.5,  0.5,  0.5,  0.5,  0.5, 0.5, 0.5, 0.5, 0.5, 0.5, 0.5, 0.5, 0.5, 0.5, 0.5, 0.5, 0.5, 0.5, 0.5, 0.5, 0.5, 0.5, 0.5, 0.5, 0.5, 0.5, 0.5, 0.5, 0.5, 0.5, 0.5

};*/
static const int lead_note_count = sizeof(lead_notes) / sizeof(float);
static const int bass_note_count = sizeof(bass_notes) / sizeof(float);

static volatile boolean songOn;

 
class TetrisTheme {
   
  public:
  
    static void init() {
      
      // enable timer, used to play music sequencer async
      TCCR1A = 0; // No options in control register A
      TCCR1B = (1 << CS10); // Set prescaler to divide by 8
      TIMSK1 = (1 << OCIE1A); // Call ISR when TCNT2 = OCRA2
      OCR1A = 0;// Set frequency of generated wave
      sei(); // Enable interrupts to generate waveform!
      songOn = false;
    }
  
    static void start() {
      init();
      songOn = true;
    }

    static void stop() {
      songOn = false;
    }

    static void tetrisThemePlay() {
      
      if (!songOn) {
        return;
      }
      
      int curr_lead_note = 0;
      int curr_bass_note = 0;
      float curr_lead_note_time_remaining = lead_times[curr_lead_note];
      float curr_bass_note_time_remaining = 0.5;// bass_times[curr_bass_note]; // hardcoded
      float lead_freq, bass_freq, note_value;
      unsigned long duration;
    
      while (curr_lead_note < lead_note_count && curr_bass_note < bass_note_count && songOn) {
        lead_freq = lead_notes[curr_lead_note];
        bass_freq = bass_notes[curr_bass_note];
        note_value = min(curr_lead_note_time_remaining, curr_bass_note_time_remaining);
        duration = note_value * 1000000 * (60.0/BPM);
    
        if (lead_freq > 0 && bass_freq > 0) {
          TetrisTheme::play_two_notes(lead_freq, bass_freq, duration);
        } else if (lead_freq > 0) {
          TetrisTheme::play_one_note(lead_freq, duration);
        } else if (bass_freq > 0) {
          TetrisTheme::play_one_note(bass_freq, duration);
        } else {
          delay( duration/1000 );
        }
    
        // Advance lead note
        curr_lead_note_time_remaining -= note_value;
        if (curr_lead_note_time_remaining < 0.001) {
          curr_lead_note++;
          curr_lead_note_time_remaining = lead_times[curr_lead_note];
        }
    
        // Advance bass note
        curr_bass_note_time_remaining -= note_value;
        if (curr_bass_note_time_remaining < 0.001) {
          curr_bass_note++;
          curr_bass_note_time_remaining = 0.5;// bass_times[curr_bass_note]; // hardcoded
        }
        
        if(digitalRead(A0) == LOW) {
          songOn = false; 
        }
      }
    }

  private:
    static void play_one_note(float frequency, unsigned long duration) {
      unsigned long period = 1000000.0/frequency;
    
      for (unsigned int cycles=duration/period; cycles>0; cycles--) {
        // half the time on
        digitalWrite(PIEZO_PIN, HIGH);
        delayMicroseconds( period/2 );
    
        // half the time off
        digitalWrite(PIEZO_PIN, LOW);
        delayMicroseconds( period/2 );
      }
    
      // If the duration wasn't a multiple of the period, delay the remainder
      delayMicroseconds(duration % period);
    }
    
    
    // duration is in microseconds
    static void play_two_notes(float freq1, float freq2, unsigned long duration) {
        for (unsigned long t=0; t<duration; t+=2*POLY_DELTA) {
          play_one_note(freq1, POLY_DELTA);
          play_one_note(freq2, POLY_DELTA);
        }
    }
   
   
};
 

#endif
