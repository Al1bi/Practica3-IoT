#include <ArduinoJson.h>

#include "WiFiManager.h"
#include "AmazonCredentials.h"
#include "MQTTConfig.h"
#include "MQTTManager.h"
#include "Dispenser.h"

const char * WIFI_SSID = "UCB-PREMIUM";
const char * WIFI_PASS = "lacatoucb";

const char * MQTT_BROKER_HOST = "a2dmh18tmfqz8b-ats.iot.us-east-2.amazonaws.com";
const int MQTT_BROKER_PORT = 8883;

const char * MQTT_CLIENT_ID = "ESP-32";               
const char * UPDATE_TOPIC = "$aws/things/Dispenser/shadow/update";              // publish
const char * UPDATE_DELTA_TOPIC = "$aws/things/Dispenser/shadow/update/delta";  // subscribe

const char ROOT_CA1[] PROGMEM = R"EOF(
-----BEGIN CERTIFICATE-----
MIIDQTCCAimgAwIBAgITBmyfz5m/jAo54vB4ikPmljZbyjANBgkqhkiG9w0BAQsF
ADA5MQswCQYDVQQGEwJVUzEPMA0GA1UEChMGQW1hem9uMRkwFwYDVQQDExBBbWF6
b24gUm9vdCBDQSAxMB4XDTE1MDUyNjAwMDAwMFoXDTM4MDExNzAwMDAwMFowOTEL
MAkGA1UEBhMCVVMxDzANBgNVBAoTBkFtYXpvbjEZMBcGA1UEAxMQQW1hem9uIFJv
b3QgQ0EgMTCCASIwDQYJKoZIhvcNAQEBBQADggEPADCCAQoCggEBALJ4gHHKeNXj
ca9HgFB0fW7Y14h29Jlo91ghYPl0hAEvrAIthtOgQ3pOsqTQNroBvo3bSMgHFzZM
9O6II8c+6zf1tRn4SWiw3te5djgdYZ6k/oI2peVKVuRF4fn9tBb6dNqcmzU5L/qw
IFAGbHrQgLKm+a/sRxmPUDgH3KKHOVj4utWp+UhnMJbulHheb4mjUcAwhmahRWa6
VOujw5H5SNz/0egwLX0tdHA114gk957EWW67c4cX8jJGKLhD+rcdqsq08p8kDi1L
93FcXmn/6pUCyziKrlA4b9v7LWIbxcceVOF34GfID5yHI9Y/QCB/IIDEgEw+OyQm
jgSubJrIqg0CAwEAAaNCMEAwDwYDVR0TAQH/BAUwAwEB/zAOBgNVHQ8BAf8EBAMC
AYYwHQYDVR0OBBYEFIQYzIU07LwMlJQuCFmcx7IQTgoIMA0GCSqGSIb3DQEBCwUA
A4IBAQCY8jdaQZChGsV2USggNiMOruYou6r4lK5IpDB/G/wkjUu0yKGX9rbxenDI
U5PMCCjjmCXPI6T53iHTfIUJrU6adTrCC2qJeHZERxhlbI1Bjjt/msv0tadQ1wUs
N+gDS63pYaACbvXy8MWy7Vu33PqUXHeeE6V/Uq2V8viTO96LXFvKWlJbYK8U90vv
o/ufQJVtMVT8QtPHRh8jrdkPSHCa2XV4cdFyQzR1bldZwgJcJmApzyMZFo6IQ6XU
5MsI+yMRQ+hDKXJioaldXgjUkK642M4UwtBV8ob2xJNDd2ZhwLnoQdeXeGADbkpy
rqXRfboQnoZsG4q5WTP468SQvvG5
-----END CERTIFICATE-----
)EOF";

