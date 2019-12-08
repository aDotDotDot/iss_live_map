#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <ArduinoJson.h>
// Network SSID
const char* ssid = "<YOUR SSID>";
const char* password = "<YOUR PASSWORD>";
DynamicJsonDocument doc(300);

void setup() {
  Serial.begin(115200);
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {

    delay(1000);
    Serial.print("Connecting..");
  }
  // Print the IP address
  Serial.print("IP address: ");
  Serial.print(WiFi.localIP());

}

void loop() {
  if (WiFi.status() == WL_CONNECTED) { //Check WiFi connection status

    HTTPClient http;  //Declare an object of class HTTPClient

    http.begin("http://api.open-notify.org/iss-now.json");  //Specify request destination
    int httpCode = http.GET();//Send the request

    if (httpCode > 0) { //Check the returning code

      String payload = http.getString();   //Get the request response payload
      Serial.println(payload);//Print the response payload
      //{"iss_position": {"latitude": "-10.3147", "longitude": "130.5024"}, "message": "success", "timestamp": 1575810636}
      deserializeJson(doc, payload);
      String lat_s = doc["iss_position"]["latitude"];
      String long_s = doc["iss_position"]["longitude"];
      double latitude_n = lat_s.toDouble() + 90;//normalized to be 0°-180°, not -90°/+90°
      double longitude_n = long_s.toDouble() + 180;//normalized to be 0°-360° and not -180°/+180°
      Serial.println(latitude_n);
      Serial.println(longitude_n);
    }

    http.end();   //Close connection

  }

  delay(30000);    //Send a request every 30 seconds

}
