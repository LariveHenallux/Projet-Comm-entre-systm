#include <ArduinoOTA.h>
#include <M5Stack.h>
#include <WiFi.h>
#include <PubSubClient.h>

const char* ssid     = "WIFI-IoT";
const char* password = "IoT-1234";
const char* deviceName = "M5-GroupeX";
const char* OTAPassword = "123456";// mot de passe upload sketch
const char* mqtt_server = "IoT-broker.local";

WiFiClient espClient;
PubSubClient client(espClient);

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
  M5.begin();  // Init M5Core
  M5.Power.begin(); // initialize battery usage
  WifiConnect();
  OTASetup();
  // -----------------------------
  // Rest of the setup code here : 
  // configure the trigger pin to output mode
  pinMode(trigPin, OUTPUT);
  // configure the echo pin to input mode
  pinMode(echoPin, INPUT);

}

void loop() {
  ArduinoOTA.handle();  // Continuously check for update requests.
  M5.update();
  // -----------------------------
  // Rest of the loop code here :

    
}