const char CERTIFICATE[] PROGMEM = R"KEY(
-----BEGIN CERTIFICATE-----
MIIDWjCCAkKgAwIBAgIVANB0EGOr2YtcxCpTzGgSwE/WgS9OMA0GCSqGSIb3DQEB
CwUAME0xSzBJBgNVBAsMQkFtYXpvbiBXZWIgU2VydmljZXMgTz1BbWF6b24uY29t
IEluYy4gTD1TZWF0dGxlIFNUPVdhc2hpbmd0b24gQz1VUzAeFw0yNDA1MDcxNjM4
MTVaFw00OTEyMzEyMzU5NTlaMB4xHDAaBgNVBAMME0FXUyBJb1QgQ2VydGlmaWNh
dGUwggEiMA0GCSqGSIb3DQEBAQUAA4IBDwAwggEKAoIBAQCp9WVxq/6hAlWqMgtZ
5WjWO2rthKQh3X35mJRQ9fCf7fNXZKTrD3nxKSjvlpSNIZMVOTTcPOjeXfNHD5Pu
f9BBvvmkeDSLbTtDFL0OOjVomOAy0t9j7zEK3TiTCNPcBHQAmFW+PCDg3FooP75d
tTqmtXaM/MAU/Ucfg1GXQsZoCVi44s17m9Khad+oCc2xhBWNYX7cXDN/rG0pRC3O
q6mMpTIphYdLTiVyuluhr4cGN4bz8Y0B+SMFLgHzETDV0GvUyXjX0eehhadrTie5
2sVfT+Wa+Djz4TInnCOhQwOvlv2Ff9jFPD0tfhmAkVz4BGDrHkaanVtx5Dn0cRkt
uA93AgMBAAGjYDBeMB8GA1UdIwQYMBaAFAZTF8jtncHMUCTKtjEay/g+LT1nMB0G
A1UdDgQWBBS/KG6rOLym2AFvWzsfHTvnLucSIjAMBgNVHRMBAf8EAjAAMA4GA1Ud
DwEB/wQEAwIHgDANBgkqhkiG9w0BAQsFAAOCAQEAetXz/qxhBmFgt1+bU8lKnHM4
z4NoFSAawxNeanwg0sEduNloHVzcq3mIZqsbaNoR4hhQF3XrK7VjTjr478Fp9BGH
t2k9+WzQOLZ4Ejyjp8T2qwMO/LEX1uHNkhdfGIJPzZ5sHJKHdZt6fEN0Hg6DHwOA
ra2lW5fIKZUtemsEyb+L6DA8AL8Iq5bzoMT3/hkBiYpDO0yMu+qyEeXrTZ/Iw2yY
TIW0ajBZS/2Ss0DaKF5LUYMBTb40I75QYoBJqQaDvd2HbzSnGJyf+S7QUmlu9UoW
bw6DWYNTQmHdsmzNT96rHtLgwjy2w0AY1YNGgOTzjTWTqMC0FLIGy+Lr6FgF4g==
-----END CERTIFICATE-----
)KEY";

const char PRIVATE_KEY[] PROGMEM = R"KEY(
-----BEGIN RSA PRIVATE KEY-----
MIIEpAIBAAKCAQEAqfVlcav+oQJVqjILWeVo1jtq7YSkId19+ZiUUPXwn+3zV2Sk
6w958Sko75aUjSGTFTk03Dzo3l3zRw+T7n/QQb75pHg0i207QxS9Djo1aJjgMtLf
Y+8xCt04kwjT3AR0AJhVvjwg4NxaKD++XbU6prV2jPzAFP1HH4NRl0LGaAlYuOLN
e5vSoWnfqAnNsYQVjWF+3Fwzf6xtKUQtzqupjKUyKYWHS04lcrpboa+HBjeG8/GN
AfkjBS4B8xEw1dBr1Ml419HnoYWna04nudrFX0/lmvg48+EyJ5wjoUMDr5b9hX/Y
xTw9LX4ZgJFc+ARg6x5Gmp1bceQ59HEZLbgPdwIDAQABAoIBAQCPsi0s6JCGrSSk
m19BiFcVGnOTkhv7jftetoRf1zC050hg0XUmUJV3MqeruhKrKIHk5dMtikoRCDoS
ELewZFw92ivTxNYYNChw1O3rQ4e4gdi839GD+s3qM5QLrOEDGrjBcAq+prPdqjCH
6y0WqY2HOdGb7m6w0Fv75tlrHEgH2M3GclXlAMeHIt9ibY009upDIuCiO4C1/izu
MM4yB3Uq0OGXhPwbXXK4BdUAmqwPE+adueWCpZNmLGxBsdXXb0JaokUZS/oDCgB8
htaoFwb9eBbvphU9OzywDgJOMeiQphpz0ftaeldgptg6hw7axke0cQj9+ZvT805b
hO6Gbc3RAoGBAN3yY947QSiUyjjgMAVajlUOHxcLUpH/QGcYw+9BavySKFKYIOcN
uOhTc7guiM7ZQrdnXINUdNGpGB243bxDWY7t2Den1y28I1rGbL5EnceXpi29WPOJ
+M5euu+dT+7Pd5aY2AyXeAWQ2/K8QTChTmYH6VKnuaXZvMvh3+AiDD/LAoGBAMQJ
A/30owM8d7Qi0PW5p3Hae5Nlwha8CSPTUp+nPNqdVkZrOnSR2LN93+kLiHWVI8xy
N62iMU22AxXwauFZ6xsPvolPK1xOdq0eX7hbFwSwdw1ikyczjf5BrHoVPwhVfRx1
O6UNuQvmrJa7vda/V3zIdlbZ8CqDRBK9DWuXrWGFAoGBAKlIMUfVJhOPuNW8mKPc
t+bDyI7JAgd9CmAYEYmw+ar+x2aHxcC1RK1eyUpXo90w9KPyYCuDBT7MnCo/UFBB
hlNvsZRxnbuyXbzkzxgoObd8QTFt1CScvXqZ4jbUY50f/HSOy2p/NPn5zOjNEWPI
BIXpztpON1j6Q/e1pg3LqlAVAoGAGFPaF/j1BeDKJiOv9N6eRgXMEiWtqW6aX8TI
2yFA7wxZeZGzBmZXEydK0zvoKldgwu1ltXmMTs2Ion9cdCzZfVk91wJpKiJenetI
C7PaLm3LyHB0PIgsVeJGijv6v4kEZrSxuzDxxuVQg8A2teC29bhW3z5ajJwPpZeq
g7CmBpUCgYBukdumOOMIi7hA5QrElTchOTe7o9Wvnx4X1fSvr5YZwntmRtqHAhzo
ZizKzZYnT53FTXIj9RqHk3HVXGN0CgZRJh5ANpO7v6aD+7Uvt9si3qYctyI3RApO
x12+TpIQGgN8TdkzAR7zpXXb0k629dyAFjvsVUJvovNgx1PyLmvyMQ==
-----END RSA PRIVATE KEY-----
)KEY";

