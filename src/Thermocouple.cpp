/*!
 * @file Thermocouple.cpp
 *
 * This file contains the functionality to print Thermocouple readings
 * intended for Beach Launch Team's igniter testing. Based off adafruit's example code.
 * It is designed specifically to work with theAdafruit MAX31856 breakout.
 *
 * This sensor uses SPI to communicate. 4 pins are required to
 * interface with the breakout.
 *
 */

#include <Adafruit_MAX31856.h>
#include <SD.h>
#include <SPI.h>

#define DRDY_PIN 5

// change to desired number of samples per second
#define SAMPLES_PER_SECOND 10 

// SD CARD CS
const int chipSelect = BUILTIN_SDCARD;
const char* fileName = "thisisatest.csv";
unsigned long currTime = 0;
unsigned long prevTime = 0;
unsigned long initialTime = 0;
void sdWrite(unsigned long currTime);
void sdInit();
void maxInit();
// Use software SPI: CS, DI, DO, CLK
// Adafruit_MAX31856 maxthermo = Adafruit_MAX31856(10, 11, 12, 13);
// use hardware SPI, just pass in the CS pin
Adafruit_MAX31856 maxthermo = Adafruit_MAX31856(10);

void setup() {
  // Serial.begin(115200);
  // while (!Serial) delay(10);
  maxInit();
  sdInit();
  // continuous mode
  maxthermo.setConversionMode(MAX31856_CONTINUOUS);
  currTime = millis();
  initialTime = currTime;
}

void loop() {
  if(currTime - prevTime >= 1000.00/SAMPLES_PER_SECOND){
    // Serial.print((currTime-initialTime)/1000.00);
    // Serial.print(" ");
    // Serial.println(maxthermo.readThermocoupleTemperature());
    sdWrite(currTime);
    prevTime = currTime;
  }
  currTime = millis();
}

void maxInit(){
  // Serial.println("MAX31856 thermocouple test");

  pinMode(DRDY_PIN, INPUT);

  if (!maxthermo.begin()) {
    // Serial.println("Could not initialize thermocouple.");
    while (1) delay(10);
  }

  // set type j thermocouple
  maxthermo.setThermocoupleType(MAX31856_TCTYPE_J);

  // Serial.print("Thermocouple type: ");
  // switch (maxthermo.getThermocoupleType() ) {
  //   case MAX31856_TCTYPE_B: Serial.println("B Type"); break;
  //   case MAX31856_TCTYPE_E: Serial.println("E Type"); break;
  //   case MAX31856_TCTYPE_J: Serial.println("J Type"); break;
  //   case MAX31856_TCTYPE_K: Serial.println("K Type"); break;
  //   case MAX31856_TCTYPE_N: Serial.println("N Type"); break;
  //   case MAX31856_TCTYPE_R: Serial.println("R Type"); break;
  //   case MAX31856_TCTYPE_S: Serial.println("S Type"); break;
  //   case MAX31856_TCTYPE_T: Serial.println("T Type"); break;
  //   case MAX31856_VMODE_G8: Serial.println("Voltage x8 Gain mode"); break;
  //   case MAX31856_VMODE_G32: Serial.println("Voltage x8 Gain mode"); break;
  //   default: Serial.println("Unknown"); break;
  // }
}

void sdInit() { 
  SD.begin(chipSelect);
// Serial.print("Initializing SD card...");

//   // see if the card is present and can be initialized:
//   if (!SD.begin(chipSelect)) {
//     Serial.println("Card failed, or not present");
//     while (1) {
//       // No SD card, so don't do anything more - stay stuck here
//     }
//   }
//   Serial.println("card initialized.");
}

void sdWrite(unsigned long currTime) {
  String datastring = "";
  datastring += String((currTime-initialTime)/1000.00);
  datastring += ",";
  datastring += String(maxthermo.readThermocoupleTemperature());  

  File dataFile = SD.open(fileName, FILE_WRITE);
  
  if (dataFile) {
    dataFile.println(datastring);
    dataFile.close();  
    // Serial.println(datastring);
  }else {
    // Serial.println(fileName);
  }
}