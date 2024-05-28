
//    FILE: HX_is_ready.ino
//  AUTHOR: Rob Tillaart
// PURPOSE: HX711 demo
//     URL: https://github.com/RobTillaart/HX711


#include "HX711.h"

HX711 scale;

uint8_t dataPin = 6;
uint8_t clockPin = 7;
uint8_t BUTTON_PIN = 2;
uint8_t MOSFET = 13;  //3
bool flag = LOW;

#define SHORT_PRESS_TIME 500  // 500 milliseconds
#define LONG_PRESS_TIME 3000  // 3000 milliseconds

// Variables will change:
int lastState = LOW;  // the previous state from the input pin
int currentState;     // the current reading from the input pin
unsigned long pressedTime = 0;
unsigned long releasedTime = 0;

void setup() {
  Serial.begin(115200);
  Serial.println(__FILE__);
  // Serial.print("LIBRARY VERSION: ");
  // Serial.println(HX711_LIB_VERSION);
  Serial.println();

  scale.begin(dataPin, clockPin);

  pinMode(BUTTON_PIN, INPUT_PULLUP);
  pinMode(MOSFET, OUTPUT);
  digitalWrite(MOSFET, HIGH);  // turn the MOSFET on (HIGH is the voltage level)
  Serial.println("device on");

  // TODO find a nice solution for this calibration..
  // load cell factor 20 KG
  // scale.set_scale(127.15);

  // load cell factor 5 KG
  scale.set_scale(-415.97);  // TODO you need to calibrate this yourself. //420.0983   -417.97
  // reset the scale to zero = 0
  scale.tare(20);

  // delay(2000);
}


void loop() {
  // read the state of the switch/button:
  currentState = digitalRead(BUTTON_PIN);
  //  Serial.println(currentState);

  if (lastState == HIGH && currentState == LOW)  // button is pressed
    pressedTime = millis();
  else if (lastState == LOW && currentState == HIGH) {  // button is released
    releasedTime = millis();

    long pressDuration = releasedTime - pressedTime;

    if (pressDuration < SHORT_PRESS_TIME) {
      Serial.println("A short press is detected");
      scale.tare(20);
    }
    if (pressDuration > LONG_PRESS_TIME) {
      Serial.println("A long press is detected");
      Serial.println("device off");
      delay(5);
      digitalWrite(MOSFET, LOW);  // turn the MOSFET off (LOW is the voltage level)
      delay(500);
    }
  }

  // save the the last state
  lastState = currentState;

  if (scale.is_ready()) {
    Serial.println((int)scale.get_units(5));
  }
}


// -- END OF FILE --
