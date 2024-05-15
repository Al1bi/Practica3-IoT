#ifndef WIFIMANAGER_H
#define WIFIMANAGER_H

#include <WiFi.h>

class WiFiManager {
private:  
  const char* ssid;
  const char* password;

  void initializeWiFi();
  void waitForWiFiConnection();
  void showIpAddress();

public:
  WiFiManager(const char* _ssid, const char* _password);
  ~WiFiManager();
  void connect(); 
};

#endif
