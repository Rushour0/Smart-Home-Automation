/*
 * This is a basic example on how to use Espalexa and its device declaration methods.
 */
#ifdef ARDUINO_ARCH_ESP32
#include <WiFi.h>
#else
#include <ESP8266WiFi.h>
#endif
#include <Espalexa.h>
#include <Servo.h>
// #include <Servo.h>

Servo servo;
// prototypes
boolean connectWifi();

//callback functions
void firstLightChanged(uint8_t brightness);
void secondLightChanged(uint8_t brightness);
void feederCallBack(uint8_t value);

// Change this!!
const char* ssid = "JioFiber-CFPcg";
const char* password = "orieveit8muroh2X";

boolean wifiConnected = false;

Espalexa espalexa;

EspalexaDevice* device3;  //this is optional

void setup() {
  Serial.begin(115200);
  servo.attach(13);
  delay(2000);
  // Initialise wifi connection
  wifiConnected = connectWifi();

  if (wifiConnected) {

    // Define your devices here.
    espalexa.addDevice("Light 1", firstLightChanged);        //simplest definition, default state off
    espalexa.addDevice("Light 2", secondLightChanged, 255);  //third parameter is beginning state (here fully on)

    device3 = new EspalexaDevice("Feeder", feederCallBack);  //you can also create the Device objects yourself like here
    espalexa.addDevice(device3);                             //and then add them
    device3->setValue(128);                                  //this allows you to e.g. update their state value at any time!

    espalexa.begin();

  } else {
    while (1) {
      Serial.println("Cannot connect to WiFi. Please check data and reset the ESP.");
      delay(2500);
    }
  }
}

void loop() {
  espalexa.loop();
  delay(1);
}

//our callback functions
void firstLightChanged(uint8_t brightness) {
  Serial.print("Device 1 changed to ");

  //do what you need to do here

  //EXAMPLE
  if (brightness) {
    Serial.print("ON, brightness ");
    Serial.println(brightness);
  } else {
    Serial.println("OFF");
  }
}

void secondLightChanged(uint8_t brightness) {
  //do what you need to do here
}

void feederCallBack(uint8_t feeder) {
  Serial.println(feeder);
  //do what you need to do here
  if (feeder > 0) {

    servo.write(90);
    delay(5000);
    servo.write(0);
    delay(1000);
  }
}

// connect to wifi – returns true if successful or false if not
boolean connectWifi() {
  boolean state = true;
  int i = 0;

  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  Serial.println("");
  Serial.println("Connecting to WiFi");

  // Wait for connection
  Serial.print("Connecting...");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
    if (i > 20) {
      state = false;
      break;
    }
    i++;
  }
  Serial.println("");
  if (state) {
    Serial.print("Connected to ");
    Serial.println(ssid);
    Serial.print("IP address: ");
    Serial.println(WiFi.localIP());
  } else {
    Serial.println("Connection failed.");
  }
  return state;
}