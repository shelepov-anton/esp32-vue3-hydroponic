#include <WiFi.h>
#include <WebSocketsClient.h>
#include <DHT.h>

// Temperature pin / type
#define DHTTYPE DHT22
#define DHTPIN 4

// WiFi network credentials
const char* ssid = "RT-GPON-2188";
const char* password = "y9peNyjZiY";

// WebSocket server details
const char* websocket_server = "192.168.0.5";
const uint16_t websocket_port = 3000;
const char* websocket_path = "/";

// Sensor pins
const int sensorPin = 25; // Дискретный датчик переполнения
const int phSensorPin = 32; // Аналоговый пин для pH датчика
const int capacitiveSensorPin = 33; // Аналоговый пин для погружного емкостного датчика

// Relay pins
const int relayPin16 = 16;
const int relayPin17 = 17;
const int relayPin18 = 18;
const int relayPin19 = 19;

// Timing variables
unsigned long lastSensorReadTime = 0;
const unsigned long sensorInterval = 500; // 0.5 second interval for sensor reading
unsigned long lastPhReadTime = 0;
const unsigned long phInterval = 3000; // 3 seconds interval for pH reading
unsigned long lastTempReadTime = 0;
const unsigned long tempInterval = 3000; // 3 seconds interval for temp reading
unsigned long lastCapacitiveReadTime = 0;
const unsigned long capacitiveInterval = 1000; // 1 second interval for capacitive sensor reading

// Create an instance of the DHT
DHT dht(DHTPIN, DHTTYPE);

// Calibration constants
const float voltageAtpH7 = 2.5; // напряжение при pH 7.0
const float conversionCoefficient = 0.05916; // коэффициент преобразования для pH (59.16 мВ/рН)

// Create an instance of the WebSocketsClient
WebSocketsClient webSocket;

// Flags for relay control
bool relay17State = false;

void webSocketEvent(WStype_t type, uint8_t * payload, size_t length) {
  switch(type) {
    case WStype_DISCONNECTED:
      Serial.println("WebSocket Disconnected");
      break;
    case WStype_CONNECTED:
      Serial.println("WebSocket Connected");
      webSocket.sendTXT("Hello Server");
      break;
    case WStype_TEXT:
      Serial.printf("WebSocket Received: %s\n", payload);
      if (strcmp((char*)payload, "TURN_ON_RELAY_17") == 0) {
        relay17State = true;
        digitalWrite(relayPin17, HIGH);
      } else if (strcmp((char*)payload, "TURN_OFF_RELAY_17") == 0) {
        relay17State = false;
        digitalWrite(relayPin17, LOW);
      }
      break;
    case WStype_BIN:
      Serial.println("WebSocket Received Binary Data");
      break;
  }
}

void setup() {
  // Initialize serial communication for debugging
  Serial.begin(9600);

  // Initialize sensor pins as input
  pinMode(sensorPin, INPUT);
  pinMode(capacitiveSensorPin, INPUT);

  // Initialize relay pins as output
  pinMode(relayPin16, OUTPUT);
  pinMode(relayPin17, OUTPUT);
  pinMode(relayPin18, OUTPUT);
  pinMode(relayPin19, OUTPUT);
  digitalWrite(relayPin16, LOW); // Ensure relay is off initially
  digitalWrite(relayPin17, HIGH); // Ensure relay is off initially
  digitalWrite(relayPin18, LOW); // Ensure relay is off initially
  digitalWrite(relayPin19, HIGH); // Relay on pin 19 should always be ON

  // Initialize DHT sensor
  dht.begin();

  // Connect to WiFi
  WiFi.begin(ssid, password);
  Serial.print("Connecting to WiFi ");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println();
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());

  // Initialize WebSocket
  webSocket.begin(websocket_server, websocket_port, websocket_path);
  webSocket.onEvent(webSocketEvent);
  Serial.println("Connecting to WebSocket server...");
}

