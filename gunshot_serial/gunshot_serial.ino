#include <SD.h>

/******************************************************************************
  gunshot_serial.ino
  Gunshot detector serial debugger
  mzec @ BIOM
  2017-03-23
    code available at:
    https://github.com/udruga-biom/gunshot
  based on:
    https://github.com/sparkfun/Sound_Detector
    https://www.arduino.cc/en/tutorial/smoothing
 ******************************************************************************/

// define input pin
#define PIN_ANALOG_IN A0

// parameters, change for different output
const int numReadings = 30; // smoothing window size
const int treshold = 25; // sensitivity of detector
const int numDelay = 10; // delay between readings
const int eventDuration = 1000; // delay after sound detection

int readings[numReadings];
int readIndex = 0;
int total = 0;
int average = 0;
unsigned long event = 0;

void setup()
{
  for (int thisReading = 0; thisReading < numReadings; thisReading++) {
    readings[thisReading] = 0;
  }

  Serial.begin(9600);

  // Display status
  Serial.println("Initialized");
}

void loop()
{
  int value;
  String valueString;
  int paf = 0;

  // read the envelope input
  value = analogRead(PIN_ANALOG_IN);
  
  // smoother
  // subtract the last reading:
  total = total - readings[readIndex];
  // read from the sensor:
  readings[readIndex] = value;
  // add the reading to the total:
  total = total + readings[readIndex];
  // advance to the next position in the array:
  readIndex = readIndex + 1;

  // if we're at the end of the array...
  if (readIndex >= numReadings) {
    // ...wrap around to the beginning:
    readIndex = 0;
  }

  // calculate the average:
  average = total / numReadings;

  if (value - average > treshold) {
    if (millis() - event > eventDuration) {
      event = millis();
      paf = -50;
    }

  }
  Serial.print(average);
  Serial.print(" ");
  Serial.print(value);
  Serial.print(" ");
  Serial.println(paf);

  // pause
  delay(numDelay);
}