WiFiManager* wiFiManager;
AmazonCredentials* amazonCredentials;
MQTTConfig* mqttConfig;
MQTTManager* mqttManager;

StaticJsonDocument<JSON_OBJECT_SIZE(64)> inputDoc;
StaticJsonDocument<JSON_OBJECT_SIZE(4)> outputDoc;

UltrasonicSensor* ultrasonicSensor = new UltrasonicSensor(27, 26);
WaterPump* waterPump = new WaterPump(13, 12);
Dispenser* dispenser = new Dispenser(ultrasonicSensor, waterPump);

unsigned long previousPublishMillis = 0;

char outputBuffer[128];

void reportCurrentState(const char * state, float distance){
  outputDoc["state"]["reported"]["distanceFromGlass"] = distance;
  outputDoc["state"]["reported"]["pumpState"] = state;
  serializeJson(outputDoc, outputBuffer);
  mqttManager->publish(outputBuffer);
}

void setPumpState(const char * desiredState){
  float distance;
  if(strcmp(desiredState, "off")){
    distance = 11;
    dispenser->turnOffWaterPump();
  }else if(strcmp(desiredState, "on")){
    distance = 9;
    dispenser->turnOnWaterPump();
  }else{
    Serial.println("An error has occurred");
  }
  reportCurrentState(desiredState, distance);
}

void callback(const char * topic, byte * payload, unsigned int length) {
  String message;
  for (int i = 0; i < length; i++) message += String((char) payload[i]);
  
  if (String(topic) == UPDATE_DELTA_TOPIC) {
    String messageTopic = "Message from topic " + String(topic) + ":" + message;
    Serial.println(messageTopic);

    DeserializationError err = deserializeJson(inputDoc, payload);
    if (!err) {
      String desiredState = inputDoc["state"]["pumpState"].as<String>();
      setPumpState(desiredState.c_str());
    }
  }
}

void connect(){
  wiFiManager->connect();
  mqttManager->connect();
  reportCurrentState("off", 1200);
}

void setup() {
  pinMode(LED_BUILTIN, OUTPUT);
  Serial.begin(115200);

  wiFiManager = new WiFiManager(WIFI_SSID, WIFI_PASS);
  amazonCredentials = new AmazonCredentials(ROOT_CA1, CERTIFICATE, PRIVATE_KEY);
  mqttConfig = new MQTTConfig(MQTT_BROKER_HOST, MQTT_BROKER_PORT, MQTT_CLIENT_ID, UPDATE_DELTA_TOPIC, UPDATE_TOPIC, amazonCredentials);
  mqttManager = new MQTTManager(mqttConfig, callback);

}

void loop() {
  if (mqttManager->getStatusWiFiClient()) {
    unsigned long now = millis();
    if (now - previousPublishMillis >= 2000) {
      previousPublishMillis = now;
      float distance = dispenser->readDistanceFromGlass();
      Serial.println(distance);
      reportCurrentState(dispenser->getWaterPumpState(), distance);
    }
    mqttManager->loop();

  }
  else {
    Serial.println("MQTT broker not connected!");
    delay(2000);
    connect();
  }
  

}
