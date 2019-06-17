#include <SoftwareSerial.h>
#include <SparkFunESP8266WiFi.h>

#define NETWORK_NAME "iPhone van Max Kievits"
#define NETWORK_PASSWORD "Haha dacht het even niet ;)"
 
int trigPin = 11;    // Trigger
int echoPin = 12;    // Echo
long duration, cm, inches;

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
}
 
void loop() {
  //uploadData();
  meting();
}

void uploadData(int waterlevel) {
  ESP8266Client client;
  
  int result = client.connect("max.mobidapt.com", 80);
  if (result <= 0) {
    Serial.println("Failed to connect to server.");
    delay(1000);
  } else {
    Serial.println(result);
    Serial.println("Send HTTP request...");

    String request = "/ixd/uploadWaterData.php?waterlevel=" + String(waterlevel) + "";
    
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
    if (response.indexOf("success") >= 0) {
      Serial.println("JAJAJAJAJJJAJAJAJJAJ");
    }
  }

  if (client.connected()) {
    client.stop();
  }
}

void meting() {
  // The sensor is triggered by a HIGH pulse of 10 or more microseconds.
  // Give a short LOW pulse beforehand to ensure a clean HIGH pulse:
  digitalWrite(trigPin, LOW);
  delayMicroseconds(5);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
 
  // Read the signal from the sensor: a HIGH pulse whose
  // duration is the time (in microseconds) from the sending
  // of the ping to the reception of its echo off of an object.
  pinMode(echoPin, INPUT);
  duration = pulseIn(echoPin, HIGH);
 
  // Convert the time into a distance
  cm = (duration/2) * 0.0343;     // Divide by 29.1 or multiply by 0.0343
  cm2 = (duration/2) * 0.0343;
  inches = (duration/2) / 74;   // Divide by 74 or multiply by 0.0135]

  capacity = map(cm, 0, 19, 100, 0);

  //Serial.print(oldCapacity);
  //Serial.print("-");
  //Serial.println(capacity);

  
  
  int difference = capacity - oldCapacity;
  //Serial.println(difference);

  Serial.println(capacity);

  if (difference > 6 || difference < -6) {
     Serial.print(capacity);
     Serial.print("%");
     Serial.println();

     uploadData(capacity);
  } else {
     Serial.println("Sprong is te klein!");
  }
  
/*
  if(averageMetingArray[0] == null) {
    
  }
  */

  oldCapacity = capacity;

  delay(20000);

}

float average (int * array, int len)  // assuming array is int.
{
  long sum = 0L ;  // sum will be larger than an item, long for safety.
  for (int i = 0 ; i < len ; i++)
    sum += array [i] ;
  return  ((float) sum) / len ;  // average will be fractional, so float may be appropriate.
}
