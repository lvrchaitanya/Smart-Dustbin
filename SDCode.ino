#include <ESP8266WiFi.h>
const int trigPin = D5;
const int echoPin = D6;
long duration;
int distance; 

void send_event(const char *event);
float level;

const char* ssid = "Home_WIFI"; 
const char* password = "**********";    

const char *host = "maker.ifttt.com";
const char *privateKey = "i0AYIY9ZVoN-JrJRg2FmniJzPsjN64RKSLGKPiAR";  Webhooks key

unsigned long myChannelNumber = 1039305;       channrl number
const char * myWriteAPIKey = "I3SVNCB91VI8S7NS";   THingSpeak api key

WiFiServer server(80);

void setup() {
pinMode(trigPin, OUTPUT);
pinMode(echoPin, INPUT);
Serial.begin(9600);
Serial.print("Connecting to Wifi Network");
Serial.println(ssid);
WiFi.begin(ssid, password);
while (WiFi.status() != WL_CONNECTED) {
delay(500);
Serial.print(".");
}
Serial.println("");
Serial.println("Successfully connected to WiFi.");
Serial.println("IP address is : ");
Serial.println(WiFi.localIP());
server.begin();
Serial.println("Server started");
}

void loop() {

digitalWrite(trigPin, LOW);
delayMicroseconds(2);
digitalWrite(trigPin, HIGH);
delayMicroseconds(10);
digitalWrite(trigPin, LOW);
duration = pulseIn(echoPin, HIGH);
distance = duration * 0.0340 / 2;
Serial.println("Distance");
Serial.println(distance);
level =((28 -distance)/28.0)*100;
Serial.println("level");
Serial.println(level);

delay(1000);


WiFiClient client = server.available();

if ( level >= 70) {
send_event("dustbin_event");
}

} 
}

void send_event(const char *event)
{
Serial.print("Connecting to "); 
Serial.println(host);

// Use WiFiClient class to create TCP connections
WiFiClient client;
const int httpPort = 80;
if (!client.connect(host, httpPort)) {
Serial.println("Connection failed");
return;
}

// We now create a URI for the request
String url = "/trigger/";
url += event;
url += "/with/key/";
url += privateKey;

Serial.print("Requesting URL: ");
Serial.println(url);

// This will send the request to the server
client.print(String("GET ") + url + " HTTP/1.1\r\n" +
"Host: " + host + "\r\n" + 
"Connection: close\r\n\r\n");
while(client.connected())
{
if(client.available())
{
String line = client.readStringUntil('\r');
Serial.print(line);
} else {
// No data yet, wait a bit
delay(50);
};
}

  
Serial.println();
Serial.println("closing connection");
client.stop();
}
