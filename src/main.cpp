#include <ArduinoOTA.h>
#include <M5Stack.h>
#include <WiFi.h>
#include <PubSubClient.h>
#include <ESP32Servo.h>

#define trigPin 5
#define echoPin 18

Servo servo1;  // Premier servo
Servo servo2;  // Deuxième servo

int pinServo1 = 22;
int pinServo2 = 23;

const char* ssid     = "WIFI-IoT";
const char* password = "IoT-1234";
const char* deviceName = "Groupe_RAH";
const char* OTAPassword = "tiensdonc";// mot de passe upload sketch
const char* mqtt_server = "IoT-broker.local"; //ping IoT-broker.local

int positionActuelle1 = 0; // Position actuelle du servo1
int positionActuelle2 = 0; // Position actuelle du servo2

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

void Servomoteur1(int anglevoulue) {
  int increment = (anglevoulue > positionActuelle1) ? 1 : -1;
  if (anglevoulue != positionActuelle1) {
    for ( int pos = positionActuelle1; pos <= anglevoulue; pos += increment)
    {
      servo1.write(pos);
      delay(20);
    }
  else {
      for ( int pos = positionActuelle1; pos >= anglevoulue; pos += increment)
    {
      servo1.write(pos);
      delay(20);
    }
  }
    
}

void Servomoteur2(int anglevoulue, int angleprecedent){
  for (int pos = angleprecedent; pos <= anglevoulue; pos++) {
    servo2.write(pos);
    delay(20);
  }
}

void setup() {
  M5.begin();  // Init M5Core
  M5.Power.begin(); // initialize battery usage
  WifiConnect();
  OTASetup();
  // -----------------------------
  // Rest of the setup code here : 
  ESP32PWM::allocateTimer(0);
  ESP32PWM::allocateTimer(1);
  ESP32PWM::allocateTimer(2);
  ESP32PWM::allocateTimer(3);
  servo1.setPeriodHertz(50);
  servo1.attach(pinServo1, 500, 2400);
  
  servo2.setPeriodHertz(50);
  servo2.attach(pinServo2, 500, 2400);
  

}

void loop() {
  ArduinoOTA.handle();  // Continuously check for update requests.
  M5.update();
  // appuie sur un bouton envoie un message MQTT
  
  Servomoteur1(90);
  delay(1000);
  Servomoteur1(90);
  delay(1000);
  // -----------------------------
  // Rest of the loop code here :

    
}