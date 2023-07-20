#include <WiFiNINA.h>              // Library for WiFi connection using the WiFiNINA module
#include <ArduinoMqttClient.h>     // Library for MQTT communication
#include <DHT.h>                   // Library for DHT sensor

#define DHTPIN A1                  // Pin to which the DHT sensor is connected
#define DHTTYPE DHT11              // Type of the DHT sensor
DHT dht(DHTPIN, DHTTYPE);          // Create DHT object with the specified pin and type

char ssid[] = "SAY THE NAME";      // WiFi SSID
char pass[] = "SEVENTEEN";         // WiFi password

WiFiClient wifiClient;             // WiFi client object
MqttClient mqttClient(wifiClient); // MQTT client object using WiFi client

const char* mqttBroker = "raspberryPI";         // MQTT broker address
const int mqttPort = 1883;                      // MQTT broker port

const char topic1[] = "cse/monitor/temp";   // MQTT topic for temperature
const char topic2[] = "cse/monitor/hum";    // MQTT topic for humidity
const char topic3[] = "cse/monitor/cur";    // MQTT topic for current
const char topic4[] = "cse/monitor/volt";   // MQTT topic for voltage

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

    Serial.print("Temperature: ");
    Serial.print(temp);
    Serial.print("°C  ");
    Serial.print("    Humidity: ");
    Serial.print(hum);
    Serial.print("%  ");
    Serial.print("    Current: ");
    Serial.print(cur);
    Serial.print("VA  ");
    Serial.print("    Voltage: ");
    Serial.print(volt);
    Serial.println("volt  ");

    mqttClient.beginMessage(topic1);           
    mqttClient.print(temp);                    // Publish temperature to MQTT broker
    mqttClient.endMessage();                   // End MQTT message
    
    mqttClient.beginMessage(topic2);           
    mqttClient.print(hum);                     // Publish humidity to MQTT broker
    mqttClient.endMessage();                   // End MQTT message

    mqttClient.beginMessage(topic3);           
    mqttClient.print(cur);                     // Publish current to MQTT broker
    mqttClient.endMessage();                   // End MQTT message

    mqttClient.beginMessage(topic4);           
    mqttClient.print(volt);                    // Publish voltage to MQTT broker
    mqttClient.endMessage();                   // End MQTT message
  }
}
