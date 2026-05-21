#include <ArduinoOTA.h>
#include <M5Stack.h>
#include <WiFi.h>
#include <PubSubClient.h>
#include <ESP32Servo.h>

#define trigPin 5
#define echoPin 18
#define s_pin1 35
#define s_pin2 36
#define R1 26
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
const char* topic1 = "Camion/PoidsEntree";
const char* topic2 = "Camion/PoidsSortie";
const char* topic3 = "Camion/R1";
const char* topic4 = "Camion/R2";
int positionActuelle1 = 0; // Position actuelle du servo1
int positionActuelle2 = 0; // Position actuelle du servo2

WiFiClient espClient;
PubSubClient client(espClient);


void listInstruction(int temps, int degrefinal, int degreInit) {
    
    int hertz = temps * 20;
    int step = (degrefinal - degreInit) / hertz; // Calculate the step size for each increment
    int instruction = degreInit;
    for(int i = 0; i < hertz; i++) {
        int instruction = instruction  + step; // Initialize all elements to the corresponding step value
        servo1.write(instruction);
        delay(50);
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
void reconnect() {
  while (!client.connected()) {
    M5.Lcd.println("Connexion MQTT...");
    
    if (client.connect("M5StackClient")) {
      M5.Lcd.println("MQTT connecte !");
    } else {
      M5.Lcd.print("Erreur, rc=");
      M5.Lcd.print(client.state());
      delay(2000);
    }
  }
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
float lireTension(int pin) {
  int valeur = analogRead(pin);

  // Conversion en tension 0–3.3V
  float tension = valeur * (3.3 / 4095.0);

  // Correction pont diviseur (ex: 10k / 10k → x2)
  float tension_reelle = tension * 2;

  return tension_reelle;
}

float retourCapteur1(){
  float v = lireTension(s_pin1);
  M5.Lcd.printf("Capteur1: %.2f V\n", v);
  return v;
}

float retourCapteur2(){
  float v = lireTension(s_pin2);
  M5.Lcd.printf("Capteur2: %.2f V\n", v);
  return v;
}
void rougeOn(int pin){
  digitalWrite(pin, HIGH);
}
void rougeOff(int pin){
  digitalWrite(pin, LOW);
}
void callback(char* topic, byte* payload, unsigned int length) {

  String message = "";

  for (int i = 0; i < length; i++) {
    message += (char)payload[i];
  }

  message.trim();
  message.toUpperCase();

  // -------- LED R1 --------
  if (message == "true") rougeOn(R1);
  if (message == "false") rougeOff(R1);

}

void setup() {
  M5.begin();  // Init M5Core
  M5.Lcd.setTextSize(2);
  M5.Power.begin(); // initialize battery usage
  WifiConnect();
  OTASetup();
  client.setServer(mqtt_server, 1883);
  client.setCallback(callback);
  // -----------------------------
  // Rest of the setup code here : 
  ESP32PWM::allocateTimer(0);
  ESP32PWM::allocateTimer(1);
  ESP32PWM::allocateTimer(2);
  ESP32PWM::allocateTimer(3);
  pinMode(R1, OUTPUT);
  rougeOff(R1);
}
void loop() {
if (!client.connected()) {
    reconnect();
  }
  client.loop();
  ArduinoOTA.handle();  // Continuously check for update requests.
  M5.update();
  // appuie sur un bouton envoie un message MQTT
if (a==0){
  M5.Lcd.fillScreen(BLACK);
  a+=1;
}
  client.subscribe(topic3);
  M5.Lcd.setCursor(0,0);
  // M5.Lcd.setTextColor(WHITE,BLACK);
  float c1=retourCapteur1();
  float c2=retourCapteur2();
  char message1[50];
  char message2[50];
  dtostrf(c1, 4, 2, message1);
  client.publish(topic1, message1);
  dtostrf(c2, 4, 2, message2);
  client.publish(topic2, message2);

  delay(300);

  // -----------------------------
  // Rest of the loop code here : 
    
}