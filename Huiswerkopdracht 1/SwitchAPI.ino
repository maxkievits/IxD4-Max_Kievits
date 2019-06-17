#include <SoftwareSerial.h>
#include <SparkFunESP8266WiFi.h>

#define NETWORK_NAME "iPhone van Max Kievits"
#define NETWORK_PASSWORD "Haha dacht het even niet ;)"
 
int trigPin = 11;    // Trigger
int echoPin = 12;    // Echo
long duration, cm, inches;

int ledPin = 5;

float capacity;
float oldCapacity = 0;

int averageMetingArray[10];

float cm2;
 
void setup() {
  //Serial Port begin
  Serial.begin (9600);
  //Define inputs and outputs
  setupESP8266(true);

  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  pinMode(ledPin, OUTPUT);
}

 
void loop() {
  getSwitches();
}

void getSwitches {
  ESP8266Client client;
  
  int result = client.connect("km4.mobidapt.com", 80);
  if (result <= 0) {
    Serial.println("Failed to connect to server.");
    delay(1000);
  } else {
    Serial.println(result);
    Serial.println("Send HTTP request...");

    String request = "/sliders.php?userid=max&sliderid=slider1";
    
    client.println("GET " + request + " HTTP/1.1\n"
                   "Host: max.mobidapt.com\n"
                   "Connection: close\n");

    Serial.println("Response from server");
    String response;
    while (client.available()) {
      response += (char)client.read();
    }
    //Serial.println(response);

     // zoek in de response naar 't woord 'off'
    if (response.indexOf("off") >= 0) {
      digitalWrite(ledPin, LOW);
    }
    if (response.indexOf("on") >= 0) {
      digitalWrite(ledPin, HIGH);
    }
  }

  if (client.connected()) {
    client.stop();
  }
}