void loop() {
  // Maintain WebSocket connection
  webSocket.loop();
  yield(); // Allow background tasks to run to avoid WDT timeout

  // Get the current time
  unsigned long currentTime = millis();

  // Handle sensor reading
  if (currentTime - lastSensorReadTime >= sensorInterval) {
    lastSensorReadTime = currentTime;
    
    int sensorState = digitalRead(sensorPin);
    Serial.print("Sensor state: ");
    Serial.println(sensorState);
    
    if (sensorState == HIGH) {
      // Turn on relay
      digitalWrite(relayPin16, HIGH);

// Send flag to the WebSocket server
      webSocket.sendTXT("esp32-alarm: Угроза переполнения");

      // Print to serial for debugging
      Serial.println("Sensor triggered, relay turned on, flag sent to server");
    } else {
      // Turn off relay
      digitalWrite(relayPin16, LOW);
      
      // Send flag to the WebSocket server
      webSocket.sendTXT("Sensor not triggered");

      // Print to serial for debugging
      Serial.println("Sensor not triggered, relay turned off, flag sent to server");
    }
  }

  // Handle temperature sensor reading
  if (currentTime - lastTempReadTime >= tempInterval) {
    lastTempReadTime = currentTime;

    float tempValue = dht.readTemperature();
    if (!isnan(tempValue)) {
      String tempMessage = "esp32-temp-value: " + String(tempValue);
      webSocket.sendTXT(tempMessage);

      // Print to serial for debugging
      Serial.println(tempMessage);
    } else {
      Serial.println("Failed to read temperature from DHT sensor");
    }
  }

  // Handle capacitive sensor reading
  if (currentTime - lastCapacitiveReadTime >= capacitiveInterval) {
    lastCapacitiveReadTime = currentTime;

    int capacitiveValue = analogRead(capacitiveSensorPin);
    float voltage = capacitiveValue * (3.3 / 4095.0); // Преобразование значения в напряжение (для ESP32 12-битный ADC)
    float percentage = (voltage / 3.3) * 100.0; // Преобразование напряжения в процент

    // Print capacitive sensor value for debugging
    Serial.print("Capacitive sensor value: ");
    Serial.print(capacitiveValue);
    Serial.print(" Voltage: ");
    Serial.print(voltage);
    Serial.print(" Percentage: ");
    Serial.println(percentage);

    // Send capacitive value to the WebSocket server
    String capacitiveMessage = "esp32-capacitive-value: " + String(capacitiveValue);
    webSocket.sendTXT(capacitiveMessage);

    if (percentage <= 30) {
      digitalWrite(relayPin18, HIGH);
      webSocket.sendTXT("esp32-alarm: Уровень воды ниже порогового");
      Serial.println("Capacitive sensor detected immersion above 30%, relay 18 turned on, flag sent to server");
    } else {
      digitalWrite(relayPin18, LOW);
    }
  }

  // Handle pH sensor reading
  if (currentTime - lastPhReadTime >= phInterval) {
    lastPhReadTime = currentTime;

    int phValue = analogRead(phSensorPin);
    float voltage = phValue * (3.3 / 4095.0); // Преобразование значения в напряжение (для ESP32 12-битный ADC)
    float ph = 7 + ((voltageAtpH7 - voltage) / conversionCoefficient); // Преобразование напряжения в pH

    // Send pH value to the WebSocket server
    String phMessage = "esp32-pH-value: " + String(ph);
    webSocket.sendTXT(phMessage);
    
    // Send pH alarm notification
    if (ph < 5.5) {
      webSocket.sendTXT("esp32-alarm: Низкий уровень pH");
    } else if (ph > 7.5) {
      webSocket.sendTXT("esp32-alarm: Высокий уровень pH");
    }

    // Print to serial for debugging
    Serial.println(phMessage);
  }

  delay(1); // Small delay to avoid WDT timeout
}