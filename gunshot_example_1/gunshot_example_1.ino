#include <SD.h>

/******************************************************************************
  gunshot_serial.ino
  Gunshot detector serial debugger
  mzec @ BIOM
  2018-03-19
    code available at:
    https://github.com/udruga-biom/gunshot
  based on:
    https://github.com/sparkfun/Sound_Detector
    https://www.arduino.cc/en/tutorial/smoothing
 ******************************************************************************/

// define input pin
#define PIN_ANALOG_IN A0

// ---------------------------------------------------------------------
// FEEL FREE TO CHANGE AND PLAY AROUND WITH THE FOLLOWING PARAMETERS:
// ---------------------------------------------------------------------

// delay between readings in milliseconds
// smaller number - more readings, more sensitivity
const int numDelay = 10;
// ---------------------------------------------------------------------

int readIndex = 0;
int current = 0;
int previous = 0;
unsigned long event = 0;

void setup()
{
  Serial.begin(9600);

  // Display status
  Serial.println("Initialized");
}

void loop()
{
  int value;
  String valueString;
  int paf = 0;

  previous = current;
  // read the envelope input
  current = analogRead(PIN_ANALOG_IN);
  
  if (current - previous > threshold) {
    if (millis() - event > eventDuration) {
      event = millis();
      paf = -50;
    }
  }
  // Serial.print(average);
  // Serial.print(" ");
  Serial.println(current);
  // Serial.print(" ");
  // Serial.println(paf);

  // pause
  delay(numDelay);
}

