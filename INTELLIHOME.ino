#include <WiFi.h>
#include <BlynkSimpleEsp32.h>
#include <DHT.h>

char ssid[] = "Pranay";
char pass[] = "12345678";

bool fetch_blynk_state = true;

#define DHTPIN              16 // D16 pin connected with DHT

// Uncomment whatever type you're using!
#define DHTTYPE DHT11     // DHT 11
//#define DHTTYPE DHT22   // DHT 22, AM2302, AM2321
//#define DHTTYPE DHT21   // DHT 21, AM2301

#define RelayPin1 23  // D23
#define RelayPin2 22  // D22
#define RelayPin3 21  // D21
#define RelayPin4 19  // D19

#define SwitchPin1 13  // D13
#define SwitchPin2 12  // D12
#define SwitchPin3 14  // D14
// #define SwitchPin4 27  // D27 (commented out as it is not used)

#define wifiLed   2   // D2

#define VPIN_BUTTON_1    V1 
#define VPIN_BUTTON_2    V2
#define VPIN_BUTTON_3    V3 
#define VPIN_TEMPERATURE V4
#define VPIN_HUMIDITY    V5

bool toggleState_1 = LOW;
bool toggleState_2 = LOW;
bool toggleState_3 = LOW;

int wifiFlag = 0;
float temperature1 = 0;
float humidity1 = 0;

char auth[] = "fjgqhbLxJPaop0BqR2YRAcWEAemKjVcp";

BlynkTimer timer;
DHT dht(DHTPIN, DHTTYPE);

// Additional setup for relay pins
int relayPins[] = {RelayPin1, RelayPin2, RelayPin3, RelayPin4};

void toggleRelay(int virtualPin, int relayPin, BlynkParam param) {
  bool toggleState = param.asInt();
  digitalWrite(relayPin, !toggleState);
  Blynk.virtualWrite(virtualPin, toggleState);
}

BLYNK_WRITE(VPIN_BUTTON_1) {
  toggleRelay(VPIN_BUTTON_1, RelayPin1, param);
}

BLYNK_WRITE(VPIN_BUTTON_2) {
  toggleRelay(VPIN_BUTTON_2, RelayPin2, param);
}

BLYNK_WRITE(VPIN_BUTTON_3) {
  toggleRelay(VPIN_BUTTON_3, RelayPin3, param);
}

// BLYNK_WRITE(VPIN_BUTTON_4) {
//   toggleRelay(VPIN_BUTTON_4, RelayPin4, param);
// }

void checkBlynkStatus() {
  bool isconnected = Blynk.connected();
  if (isconnected == false) {
    wifiFlag = 1;
    Serial.println("Blynk Not Connected");
    digitalWrite(wifiLed, LOW);
  }
  if (isconnected == true) {
    wifiFlag = 0;
    if (!fetch_blynk_state) {
      Blynk.virtualWrite(VPIN_BUTTON_1, toggleState_1);
      Blynk.virtualWrite(VPIN_BUTTON_2, toggleState_2);
      Blynk.virtualWrite(VPIN_BUTTON_3, toggleState_3);
    }
    digitalWrite(wifiLed, HIGH);
  }
}

BLYNK_CONNECTED() {
  if (fetch_blynk_state) {
    Blynk.syncVirtual(VPIN_BUTTON_1);
    Blynk.syncVirtual(VPIN_BUTTON_2);
    Blynk.syncVirtual(VPIN_BUTTON_3);
  }
  Blynk.syncVirtual(VPIN_TEMPERATURE);
  Blynk.syncVirtual(VPIN_HUMIDITY);
}

void readSensor() {
  delay(3000); // Increase delay before reading the sensor
  float h = dht.readHumidity();
  float t = dht.readTemperature();

  if (isnan(h) || isnan(t)) {
    Serial.println("Failed to read from DHT sensor!");
    return;
  }
  else {
    humidity1 = h;
    temperature1 = t;
  }
}

void sendSensor() {
  readSensor();
  Blynk.virtualWrite(VPIN_HUMIDITY, humidity1);
  Blynk.virtualWrite(VPIN_TEMPERATURE, temperature1);
}

void setup() {
  Serial.begin(9600);

  Blynk.begin(auth, ssid, pass, "blynk.cloud", 80);

  // Set relay pins as output
  for (int i = 0; i < 4; i++) {
    pinMode(relayPins[i], OUTPUT);
    digitalWrite(relayPins[i], HIGH); // Turn off all relays initially
  }

  pinMode(wifiLed, OUTPUT);

  pinMode(SwitchPin1, INPUT_PULLUP);
  pinMode(SwitchPin2, INPUT_PULLUP);
  pinMode(SwitchPin3, INPUT_PULLUP);
  // pinMode(SwitchPin4, INPUT_PULLUP);

  digitalWrite(RelayPin1, !toggleState_1);
  digitalWrite(RelayPin2, !toggleState_2);
  digitalWrite(RelayPin3, !toggleState_3);
  // digitalWrite(RelayPin4, !toggleState_4);

  dht.begin();
  digitalWrite(wifiLed, LOW);

  timer.setInterval(2000L, checkBlynkStatus);
  timer.setInterval(1000L, sendSensor);
  delay(1000);

  if (!fetch_blynk_state) {
    Blynk.virtualWrite(VPIN_BUTTON_1, toggleState_1);
    Blynk.virtualWrite(VPIN_BUTTON_2, toggleState_2);
    Blynk.virtualWrite(VPIN_BUTTON_3, toggleState_3);
  }
}

void loop() {
  Blynk.run();
  timer.run();
}
