#include <WiFi.h>
#include <PubSubClient.h>

// WiFi Credentials
const char *ssid = "LBS-2G";            // Replace with your WiFi name
const char *password = "Livebuildings1";  // Replace with your WiFi password

// MQTT Broker Settings
const char *mqtt_broker = "broker.emqx.io";
const char *mqtt_topic = "test/test";
const char *mqtt_username = "emqx";
const char *mqtt_password = "public";
const int mqtt_port = 1883;


WiFiClient espClient;
PubSubClient mqtt_client(espClient);

const int ledPin = 3; // GPIO 3 for LED

// Function Declarations
void connectToWiFi();

void connectToMQTT();

void mqttCallback(char *mqtt_topic, byte *payload, unsigned int length);

void setup() {
    Serial.begin(115200);
    pinMode(ledPin, OUTPUT); 
    connectToWiFi();
    mqtt_client.setServer(mqtt_broker, mqtt_port);
    mqtt_client.setKeepAlive(60);
    mqtt_client.setCallback(mqttCallback); // Corrected callback function name
    connectToMQTT();
}

void connectToWiFi() {
    WiFi.begin(ssid, password);
    Serial.print("Connecting to WiFi");
    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.print(".");
    }
    Serial.println("\nConnected to WiFi");
}

void connectToMQTT() {
    while (!mqtt_client.connected()) {
        String client_id = "esp32-client-" + String(WiFi.macAddress());
        Serial.printf("Connecting to MQTT Broker as %s.....\n", client_id.c_str());
        if (mqtt_client.connect(client_id.c_str(), mqtt_username, mqtt_password)) {
            Serial.println("Connected to MQTT broker");
            mqtt_client.subscribe(mqtt_topic);
            mqtt_client.publish(mqtt_topic, "Hi EMQX I'm ESP32 ^^"); // Publish message upon successful connection
        } else {
            Serial.print("Failed, rc=");
            Serial.print(mqtt_client.state());
            Serial.println(" try again in 5 seconds");
            delay(5000);
        }
    }
}

void mqttCallback(char *mqtt_topic, byte *payload, unsigned int length) {
    Serial.print("Message received on mqtt_topic: ");
    Serial.println(mqtt_topic);
    Serial.print("Message: ");
    String message;
    for (unsigned int i = 0; i < length; i++) {
        message += (char) payload[i];
    }
    Serial.println(message);
    Serial.println("-----------------------");

    Serial.println("Dhana");

    if (message == "ON") {
        digitalWrite(ledPin, HIGH); // Turn LED on
        Serial.println("LED is ON");
    } else if (message == "OFF") {
        Serial.println("heyyy");
        digitalWrite(ledPin, LOW); // Turn LED off
        Serial.println("hey is it ?");
        Serial.println("LED is OFF");
    }
    Serial.println("-----------------------");
}


void loop() {
    if (!mqtt_client.connected()) {
        connectToMQTT();
    }
    mqtt_client.loop();
}