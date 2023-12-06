#include <WiFi.h>
#include <HTTPClient.h>

const char* ssid = "Vu";
const char* password = "920092Vu";
//const char* serverUrl = "http://192.168.1.11/data";

const char* serverUrl = "http://192.168.1.7:5000/";

void setup() {
  Serial.begin(115200);

  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
  }
}

void loop() {
  if (WiFi.status() == WL_CONNECTED) {
    HTTPClient http;
    http.begin(serverUrl);

    int httpResponseCode = http.GET();

    if (httpResponseCode == HTTP_CODE_OK) {
      String payload = http.getString();
      Serial.println(payload);
    } else {
      Serial.print("Error: ");
      Serial.println(httpResponseCode);
    }

    http.end();
  }

  delay(1000);
}
