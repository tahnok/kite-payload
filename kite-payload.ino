#include <Adafruit_Sensor.h>
#include <Adafruit_BMP280.h>

#include <SD.h>


const int sdChipSelect = 10;
const char dataFile[] = "log.csv";

Adafruit_BMP280 altimeter; // I2C

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

}

uint32_t time = 0;

void loop() {
  float pressure = altimeter.readPressure();
  float temperature = altimeter.readTemperature();
  logData(time, pressure, temperature);
  time++;
  delay(2000);
}

void logData(uint32_t time, float pressure, float temperature) {
  File dataFile = SD.open("DATA2.CSV", FILE_WRITE);
  if (dataFile) {
    dataFile.print(time);
    dataFile.print(",");
    dataFile.print(pressure);
    dataFile.print(",");
    dataFile.print(temperature);
    dataFile.println();
    dataFile.close();
    Serial.println("done");
  }
  // if the file isn't open, pop up an error:
  else {
    error("error opening datalog.txt");
  }
}

void error(char *errorMessage) {
  Serial.println("ERROR");
  Serial.println(errorMessage);
  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, HIGH);   // turn the LED on (HIGH is the voltage level)
  while (1) {}
}
