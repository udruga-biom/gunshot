#include <SD.h>

/******************************************************************************
  gunshot.ino
  Gunshot detector
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
const int numReadings = 25; // smoothing window size
const int treshold = 25; // sensitivity of detector
const int numDelay = 10; // delay between readings
const int eventDuration = 1000; // delay after sound detection

const uint8_t BUFFER_SIZE = 25;
char fileName[] = "demoFile.txt"; // SD library only supports up to 8.3 names
char buff[BUFFER_SIZE] = "";  // Added two to allow a 2 char peek for EOF state
uint8_t index = 0;

const uint8_t chipSelect = 8;
const uint8_t cardDetect = 9;

enum states : uint8_t { NORMAL, E, EO };
uint8_t state = NORMAL;

bool alreadyBegan = false;  // SD.begin() misbehaves if not first call

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

  //  Configure LED pin as output
  pinMode(PIN_LED_OUT, OUTPUT);

  // Display status
  Serial.println("Initialized");

  // Setup SD card
  pinMode(cardDetect, INPUT);
  Serial.print("Initializing SD card...");

  // see if the card is present and can be initialized:
  if (!SD.begin(chipSelect)) {
    Serial.println("Card failed, or not present");
    // don't do anything more:
    return;
  }
  Serial.println("card initialized.");
}

void loop()
{
  int value;
  String valueString;

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
      valueString = String(value) + " " + String(millis());
      sendToSD(valueString);
    }

  }
//  Serial.print(average);
//  Serial.print(" ");
//  Serial.println(value);

  if (millis() - event < eventDuration) {
    digitalWrite(PIN_LED_OUT, HIGH);
  } else {
    digitalWrite(PIN_LED_OUT, LOW);
  }
  
  // pause
  delay(numDelay);
}

void sendToSD(String outputString)
{
  File dataFile = SD.open("datalog.txt", FILE_WRITE);
  if (dataFile) {
    dataFile.println(outputString);
    dataFile.close();
  }
  // if the file isn't open, pop up an error:
  else {
    Serial.println("error opening datalog.txt");
  }
}

