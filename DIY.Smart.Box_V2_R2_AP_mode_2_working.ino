#include <ESP8266WebServer.h>  // <WebServer.h> for ESP32  
#include <list>

int led[6] = {5,4,A4,0,A3,2}; //pins that the LEDs are connected to
bool led_status[6] = {false};
bool seq=false;

const char* ssid = "Doris_DIY.Smart.Box";  // choose a name
const char* pass = "Ready4HTL!"; // must be >= 8 characters (at least for the ESP32)!
IPAddress ip(192,168,4,1); // should be 192.168.4.x
IPAddress gateway(192,168,4,1);  // should be 192.168.4.x
IPAddress subnet(255,255,255,0);
ESP8266WebServer server(80);  // WebServer server(80); for ESP32


String StringButtonKinds[] = {"greenButton","redButton","blueButton"};  //manages all sorts of buttons contained in headAndTitle html
String buttonList[8];
String buttonHTMLlist[8];
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
                      "<div align=\"center\">Control three LEDs and get measured value, AP - ModeS</BR></div>"
                      "</BR>"
                      ;


void addStringButton(String text, int kind){
  buttonList[getCurrentI(buttonList)] = "led" + (String)(getCurrentI(buttonList)+1);
  String html = "<a href=\"/led" + (String)getCurrentI(buttonHTMLlist) + "\"><button class=\"button " + StringButtonKinds[kind] + "\">" + text + "</button></a></br>";
  buttonHTMLlist[getCurrentI(buttonHTMLlist)] = html;

}

//all buttons required for the website - after pressed a button gets replaced by it's other version, since they are buttons but no switches         

int getCurrentI(String array[]){
  for(int i = 0;i<(sizeof(array)/sizeof(int));i++){
    if(array[i]==NULL){
      return i;
    }
  }
  return -1;
}

void setup(){
  Serial.begin(9600);
  addStringButton("Led0 On", 0);
  addStringButton("Led0 Off", 1);
  addStringButton("Led1 On", 0);
  addStringButton("Led1 Off", 1);
  addStringButton("Led2 On", 0);
  addStringButton("Led2 Off", 1);
  addStringButton("Led3 On", 0);
  addStringButton("Led3 Off", 1);
  addStringButton("Led4 On", 0);
  addStringButton("Led4 Off", 1);
  addStringButton("Led5 On", 0);
  addStringButton("Led5 Off", 1);

  pinMode(led[0], OUTPUT);
  digitalWrite(led[0], LOW);
  pinMode(led[1], OUTPUT);
  digitalWrite(led[1], LOW);
  pinMode(led[2], OUTPUT);
  digitalWrite(led[2], LOW);
  pinMode(led[3], OUTPUT);
  digitalWrite(led[3], LOW);
  pinMode(led[4], OUTPUT);
  digitalWrite(led[4], LOW);
  pinMode(led[5], OUTPUT);
  digitalWrite(led[5], LOW);
  
  WiFi.softAPConfig(ip, gateway, subnet);
  WiFi.softAP(ssid, pass);
 
  delay(500);
  server.on("/",handleRoot);
  int i = 0;
  Serial.println(getCurrentI(buttonHTMLlist));
  for (const String element : buttonList) {
    Serial.println(element);
    server.on(element, [i](){ledOn(i/2);});
    i++;
  }
  /*server.on("/led0_off", [](){ledOff(0);});
  server.on("/led1_on", [](){ledOn(1);});
  server.on("/led1_off", [](){ledOff(1);});
  server.on("/led2_on", [](){ledOn(2);});
  server.on("/led2_off", [](){ledOff(2);});
  server.on("/ledall_off", [](){ledOn(0);});
  server.on("/ledall_on", [](){ledOn(0);});*/
  server.begin();
}
  
void loop(){
  if(!seq){
    server.handleClient(); 
  }else{      //prototype sequence
    LedOn(0);
    delay(200);
    LedOn(1);
    delay(200);
    LedOn(2);
    delay(200);
    LedOn(3);
    delay(200);
    LedOn(4);
    delay(200);
    LedOn(5);
    delay(200);
    LedOff(0);
    delay(200);
    LedOff(1);
    delay(200);
    LedOff(2);
    delay(200);
    LedOff(3);
    delay(200);
    LedOff(4);
    delay(200);
    LedOff(5);
    delay(200);
  }
  
}
void handleRoot() {
  ledOff(0);
}

void ledOn(int led){
  led_status[led] = true;
  switchLEDAndSend(led, 1);
}

void ledOff(int led){
  led_status[led] = false;
  switchLEDAndSend(led, 0);
}

void seqOn(){
  for(int i=0;i<sizeOf(led_status;i++){
    ledOff(i);
  }
	seq=true;
}

void seqOff(){
  seq=false;
}


void switchLEDAndSend(int num, bool state){
  String message = "";
  message += headAndTitle;
  message += "<div align=\"center\";>";
  
  digitalWrite(led[num], state);
  (led_status[0]==true)?(message += (String)buttonHTMLlist[0]):(message += (String)buttonHTMLlist[1]);
  (led_status[1]==true)?(message += (String)buttonHTMLlist[2]):(message += (String)buttonHTMLlist[3]);
  (led_status[2]==true)?(message += (String)buttonHTMLlist[4]):(message += (String)buttonHTMLlist[5]);
  (led_status[3]==true)?(message += (String)buttonHTMLlist[6]):(message += (String)buttonHTMLlist[7]);


  float measuredValue = analogRead(A0)/1024.0 * 3.3; // adjust for your board
/*  message += "</BR><button class=\"button blueButton\">";
  message += "Voltage [V]: </BR>";
  message += String(measuredValue, 2);
  message += "</button>";
  message += "</div>";
*/
  server.send(200, "text/html", message); 
    
}
