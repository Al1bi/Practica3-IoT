#include  "WiFiManager.h"

WiFiManager::WiFiManager(const char* ssid, const char* password): ssid(ssid), password(password){}

WiFiManager::~WiFiManager(){}

void WiFiManager::initializeWiFi(){
  String messageNetwork = "Connecting to " + String(ssid);
  Serial.println(messageNetwork);
  WiFi.begin(ssid, password);
}

void WiFiManager::waitForWiFiConnection(){
  while(WiFi.status() != WL_CONNECTED){
    delay(200);
    Serial.print(".");
  }
  Serial.println("\nConnection succesful");
}

void WiFiManager::showIpAddress(){
  String messageNetwork = "IP address of the device: " + String(WiFi.localIP());
  Serial.println(messageNetwork);
}

void WiFiManager::connect(){
  initializeWiFi();
  waitForWiFiConnection();
  showIpAddress();
}