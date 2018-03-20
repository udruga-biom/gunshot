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

// sensitivity of detector (GREEN line in the serial plotter)
const int threshold = 5;
// delay after sound detection (GREEN line in the serial plotter
// how many milliseconds after a trigger should the readings be ignored?
const int eventDuration = 10; 

// this parameter changes the smoother (BLUE line in the serial plotter)
// the number of readings that you want to be included in the running average.
// bigger number = smoother graph, but less responsive
const int numReadings = 10;
// ---------------------------------------------------------------------

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
  int trigger = 0;

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

  if (value - average > threshold) {
    if (millis() - event > eventDuration) {
      event = millis();
      trigger = -50;
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

