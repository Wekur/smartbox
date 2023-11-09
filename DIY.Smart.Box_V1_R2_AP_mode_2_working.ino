#include <ESP8266WebServer.h>  // <WebServer.h> for ESP32  
int led[3] = {D1,D2,D3};
bool led_status[3] = {false};
const char* ssid = "Doris_DIY.Smart.Box";  // choose a name
const char* pass = "Ready4HTL!"; // must be >= 8 characters (at least for the ESP32)!
IPAddress ip(192,168,4,1); // should be 192.168.4.x
IPAddress gateway(192,168,4,1);  // should be 192.168.4.x
IPAddress subnet(255,255,255,0);
ESP8266WebServer server(80);  // WebServer server(80); for ESP32
 
String headAndTitle = "<head><style>"
                        //"font-family:Helvetica;"
                        ".button {"
                          "border: none;"
                          "color: white;"
                          "width: 350px;"
                          "padding: 20px;"
                          "text-align: center;"
                          "margin: 20px 200px;"
                          "border-radius:10px;"
                        "}"
                        ".greenButton {background-color: orange; font-size: 64px;}"
                        ".redButton {background-color: grey; font-size: 64px;}"
                        ".blueButton {background-color: blue; font-size: 50px;}"
                      "</style>"
                      "</head><meta http-equiv=\"refresh\" content=\"5\"></head>"
                      "</BR><h1 align=\"center\">Control Your DIY.Smart.Box</h1></div>"
                      //"<div align=\"center\">Control three LEDs and get measured value, AP - ModeS</BR></div>"
                      "</BR>"
                      ;
String led0_1= "<a href=\"/led0_on\"><button class=\"button greenButton\">LED0 On</button></a>";
String led0_0= "<a href=\"/led0_off\"><button class=\"button redButton\">LED0 Off</button></a>";
String led1_1= "</BR><a href=\"/led1_on\"><button class=\"button greenButton\">LED1 On</button></a>";
String led1_0= "</BR><a href=\"/led1_off\"><button class=\"button redButton\">LED1 Off</button></a>";
String led2_1= "</BR><a href=\"/led2_on\"><button class=\"button greenButton\">LED2 On</button></a>";
String led2_0= "</BR><a href=\"/led2_off\"><button class=\"button redButton\">LED2 Off</button></a>";
void setup(){
  pinMode(led[0], OUTPUT);
  digitalWrite(led[0], LOW);
  pinMode(led[1], OUTPUT);
  digitalWrite(led[1], LOW);
  pinMode(led[2], OUTPUT);
  digitalWrite(led[2], LOW);
  
  WiFi.softAPConfig(ip, gateway, subnet); 
  WiFi.softAP(ssid, pass);  
 
  delay(500); 
  server.on("/",handleRoot);
  server.on("/led0_on", led0on);
  server.on("/led0_off", led0off);
  server.on("/led1_on", led1on);
  server.on("/led1_off", led1off);
  server.on("/led2_on", led2on);
  server.on("/led2_off", led2off);
  server.begin();
}
  
void loop(){
  server.handleClient(); 
}
void handleRoot() {
  led0off();
}
void led0on(){
  led_status[0] = true;
  switchLEDAndSend(0,1);
}
void led0off(){
  led_status[0] = false;
  switchLEDAndSend(0,0);
}
void led1on(){
  led_status[1] = true;
  switchLEDAndSend(1,1);
}
void led1off(){
  led_status[1] = false;
  switchLEDAndSend(1,0);
}
void led2on(){
  led_status[2] = true;
  switchLEDAndSend(2,1);
}
void led2off(){
  led_status[2] = false;
  switchLEDAndSend(2,0);
}
void switchLEDAndSend(int num, bool state){
  String message = "";
  message += headAndTitle;
  message += "<div align=\"center\";>";
  
  digitalWrite(led[num], state);
  (led_status[0]==true)?(message += led0_0):(message += led0_1);
  (led_status[1]==true)?(message += led1_0):(message += led1_1);
  (led_status[2]==true)?(message += led2_0):(message += led2_1);


  float measuredValue = analogRead(A0)/1024.0 * 3.3; // adjust for your board
/*  message += "</BR><button class=\"button blueButton\">";
  message += "Voltage [V]: </BR>";
  message += String(measuredValue, 2);
  message += "</button>";
  message += "</div>";
*/
  server.send(200, "text/html", message); 
    
}
