#include <WiFi.h>
#include <HTTPClient.h>

const char* ssid     = "";
const char* password = "";

const int AirValue = 4000;   //you need to replace this value with Value_1
const int WaterValue = 1000;  //you need to replace this value with Value_2
int intervals = (AirValue - WaterValue)/3;
int soilMoistureValue = 0;
int analogPin = A0;
String soil; 



void setup() {

  
  // put your setup code here, to run once:
  Serial.begin(115200); // open serial port, set the baud rate as 115200 bps
  delay(100);

   // We start by connecting to a WiFi network

  Serial.println();
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


void loop() {
  // put your main code here, to run repeatedly:

soilMoistureValue = analogRead(analogPin);  //put Sensor insert into soil
if(soilMoistureValue > WaterValue && soilMoistureValue < (WaterValue + intervals))
{
    soil = "Very wet";
}
else if(soilMoistureValue > (WaterValue + intervals) && soilMoistureValue < (AirValue - intervals))
{

   soil = "Wet";
}
else if(soilMoistureValue < AirValue && soilMoistureValue > (AirValue - intervals))
{
    soil = "Dry";
}

String value= String(soilMoistureValue);

HTTPClient http;

http.begin("http://192.168.1.153:8088/services/collector");
http.addHeader("Authorization", "Splunk 313ba7d4-4326-4450-b8b5-eea18fcc9c6c");
String payload = "{ \"host\" : \"plantOffice\", \"sourcetype\" : \"_json\", \"index\" : \"plants\", \"event\" : {\"Soil\" : \"" + soil + "\", \"sensorRead\" : \"" + value + "\"}}";
Serial.println(payload);
http.POST(payload);
http.writeToStream(&Serial);
http.end();
delay(3600);

}
