#ifndef ARDPRINTF
#define ARDPRINTF
#define ARDBUFFER 16
#include <stdarg.h>
#include <Arduino.h>

int ardprintf(char *str, ...) {
  int i, count = 0, j = 0, flag = 0;
  char temp[ARDBUFFER + 1];
  for (i = 0; str[i] != '\0'; i++)
    if (str[i] == '%') count++;

  va_list argv;
  va_start(argv, count);
  for (i = 0, j = 0; str[i] != '\0'; i++) {
    if (str[i] == '%') {
      temp[j] = '\0';
      Serial.print(temp);
      j = 0;
      temp[0] = '\0';

      switch (str[++i]) {
        case 'd':
          Serial.print(va_arg(argv, int));
          break;
        case 'l':
          Serial.print(va_arg(argv, long));
          break;
        case 'f':
          Serial.print(va_arg(argv, double));
          break;
        case 'c':
          Serial.print((char)va_arg(argv, int));
          break;
        case 's':
          Serial.print(va_arg(argv, char *));
          break;
        default:;
      };
    } else {
      temp[j] = str[i];
      j = (j + 1) % ARDBUFFER;
      if (j == 0) {
        temp[ARDBUFFER] = '\0';
        Serial.print(temp);
        temp[0] = '\0';
      }
    }
  };
  Serial.println();
  return count + 1;
}
#undef ARDBUFFER
#endif


#include <Wire.h>
#include <Adafruit_MLX90614.h>
#include "MAX30105.h"
#include "heartRate.h"
MAX30105 particleSensor;


const byte RATE_SIZE = 4;  //Increase this for more averaging. 4 is good.
byte rates[RATE_SIZE];     //Array of heart rates
byte rateSpot = 0;
long lastBeat = 0;  //Time at which the last beat occurred
float beatsPerMinute;
int beatAvg;
Adafruit_MLX90614 mlx = Adafruit_MLX90614();

void setup() {
  Serial.begin(9600);
  Serial.println("Initializing...");
  while (!Serial)
    ;
  if (!mlx.begin()) {

    Serial.println("Error connecting to MLX sensor. Check wiring.");
    Serial.println("Restart Arduino once wiring is fixed");
    while (1)
      ;
  };
  // Initialize sensor
  if (!particleSensor.begin(Wire, 100000))  //Use default I2C port, 400kHz speed
  {
    Serial.println("MAX30102 Sensor was not found. Please check wiring/power.");
    Serial.println("Restart Arduino once wiring is fixed");
    // while (1)
    //   ;
  }
  Serial.println("Place your index finger on the sensor with steady pressure.");
  particleSensor.setup();                     //Configure sensor with default settings
  particleSensor.setPulseAmplitudeRed(0x0A);  //Turn Red LED to low to indicate sensor is running
  particleSensor.setPulseAmplitudeGreen(0);   //Turn off Green LED
  particleSensor.enableDIETEMPRDY();
}

void loop() {
  long irValue = particleSensor.getIR();

    if (checkForBeat(irValue) == true)  // checkForBeat is a function of heartRate.h Library
  {
    //We sensed a beat!

    Serial.println(millis());
    long delta = millis() - lastBeat;
    lastBeat = millis();
    beatsPerMinute = 60 / (delta / 1000.0);
    if (beatsPerMinute < 255 && beatsPerMinute > 20) {
      rates[rateSpot++] = (byte)beatsPerMinute;  //Store this reading in the array
      rateSpot %= RATE_SIZE;                     //Wrap variable
      //Take average of readings
      beatAvg = 0;
      for (byte x = 0; x < RATE_SIZE; x++)
        beatAvg += rates[x];
      beatAvg /= RATE_SIZE;
    }
  }


  ardprintf("spo2=98&bpm=%d&temp=%f", beatAvg, mlx.readObjectTempC());

  // delay(500);
}