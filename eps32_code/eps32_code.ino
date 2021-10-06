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
#define FREQ 30000
#define RES 8

IPAddress IP(10, 100, 102, 100);
IPAddress GATEWAY(10, 100, 102, 1);
IPAddress SUBNET(255, 255, 255, 0);
WiFiServer server(PORT);

/*
this function is used to set a pin and a channel together
 */
void setLed(int ch, int pin){
  ledcSetup(ch, FREQ, RES);
  ledcAttachPin(pin, ch);
}

/*
this function is used for connecting to the wifi
 */
void connectToWifi()
{
  while(!WiFi.config(IP, GATEWAY, SUBNET)){delay(1000);}
  while (WiFi.status() != WL_CONNECTED) {delay(1000);}
}

/*
his function is used for changing the led color
 */
void writeRGBVal(uint8_t r, uint8_t g, uint8_t b)
{
    ledcWrite(R_CH,r);
    ledcWrite(G_CH,g);
    ledcWrite(B_CH,b);
}

/*
this function is used for extracting rgb value from the messege
 */
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

void setup(){
  WiFi.begin(SSID,PASSWORD);
  connectToWifi();
  
  setLed(R_CH,R);
  setLed(G_CH,G);
  setLed(B_CH,B);
  
  writeRGBVal(0,0,0);
    
  server.begin();
}
 
void loop(){
  WiFiClient client = server.available();
 
  if (client) {
    while (client.connected()) {
      //getting the messege from the pc
      char msg[9] = {0};
      int i = 0;
      while (client.available()> 0) {
        char c = client.read();
        if (c == ';')
          break;
        msg[i] = c;
        i++;
      }
      //extracting the red value
      int red = extractValue(msg,0,3);
      if (red >= 0 && red <= 256)//if the value is valid
      {
        //extracting all values and updating led
        uint8_t r = red, g = 0, b = 0;
        g = extractValue(msg,3,6);
        b = extractValue(msg,6,9);
        writeRGBVal(r,g,b);
      }
    }
    client.stop();
    //client has disconnected, turnning leds off
    writeRGBVal(0,0,0);
  }
}
