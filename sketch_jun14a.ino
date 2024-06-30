#include <WiFi.h>
#include <WebSocketsClient.h>

// WiFi network credentials
const char* ssid = "RT-GPON-2188";
const char* password = "y9peNyjZiY";

// WebSocket server details
const char* websocket_server = "192.168.0.5";
const uint16_t websocket_port = 3000;
const char* websocket_path = "/";

// Sensor pin
const int sensorPin = 25;

// pH sensor pin
const int phSensorPin = 32; // Аналоговый пин для pH датчика

// Relay pin
const int relayPin = 16;

// Timing variables
unsigned long lastSensorReadTime = 0;
const unsigned long sensorInterval = 500; // 0.5 second interval for sensor reading
unsigned long lastPhReadTime = 0;
const unsigned long phInterval = 3000; // 3 seconds interval for pH reading

// Calibration constants
const float voltageAtpH7 = 2.5; // напряжение при pH 7.0
const float conversionCoefficient = 0.05916; // коэффициент преобразования для pH (59.16 мВ/рН)

// Create an instance of the WebSocketsClient
WebSocketsClient webSocket;

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
      break;
    case WStype_BIN:
      Serial.println("WebSocket Received Binary Data");
      break;
  }
}

void setup() {
  // Initialize serial communication for debugging
  Serial.begin(9600);

  // Initialize sensor pin as input
  pinMode(sensorPin, INPUT);

  // Initialize relay pin as output
  pinMode(relayPin, OUTPUT);
  digitalWrite(relayPin, LOW); // Ensure relay is off initially

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

  // Get the current time
  unsigned long currentTime = millis();

  // Handle sensor reading
  if (currentTime - lastSensorReadTime >= sensorInterval) {
    lastSensorReadTime = currentTime;
    
    int sensorState = digitalRead(sensorPin);
    if (sensorState == HIGH) {
      // Turn on relay
      digitalWrite(relayPin, HIGH);

      // Send flag to the WebSocket server
      webSocket.sendTXT("esp32-alarm: Угроза переполнения");

      // Print to serial for debugging
      Serial.println("Sensor triggered, relay turned on, flag sent to server");
    } else {
      // Turn off relay
      digitalWrite(relayPin, LOW);
      
      // Send flag to the WebSocket server
      webSocket.sendTXT("Sensor not triggered");

      // Print to serial for debugging
      Serial.println("Sensor not triggered, relay turned off, flag sent to server");
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
}