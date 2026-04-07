#include <ArduinoOTA.h>
#include <M5Stack.h>
#include <WiFi.h>
#include <PubSubClient.h>
#include <ESP32Servo.h>

#define trigPin 5
#define echoPin 18
#define s_pin1 16
#define s_pin2 17

Servo servo1;  // Premier servo
Servo servo2;  // Deuxième servo

int pinServo1 = 22;
int pinServo2 = 23;
int a=0;

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
  M5.lcd.println("OTA ready!");
  M5.lcd.println(analogRead(s_pin1));
  M5.lcd.println(analogRead(s_pin2));
  M5.lcd.println("CAPTEUR 1");  // M5.lcd port output format string.
}

void retourCapteur1(){
  M5.lcd.println(analogRead(s_pin1));
}
void retourCapteur2(){
  M5.lcd.println(analogRead(s_pin2));
}


void setup() {
  M5.begin();  // Init M5Core
  M5.Lcd.setTextSize(2);
  M5.Power.begin(); // initialize battery usage
  WifiConnect();
  OTASetup();
  // -----------------------------
  // Rest of the setup code here : 
  ESP32PWM::allocateTimer(0);
  ESP32PWM::allocateTimer(1);
  ESP32PWM::allocateTimer(2);
  ESP32PWM::allocateTimer(3);

}

void loop() {

  ArduinoOTA.handle();  // Continuously check for update requests.
  M5.update();
  // appuie sur un bouton envoie un message MQTT
if (a==0){
  M5.Lcd.fillScreen(BLACK);
  a+=1;
}
  M5.Lcd.setCursor(0,0);
  // M5.Lcd.setTextColor(WHITE,BLACK);
  retourCapteur1();
  delay(100);
  retourCapteur2();
  delay(100);

  // -----------------------------
  // Rest of the loop code here : 
    
}