#include <WiFi.h>

//wifi
#define SSID "Takila"
#define PASSWORD "88888888"
//server
#define PORT 1234
// the number of the LED pin
#define R 14
#define G 13
#define B 32
//channels
#define R_CH 0
#define G_CH 1
#define B_CH 2
//PWM CONST
#define FREQ 10
#define RES 8

IPAddress IP(10, 100, 102, 100);
IPAddress GATEWAY(10, 100, 102, 1);
IPAddress SUBNET(255, 255, 255, 0);

WiFiServer server(PORT);
void setLed(int ch, int pin){
  ledcSetup(ch, FREQ, RES);
  ledcAttachPin(pin, ch);
}

void connectToWifi()
{
  while(!WiFi.config(IP, GATEWAY, SUBNET)){delay(1000);}
  while (WiFi.status() != WL_CONNECTED) {delay(1000);}
}

void setup(){
  WiFi.begin(SSID,PASSWORD);
  connectToWifi();
  
  setLed(R_CH,R);
  setLed(G_CH,G);
  setLed(B_CH,B);
  
  server.begin();
}

int extractValue(char msg[], int start, int end_at){
  int to_ret = 0;
  for (int i = start; i < end_at; i++){
    int current = msg[i]-'0';
    to_ret += current;
    to_ret *= 10;
  }
  if(to_ret!=0)
    to_ret/=10;
  return to_ret;
}
 
void loop(){
  WiFiClient client = server.available();
 
  if (client) {
    ledcWrite(R_CH,256);
    while (client.connected()) {
      char msg[9] = {0};
      int i = 0;
      while (client.available()> 0) {
        char c = client.read();
        if (c == ';')
          break;
        msg[i] = c;
        i++;
      }
      int red = extractValue(msg,0,3);
      if (red >= 0 && red <= 256)
      {
        ledcWrite(R_CH,red);
        ledcWrite(G_CH,extractValue(msg,3,6));
        ledcWrite(B_CH,extractValue(msg,6,9));
      }
    }
    client.stop();
    ledcWrite(R_CH,0);
    ledcWrite(G_CH,0);
    ledcWrite(B_CH,0);
  }
}
