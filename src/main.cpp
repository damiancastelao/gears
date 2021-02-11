#include <Arduino.h>
#include <gears.h>

int motores[]={13, 12, 14, 27, 26, 33, 32};

AsyncWebServer server(80);

const char* ssid = "wifi";
const char* password = "perlitalagatita";
const char * hostName = "esp-gears";

const char* PARAM_MESSAGE = "gpio";

void notFound(AsyncWebServerRequest *request) {
    request->send(404, "text/plain", "Not found");
}

String outputState(int output){
  if(digitalRead(output)){
    return "checked";
  }
  else {
    return "";
  }
}

// Replaces placeholder with button section in your web page
String processor(const String& var){
  //Serial.println(var);
  if(var == "BUTTONPLACEHOLDER"){
    String buttons = "";
    buttons += "<h4>Output - GPIO 12</h4><label class=\"switch\"><input type=\"checkbox\" onchange=\"toggleCheckbox(this)\" id=\"12\" " + outputState(12) + "><span class=\"slider\"></span></label>";
    buttons += "<h4>Output - GPIO 14</h4><label class=\"switch\"><input type=\"checkbox\" onchange=\"toggleCheckbox(this)\" id=\"14\" " + outputState(14) + "><span class=\"slider\"></span></label>";
    buttons += "<h4>Output - GPIO 27</h4><label class=\"switch\"><input type=\"checkbox\" onchange=\"toggleCheckbox(this)\" id=\"27\" " + outputState(33) + "><span class=\"slider\"></span></label>";
    buttons += "<h4>Output - GPIO 32</h4><label class=\"switch\"><input type=\"checkbox\" onchange=\"toggleCheckbox(this)\" id=\"32\" " + outputState(32) + "><span class=\"slider\"></span></label>";
    buttons += "<h4>Output - GPIO 33</h4><label class=\"switch\"><input type=\"checkbox\" onchange=\"toggleCheckbox(this)\" id=\"33\" " + outputState(33) + "><span class=\"slider\"></span></label>";
    return buttons;
  }
  return String();
}



void setup() {
  Serial.begin(115200);
  Serial.setDebugOutput(true);

for (int i=12; i<34; i++){
    pinMode(i, OUTPUT);
    digitalWrite(i, LOW);
    Serial.print(i);Serial.print(".");
  }

  WiFi.mode(WIFI_MODE_STA);
  WiFi.begin(ssid, password);
  if (WiFi.status() != WL_CONNECTED) {
      delay(500);
      Serial.print(".");
  }

  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
      request->send_P(200, "text/html", index_html, processor);
  });

  // Send a GET request to <ESP_IP>/update?output=<inputMessage1>&state=<inputMessage2>
  server.on("/update", HTTP_GET, [] (AsyncWebServerRequest *request) {
    String inputMessage1;
    String inputMessage2;
    // GET input1 value on <ESP_IP>/update?output=<inputMessage1>&state=<inputMessage2>
    if (request->hasParam(PARAM_INPUT_1) && request->hasParam(PARAM_INPUT_2)) {
      inputMessage1 = request->getParam(PARAM_INPUT_1)->value();
      inputMessage2 = request->getParam(PARAM_INPUT_2)->value();
      digitalWrite(inputMessage1.toInt(), inputMessage2.toInt());
    }
    else {
      inputMessage1 = "No message sent";
      inputMessage2 = "No message sent";
    }
    Serial.print("GPIO: ");
    Serial.print(inputMessage1);
    Serial.print(" - Set to: ");
    Serial.println(inputMessage2);
    request->send(200, "text/plain", "OK");
  });

  // Send a GET request to <IP>/on?gpio=<message>
  server.on("/on", HTTP_GET, [] (AsyncWebServerRequest *request) {
      String message;
      int gpio;
      if (request->hasParam(PARAM_MESSAGE)) {
          message = request->getParam(PARAM_MESSAGE)->value();
          gpio = message.toInt();
          digitalWrite(gpio, HIGH);
      } else {
          message = "No gpio sent";
      }
      request->send(200, "text/plain", "ON GPIO: " + message);
      
  });
  
  // Send a GET request to <IP>/off?gpio=<message>
  server.on("/off", HTTP_GET, [] (AsyncWebServerRequest *request) {
      String message;
      int gpio;
      if (request->hasParam(PARAM_MESSAGE)) {
          message = request->getParam(PARAM_MESSAGE)->value();
          gpio = message.toInt();
          digitalWrite(gpio, LOW);
      } else {
          message = "No gpio sent";
      }
      request->send(200, "text/plain", "OFF GPIO: " + message);
      
  });

  // Send a POST request to <IP>/post with a form field message set to <message>
  server.on("/post", HTTP_POST, [](AsyncWebServerRequest *request){
      String message;
      if (request->hasParam(PARAM_MESSAGE, true)) {
          message = request->getParam(PARAM_MESSAGE, true)->value();
      } else {
          message = "No message sent";
      }
      request->send(200, "text/plain", "Hello, POST: " + message);
  });

  server.onNotFound(notFound);

  // Print local IP address and start web server
  server.begin();

  Serial.print("IP AP address: ");
  Serial.println(WiFi.softAPIP());
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
  Serial.print("ESP32 Board MAC Address:  ");
  Serial.println(WiFi.macAddress());
}

void loop() {
  /*for (int i=0; i<7; i++){
    digitalWrite(motores[i], HIGH);
    Serial.print(motores[i]);Serial.print(".");
  }
  delay(5000);
  for (int i=0; i<7; i++){
    digitalWrite(motores[i], LOW);
  }
  delay(2000);
  Serial.println();
  */

}