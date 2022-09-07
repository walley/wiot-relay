#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <WiFiUdp.h>
#include <ESP8266HTTPClient.h>
#include <WiFiClient.h>

const int RELAY = 3;
int relay_state = 0;
WiFiClient wifi;
HTTPClient http;

void relay_on()
{
  relay_state = 1;
  digitalWrite(RELAY, LOW);
}

void relay_off()
{
  relay_state = 0;
  digitalWrite(RELAY, HIGH);
}


char * xstrcpy(char * dest, String src)
{
  int i;

  for (i = 0; i < src.length(); i++) {
    dest[i] = src[i];
  }

  dest[i] = '\0';
  return dest;
}

char * get_ip()
{
  char * ip; //[17];
  ip = (char*)malloc(17);
  xstrcpy(ip, WiFi.localIP().toString());
  return ip;
}

int send_data(char * data)
{

  Serial.print("[HTTP] begin\n");
  Serial.printf("[HTTP] data: %s\n", data);
  http.begin(wifi, "http://grezl.eu/wiot/v1/sensor");

  Serial.print("[HTTP] POST\n");
  http.addHeader("Content-Type", "application/x-www-form-urlencoded");
  int http_code = http.POST(data);

  Serial.printf("[HTTP] http code: %d\n", http_code);

  if (http_code > 0) {
    /*   if (http_code == HTTP_CODE_OK) {
         String payload = http.getString();
         Serial.println(payload);
       }
    */
  } else {
    Serial.printf("[HTTP] failed, error: %s\n", http.errorToString(http_code).c_str());
  }

  http.end();
  return http_code;
}

char * get_data(char * uri)
{
  String payload;

  Serial.print("get_data(): GET\n");
  http.begin(wifi, uri);
  http.addHeader("Content-Type", "text/plain");
  int http_code = http.GET();

  Serial.printf("[HTTP] http code: %d\n", http_code);

  if (http_code == HTTP_CODE_OK) {
    payload = http.getString();
    Serial.println("get_data():" + payload);
  } else {
    payload = "err:" + http_code;
    Serial.printf("[HTTP] failed, error: %s\n", http.errorToString(http_code).c_str());
  }

  http.end();

  char * ret = (char*) malloc(20);
  xstrcpy(ret, payload);
  Serial.print("\nget_data() ret:");
  Serial.println(ret);
  return ret;
}

void setup_pins()
{
  pinMode(RELAY, OUTPUT);
  pinMode(10, INPUT_PULLUP);
}

void setup()
{
  setup_pins();
}

void loop()
{
//
//
}
