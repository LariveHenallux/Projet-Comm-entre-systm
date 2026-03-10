#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include <ArduinoOTA.h>

const char* ssid     = "WIFI-IoT";
const char* password = "IoT-1234";
const char* deviceName = "ESP8266-GroupeX";
const char* OTAPassword = "123456"; // mot de passe upload sketch
const char* mqtt_server = "IoT-broker.local";

WiFiClient espClient;
PubSubClient client(espClient);

void WifiConnect() {
    delay(10);
    // Connect to WiFi
    Serial.println();
    Serial.print("Connecting to ");
    Serial.println(ssid);
    
    WiFi.begin(ssid, password);

    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.print(".");
    }

    Serial.println("");
    Serial.println("WiFi connected");
    Serial.println("IP address: ");
    Serial.println(WiFi.localIP());
}

void callback(char* topic, byte* payload, unsigned int length) {
    Serial.print("Message arrived [");
    Serial.print(topic);
    Serial.print("] ");
    for (int i = 0; i < length; i++) {
        Serial.print((char)payload[i]);
    }
    Serial.println();
}

void reconnect() {
    Serial.print("Attempting MQTT connection...");
    // Attempt to connect
    if (client.connect("ESP8266Client")) {
        Serial.println("connected");
        // Once connected, publish an announcement...
        client.publish("outTopic", "hello world");
        // ... and resubscribe
        client.subscribe("inTopic");
    } else {
        Serial.print("failed, rc=");
        Serial.print(client.state());
        Serial.println(" try again in 5 seconds");
        // Wait 5 seconds before retrying
        delay(5000);
    }
}


void OTASetup(){
    ArduinoOTA.setHostname(deviceName);  // Set the network port name.
    ArduinoOTA.setPassword(OTAPassword);  // Set the network port connection
    ArduinoOTA.begin();            // Initialize the OTA.
}

void setup() {
    Serial.begin(115200);
    setup_wifi();
    client.setServer(mqtt_server, 1883);
    client.setCallback(callback);
    OTASetup();
}

void loop() {
    ArduinoOTA.handle();
    if (!client.connected()) {
        if (! reconnect());
            return;
    }
    client.loop();
    // -----------------------------
    // Rest of the loop code here :
}