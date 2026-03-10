#include <M5StickCPlus.h>
#include <WiFi.h>
#include <ArduinoOTA.h>
#include <PubSubClient.h>

const char* ssid     = "WIFI-IoT";
const char* password = "IoT-1234";
const char* deviceName = "M5Cplus-GroupeX";
const char* OTAPassword = "123456";// mot de passe upload sketch
const char* mqtt_server = "IoT-broker.local";

WiFiClient espClient;
PubSubClient client(espClient);

void callback(char* topic, byte* payload, unsigned int length) {
  // Handle message arrived
  M5.lcd.print("Message arrived [");
  M5.lcd.print(topic);
  M5.lcd.print("] ");
  for (unsigned int i = 0; i < length; i++) {
    M5.lcd.print((char)payload[i]);
  }
  M5.lcd.println();
}

bool reconnect() {
  M5.lcd.print("Attempting MQTT connection...");
  // Attempt to connect
  if (client.connect(deviceName)) {
    M5.lcd.println("connected");
    // Once connected, publish an announcement...
    client.publish("outTopic", "hello world");
    // ... and resubscribe
    client.subscribe("inTopic");
    return true;
  } else {
    M5.lcd.print("failed, rc=");
    M5.lcd.print(client.state());
    M5.lcd.println(" try again in 5 seconds");
    // Wait 5 seconds before retrying
    delay(5000);
    return false;
  }
}


void WifiConnect(){
  WiFi.begin(ssid, password);  // Connect wifi and return connection status.
  M5.lcd.print("Connecting to ");
  M5.lcd.println(ssid);
  while (WiFi.status() != WL_CONNECTED) {
      delay(1000);
      M5.lcd.print(".");
  }
  M5.lcd.println("\nWiFi Connected!");
  M5.lcd.print(deviceName);
  M5.lcd.print(" connected To: ");
  M5.lcd.println(WiFi.SSID());  // Output Network name.  
  M5.lcd.print("IP address: ");
  M5.lcd.println(WiFi.localIP());  // Output IP Address.
}

void OTASetup(){
  ArduinoOTA.setHostname(deviceName);  // Set the network port name.
  ArduinoOTA.setPassword(OTAPassword);  // Set the network port connection
  ArduinoOTA.begin();            // Initialize the OTA.
  M5.lcd.println("OTA ready!");  // M5.lcd port output format string.
}

void setup() {
    M5.begin();
    M5.Lcd.setRotation(3);       // Rotate the screen.
    client.setServer(mqtt_server, 1883);
    client.setCallback(callback);
    WifiConnect();
    OTASetup();

    // -----------------------------
    // Rest of the setup code here : 
    
}

void loop() {
    ArduinoOTA.handle();  // Continuously check for update requests.
    M5.update();
    if (!client.connected()) {
        if (!reconnect());
        return;
    }
    client.loop();
    // -----------------------------
    // Rest of the loop code here :
    M5.lcd.setCursor(4,60);
    M5.lcd.print("Battery : ");
    M5.lcd.print(M5.Axp.GetBatVoltage());
    M5.lcd.println(" V");
    delay(500);
}