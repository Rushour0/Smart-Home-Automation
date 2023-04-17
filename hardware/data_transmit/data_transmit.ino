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
#include <SoftwareSerial.h>
SoftwareSerial mySerial(4, 3);
int ledpin = 13;
Adafruit_MLX90614 mlx = Adafruit_MLX90614();

void setup() {
  Serial.begin(9600);
  mySerial.begin(9600);
  Serial.println("Initializing...");
  pinMode(ledpin, OUTPUT);
  while (!Serial)
    ;
  if (!mlx.begin()) {

    Serial.println("Error connecting to MLX sensor. Check wiring.");
    Serial.println("Restart Arduino once wiring is fixed");
    while (1)
      ;
  };
  // Initialize sensor //Turn off Green LED
}

void loop() {
  int i;

  if (mySerial.available()) {
    // mySerial.write(mlx.readObjectTempC());
    mySerial.write(Serial.read());
    // i = mySerial.read();
    // Serial.println("DATA RECEIVED:");
    // if (i == '1') {
    //   digitalWrite(ledpin, 1);
    //   Serial.println("led on");
    // }
    // if (i == '0') {
    //   digitalWrite(ledpin, 0);
    //   Serial.println("led off");
    // }
  }
  // ardprintf("Ambient : %f C | Ambient : %f F | Object : %f C | Object : %f F ", mlx.readAmbientTempC(), mlx.readAmbientTempF(), mlx.readObjectTempC(), mlx.readObjectTempF());
  delay(500);
}