//pip install esptool --upgrade
//esptool.py --chip esp32 --port COM5 erase_flash

#define BLYNK_PRINT Serial
#include <WiFi.h>
#include <WiFiClient.h>
#include <BlynkSimpleEsp32.h>

char auth[] = "ARbgnjKZdhZEu-hE3PHRnx5so0b0fKP4";
char ssid[] = "Pranay";
char pass[] = "12345678";

// Pins connected to the relay module
int relayPins[] = {23, 22, 21, 19}; // Adjust the pin numbers based on your ESP32 board

void setup() {
  Serial.begin(9600);
  Blynk.begin(auth, ssid, pass, "blynk.cloud", 80);

  // Set relay pins as output
  for (int i = 0; i < 4; i++) {
    pinMode(relayPins[i], OUTPUT);
    digitalWrite(relayPins[i], HIGH); // Turn off all relays initially
  }
}

void loop() {
  Blynk.run();
}

// Blynk virtual pin handler
BLYNK_WRITE(V1) {
  int state = param.asInt();
  digitalWrite(relayPins[0], state);
}

BLYNK_WRITE(V2) {
  int state = param.asInt();
  digitalWrite(relayPins[1], state);
}

BLYNK_WRITE(V3) {
  int state = param.asInt();
  digitalWrite(relayPins[2], state);
}

BLYNK_WRITE(V4) {
  int state = param.asInt();
  digitalWrite(relayPins[3], state);
}
