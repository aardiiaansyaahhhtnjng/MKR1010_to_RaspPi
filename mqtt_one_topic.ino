JSON

#include <WiFiNINA.h>              // Library for WiFi connection using the WiFiNINA module
#include <ArduinoMqttClient.h>     // Library for MQTT communication
#include <DHT.h>                   // Library for DHT sensor
#include <ArduinoJson.h>           // Library for JSON handling

#define DHTPIN A1                  // Pin to which the DHT sensor is connected
#define DHTTYPE DHT11              // Type of the DHT sensor
DHT dht(DHTPIN, DHTTYPE);          // Create DHT object with the specified pin and type

char ssid[] = "SAY THE NAME";      // WiFi SSID
char pass[] = "SEVENTEEN";         // WiFi password

WiFiClient wifiClient;             // WiFi client object
MqttClient mqttClient(wifiClient); // MQTT client object using WiFi client

const char* mqttBroker = "raspberryPI";         // MQTT broker address
const int mqttPort = 1883;                      // MQTT broker port

const char topic[] = "cse/monitor";    // MQTT topic for all sensor readings

const long interval = 3000;              // Interval for sending messages (in milliseconds)
unsigned long previousMillis = 0;        // Variable to store previous time

void setup() {
  Serial.begin(9600);                               // Initialize serial communication

  Serial.print("Connecting to network: ");
  Serial.println(ssid);
  while (WiFi.begin(ssid, pass) != WL_CONNECTED) {  // Connect to WiFi network
    Serial.print(".");
    delay(5000);
  }
  Serial.println("You're connected to the network");

  Serial.print("Connecting to the MQTT broker: ");
  Serial.println(mqttBroker);
  if (!mqttClient.connect(mqttBroker, mqttPort)) {  // Connect to MQTT broker
    Serial.print("MQTT connection failed! Error code = ");
    Serial.println(mqttClient.connectError());
    while (1);
  }
  Serial.println("You're connected to the MQTT broker!");

  dht.begin();                                    // Initialize DHT sensor
  Serial.println("DHT sensor ready!");
  Serial.println();
}

void loop() {
  unsigned long currentMillis = millis();           // Get current time
  if (currentMillis - previousMillis >= interval) { // Check if interval has passed
    previousMillis = currentMillis;                 // Update previous time

    float temp = dht.readTemperature();            // Read temperature from DHT sensor
    float hum = dht.readHumidity();                // Read humidity from DHT sensor
    float cur = random(900, 6600);                 // Generate random current value
    float volt = random(150, 1700);                // Generate random voltage value

    // Create a JSON object to store sensor readings
    StaticJsonDocument<200> jsonDocument;
    jsonDocument["temperature"] = temp;
    jsonDocument["humidity"] = hum;
    jsonDocument["current"] = cur;
    jsonDocument["voltage"] = volt;

    // Serialize the JSON object into a string
    String jsonStr;
    serializeJson(jsonDocument, jsonStr);

    Serial.println("Sensor Readings:");
    Serial.println(jsonStr);

    mqttClient.beginMessage(topic);           // Publish sensor readings to MQTT broker
    mqttClient.print(jsonStr);                    
    mqttClient.endMessage();                   // End MQTT message
  }
}
