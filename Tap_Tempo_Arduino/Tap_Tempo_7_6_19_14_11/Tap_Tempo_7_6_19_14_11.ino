#include "List.h"

#define BUTTON_PIN 8
#define LED_PIN 9

#define MIN_TEMPO 60.0 // BPM
#define MAX_TEMPO 150.0 // BPM

#define BPM_TO_HZ(bpm) ((bpm)/60.0)
#define BPM_TO_MSEC(bpm) (60000.0/(bpm))

List<unsigned long> tap_history(32);

unsigned long current_time;
unsigned long last_tap_time, elapsed_tap_time;
unsigned long last_blink_time, elapsed_blink_time;

int current_beat_period;

int pwm_value;
float decay_constant;

void setup()
{
  Serial.begin(115200);
  
  pinMode(BUTTON_PIN, INPUT_PULLUP);

  SetTempo(BPM_TO_MSEC(120.0));

  current_time = millis();
  last_tap_time = last_blink_time = current_time;
}

void loop()
{
  current_time = millis();
  elapsed_tap_time = current_time - last_tap_time;
  elapsed_blink_time = current_time - last_blink_time;
  
  if (digitalRead(BUTTON_PIN) == LOW && elapsed_tap_time > BPM_TO_MSEC(MAX_TEMPO))
  {
    if (elapsed_tap_time > BPM_TO_MSEC(MIN_TEMPO))
    {
      Serial.println("RESET");
      tap_history.Reset();
    }
    else
    {

      if (tap_history.GetSize() == tap_history.GetCapacity())
      {
        tap_history.Replace(elapsed_tap_time, tap_history.GetSize()-1);
      }
      else
      {
        tap_history.Insert(elapsed_tap_time);
      }

      float new_beat_period = 0;
      float weight = 2.0 / (tap_history.GetSize() * (tap_history.GetSize()+1));
      for (int index = tap_history.GetSize()-1; index >= 0; index--)
      {
        new_beat_period += (index+1) * weight * tap_history.Get(index);
      }

//      float new_beat_period = 0;
//      float alpha = 2.0 / (tap_history.GetSize() + 1.0);
//      float weight = alpha;
//      
//      for (int index = tap_history.GetSize()-1; index >= 0; index--)
//      {
//        new_beat_period += weight * tap_history.Get(index);
//        weight *= (1.0 - alpha);
//      }
      
      SetTempo(new_beat_period);

      last_blink_time = current_time;
    }    

    last_tap_time = current_time;
  }

  if (elapsed_blink_time >= current_beat_period)
  {
//    Serial.println(elapsed_blink_time);
    last_blink_time = current_time;
  }

  ShowTempo();
}

void SetTempo(int new_beat_period)
{
  if (new_beat_period > 0)
  {
    current_beat_period = new_beat_period;
    decay_constant = -2.0 / current_beat_period;

    Serial.println(60000.0 / current_beat_period);
  }
}

void ShowTempo()
{
  pwm_value = (int) max((255*pow(255, decay_constant*elapsed_blink_time)), 0.0);
  analogWrite(LED_PIN, pwm_value);
}
