#include "ESPAsyncWebServer.h"
#include <Arduino.h>
#include <esp_now.h>
#include <esp_wifi.h>
#include <WiFi.h>

constexpr char WIFI_SSID[] = "D-Link-3D1BBF";
constexpr char WIFI_PASS[] = "p2mxsajrqwkd9c7g";

// Setta un indirizzo IP Fisso
IPAddress local_IP(192, 168, 1, 200);
// Setta l'indirizzo del Gateway
IPAddress gateway(192, 168, 1, 1);
IPAddress subnet(255, 255, 0, 0);
IPAddress primaryDNS(8, 8, 8, 8); //opzionale
IPAddress secondaryDNS(8, 8, 4, 4); //opzionale

AsyncWebServer server(80);

const char index_html[] PROGMEM = R"rawliteral(
<!DOCTYPE HTML><html>
<head>
  <title>Robotdazero - rete "Ambientale" con ESP32</title>
  <meta name="viewport" content="width=device-width, initial-scale=1">
  <link rel="stylesheet" href="https://use.fontawesome.com/releases/v5.7.2/css/all.css" integrity="sha384-fnmOCqbTlWIlj8LyTjo7mOUStjsKC4pOpQbqyi7RrhN7udi9RwhKkMHpvLbHG9Sr" crossorigin="anonymous">
  <link rel="icon" href="data:,">
  <style>
    html {font-family: Arial; display: inline-block; text-align: center;}
    p {  font-size: 1.2rem;}
    body {  margin: 0;}
    .topnav { overflow: hidden; background-color: #2f4468; color: white; font-size: 1.7rem; }
    .content { padding: 20px; }
    .card { background-color: white; box-shadow: 2px 2px 12px 1px rgba(140,140,140,.5); }
    .cards { max-width: 700px; margin: 0 auto; display: grid; grid-gap: 2rem; grid-template-columns: repeat(auto-fit, minmax(300px, 1fr)); }
    .reading { font-size: 2.8rem; }
    .packet { color: #bebebe; }
    .card.temperature { color: #fd7e14; }
    .card.humidity { color: #1b78e2; }
  </style>
</head>
<body>
  <div class="topnav">
    <h3>ROBOTDAZERO - rete "Ambientale" con ESP32</h3>
  </div>
  <div class="content">
    <div class="cards">
      <div class="card temperature">
        <h4><i class="fas fa-thermometer-half"></i> SCHEDA #1 - TEMPERATURA</h4><p><span class="reading"><span id="t1"></span> &deg;C</span></p><p class="packet">sensore DHT11: <span id="rt1"></span></p>
      </div>
      <div class="card humidity">
        <h4><i class="fas fa-tint"></i> SCHEDA #1 - UMIDITA'</h4><p><span class="reading"><span id="h1"></span> &percnt;</span></p><p class="packet">sensore DHT11: <span id="rh1"></span></p>
      </div>
      <div class="card temperature">
        <h4><i class="far fa-bell"></i> SCHEDA #1 - Fumo/Metano</h4><p><span class="reading"><span id="t2"></span> ppm</span></p><p class="packet">sensore MQ-2: <span id="rt2"></span></p>
      </div>
      <div class="card humidity">
        <h4><i class="far fa-bell"></i> SCHEDA #1 - Qualita' dell'aria</h4><p><span class="reading"><span id="h2"></span> ppm</span></p><p class="packet">sensore MQ-135: <span id="rh2"></span></p>
      </div>
    </div>
  </div>
</body>
</html>)rawliteral";

void initWiFi() {
    WiFi.mode(WIFI_MODE_APSTA);

    if(!WiFi.config(local_IP, gateway, subnet, primaryDNS, secondaryDNS)) {
      Serial.println("STA Failed to configure");
    }

    WiFi.begin(WIFI_SSID, WIFI_PASS);

    Serial.printf("Connecting to %s .", WIFI_SSID);
    while (WiFi.status() != WL_CONNECTED) { Serial.print("."); delay(200); }
    Serial.println("ok");

    IPAddress ip = WiFi.localIP();

    Serial.printf("SSID: %s\n", WIFI_SSID);
    Serial.printf("Channel: %u\n", WiFi.channel());
    Serial.printf("IP: %u.%u.%u.%u\n", ip & 0xff, (ip >> 8) & 0xff, (ip >> 16) & 0xff, ip >> 24);
}


void setup() {
  Serial.begin(115200);
  initWiFi();

  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send_P(200, "text/html", index_html);
  });
   
  server.begin();
}
 
void loop() {
}
