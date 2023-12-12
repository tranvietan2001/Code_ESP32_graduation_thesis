#include <WiFi.h>
#include <ESPAsyncWebServer.h>

const char* ssid = "Vu";
const char* password = "920092Vu";

AsyncWebServer server(80);

void setup() {
  Serial.begin(115200);

  // Kết nối đến mạng Wi-Fi
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi...");
  }

  Serial.println("Connected to WiFi");

  // Địa chỉ IP của ESP32 1
  IPAddress localIP = WiFi.localIP();
  Serial.println(localIP);

  // Khởi tạo webserver
  server.on("/data", HTTP_GET, [](AsyncWebServerRequest *request){
    // Xử lý yêu cầu GET /data
    // Gửi dữ liệu từ ESP32 1 đến ESP32 2
    // ...

    // Phản hồi với thông báo thành công
    request->send(200, "text/plain", "TranVietAn");
  });

  // Bắt đầu lắng nghe yêu cầu từ ESP32 2
  server.begin();

  Serial.println("Webserver started");
}

void loop() {
  // Code xử lý khác, nếu có
}
