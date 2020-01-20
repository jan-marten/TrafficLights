#include <Arduino.h>

// Connection configuration
#define LED_RED 8
#define LED_ORANGE 9
#define LED_GREEN 10
#define BUZZER 12

#define TILT_SWITCH 6
#define CROSSING_SWITCH 7

#define MAX_COUNTER 0x20

unsigned int _loopCounter = MAX_COUNTER; // after 0xFFFF cycles the orange light will start flashing until button is pressed
unsigned int _lightStateOrangeCounter = 0;
bool _lightStateOrangeValue = false;

void setup() {
  Serial.begin(115200);
  Serial.println("Setup");
  
  pinMode(LED_BUILTIN, OUTPUT);
  pinMode(LED_RED, OUTPUT);
  pinMode(LED_ORANGE, OUTPUT);
  pinMode(LED_GREEN, OUTPUT);
  //pinMode(BUZZER, OUTPUT);
  
  //pinMode(TILT_SWITCH, INPUT);
  //pinMode(RESET_SWITCH, INPUT);

  pinMode(CROSSING_SWITCH, INPUT);
  
}

void SetLeds(byte led, bool enabled){
  digitalWrite(LED_RED, enabled && LED_RED == led ? HIGH : LOW);
  digitalWrite(LED_ORANGE, enabled && LED_ORANGE == led ? HIGH : LOW);
  digitalWrite(LED_GREEN, enabled && LED_GREEN == led ? HIGH : LOW);
}

void loop() {
  Serial.print("{");
  
  if (_loopCounter < MAX_COUNTER) _loopCounter++;

  if (digitalRead(CROSSING_SWITCH) == LOW)
  {
    Serial.print("x");
    digitalWrite(LED_BUILTIN, LOW);
    
    _loopCounter = 0;
    _lightStateOrangeCounter = 0;
    _lightStateOrangeValue = false;
    
    // Actuate a crossing sequence
    SetLeds(LED_RED, true);
    delay(1000);
    
    SetLeds(LED_GREEN, true);
    delay(3000);
    
    SetLeds(LED_ORANGE, true);
    delay(1000);
    
    SetLeds(LED_RED, true);
  }
  else
  {
    Serial.print("X");
    digitalWrite(LED_BUILTIN, HIGH);
  }

  if (_loopCounter == MAX_COUNTER)
  {
    Serial.print("LC");
    
    // actuate the orange flasher
    _lightStateOrangeCounter++;
    if (_lightStateOrangeCounter == MAX_COUNTER >> 2)
    {
      _lightStateOrangeCounter = 0;  
      _lightStateOrangeValue = !_lightStateOrangeValue; // toggle
      SetLeds(LED_ORANGE, _lightStateOrangeValue);
      Serial.print("[toggle]");
    }
  }

  Serial.println("}");

  delay(50);
}




