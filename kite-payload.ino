#include <Adafruit_Sensor.h>
#include <Adafruit_BMP280.h>

#include <SD.h>


const int sdChipSelect = 10;
const char dataFile[] = "log.csv";

Adafruit_BMP280 altimeter; // I2C
char filename[13];
uint32_t time = 0;

void setup() {
  Serial.begin(9600);


  if (!SD.begin(sdChipSelect)) {
    error("Card failed, or not present");
  }
  Serial.println("card initialized.");

  if (!altimeter.begin()) {
    error("Could not find a valid BMP280 sensor");
  }
  Serial.println("altimeter started");

  findFilename(filename);
  Serial.print("writing data to ");
  Serial.println(filename);

}

void loop() {
  float pressure = altimeter.readPressure();
  float temperature = altimeter.readTemperature();
  logData(time, pressure, temperature);
  time++;
  delay(2000);
}

void logData(uint32_t time, float pressure, float temperature) {
  File dataFile = SD.open(filename, FILE_WRITE);
  if (!dataFile) {
    error("error opening datalog.txt");
  }

  dataFile.print(time);
  dataFile.print(",");
  dataFile.print(pressure);
  dataFile.print(",");
  dataFile.print(temperature);
  dataFile.println();
  dataFile.close();
  Serial.println("done");

}

void error(char *errorMessage) {
  Serial.println("ERROR");
  Serial.println(errorMessage);
  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, HIGH);   // turn the LED on (HIGH is the voltage level)
  while (1) {}
}

/*
   Populates buffer with filename
   in the format FLIGHTXX.CSV
   where XX is a number between 00 and 99

   this method looks through the root dir of the SD card for
   any existing files in that format and returns XX + 1
*/
void findFilename(char* filename) {
  strcpy(filename, "FLIGHT00.csv");
  for (uint8_t i = 0; i < 100; i++) {
    uint8_t offset = 6 ;
    if (i < 10) {
      offset = 7;
    }
    itoa(i, filename + offset * sizeof(char), 10);

    filename[8] = '.'; //clear null termination from itoa
    if (!(SD.exists(filename))) {
      break;
    }
  }
}

