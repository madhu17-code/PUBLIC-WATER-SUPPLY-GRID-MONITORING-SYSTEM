/*
espasyncwebserver by esp32async 3.7.0 version installed

*/
// Import required libraries
#include "WiFi.h"
#include "ESPAsyncWebServer.h"

// Set to true to define Relay as Normally Open (NO)
#define RELAY_NO    false

// Set number of relays
#define NUM_RELAYS  2

// Assign each GPIO to a relay
int relayGPIOs[NUM_RELAYS] = {4, 15};

// Replace with your network credentials
const char* ssid = "Public_water_grid";
const char* password = "123456789";

const char* PARAM_INPUT_1 = "relay";  
const char* PARAM_INPUT_2 = "state";

int soilsensor_value1, soilsensor_value2, rainsensor_value, flowsensor_value;
int irsensor_value;
int switch_value; // Assuming you have another switch value
String security_status;
String leak_status1, leak_status2, flow_status;
String tankLevel;
int flag10, flag90;
const int IR_PIN = 34; // Change this to your actual pin number
const int SWITCH_PIN = 13; // Change this to your actual pin number
const int SM_PIN = 18;
const int RS_PIN = 19;
const int FS_PIN = 21;
const int SM2_PIN = 5;
int buzzer = 23;
int Level10_pin = 25;
int Level90_pin = 26;

// Create AsyncWebServer object on port 80
AsyncWebServer server(80);

const char index_html[] PROGMEM = R"rawliteral(
<!DOCTYPE HTML><html>
<head>
  <meta name="viewport" content="width=device-width, initial-scale=1">
  <style>
    html {font-family: Arial; display: inline-block; text-align: center;}
    h2 {font-size: 2.0rem;}
    p {font-size: 2.0rem;}
    body {max-width: 600px; margin:0px auto; padding-bottom: 25px;}
    .switch {position: relative; display: inline-block; width: 120px; height: 68px} 
    .switch input {display: none}
    .slider {position: absolute; top: 0; left: 0; right: 0; bottom: 0; background-color: #ccc; border-radius: 34px}
    .slider:before {position: absolute; content: ""; height: 52px; width: 52px; left: 8px; bottom: 8px; background-color: #fff; -webkit-transition: .4s; transition: .4s; border-radius: 68px}
    input:checked+.slider {background-color: #2196F3}
    input:checked+.slider:before {-webkit-transform: translateX(52px); -ms-transform: translateX(52px); transform: translateX(52px)}
  </style>
</head>
<body>
  <h2>PUBLIC WATER SUPPLY GRID MONITORING</h2>
  %BUTTONPLACEHOLDER%
  <p>
    <i class="fas fa-duotone fa-water" style="color:#039e8a;"></i> 
    <span class="dht-labels">Tank Water Level:</span> 
    <span id="tankLevel">%TANKLEVEL%</span>
  </p>
  <p>
    <i class="fas fa-duotone fa-water" style="color:#039e8a;"></i> 
    <span class="dht-labels">Node 1:</span> 
    <span id="leak_status1">%LEAKSTATUS1%</span>
  </p>
  <p>
    <i class="fas fa-duotone fa-water" style="color:#039e8a;"></i> 
    <span class="dht-labels">Node 2:</span> 
    <span id="leak_status2">%LEAKSTATUS2%</span>
  </p>
  <p>
    <span class="dht-labels">Flow Status:</span> 
    <span id="flow_status">%FLOWSTATUS%</span>
  </p>
  <p>
    <span class="dht-labels">Security Status:</span> 
    <span id="security_status">%SECURITYSTATUS%</span>
  </p>
<script>
setInterval(function ( ) 
{
  var xhttp = new XMLHttpRequest();
  xhttp.onreadystatechange = function() 
  {
    if (this.readyState == 4 && this.status == 200) {
      document.getElementById("tank_level").innerHTML = this.responseText;
    }
  };
  xhttp.open("GET", "/tankLevel", true);
  xhttp.send();
}, 1000) ;
setInterval(function ( ) 
{
  var xhttp = new XMLHttpRequest();
  xhttp.onreadystatechange = function() 
  {
    if (this.readyState == 4 && this.status == 200) {
      document.getElementById("leak_status1").innerHTML = this.responseText;
    }
  };
  xhttp.open("GET", "/leakstatus1", true);
  xhttp.send();
}, 1000) ;
setInterval(function ( ) 
{
  var xhttp = new XMLHttpRequest();
  xhttp.onreadystatechange = function() 
  {
    if (this.readyState == 4 && this.status == 200) {
      document.getElementById("leak_status2").innerHTML = this.responseText;
    }
  };
  xhttp.open("GET", "/leakstatus2", true);
  xhttp.send();
}, 1000) ;
setInterval(function ( ) 
{
  var xhttp = new XMLHttpRequest();
  xhttp.onreadystatechange = function() 
  {
    if (this.readyState == 4 && this.status == 200) {
      document.getElementById("flow_status").innerHTML = this.responseText;
    }
  };
  xhttp.open("GET", "/flowstatus", true);
  xhttp.send();
}, 1000) ;
setInterval(function ( ) 
{
  var xhttp = new XMLHttpRequest();
  xhttp.onreadystatechange = function() 
  {
    if (this.readyState == 4 && this.status == 200) {
      document.getElementById("security_status").innerHTML = this.responseText;
    }
  };
  xhttp.open("GET", "/securitystatus", true);
  xhttp.send();
}, 1000) ;
function toggleCheckbox(element) {
  var xhr = new XMLHttpRequest();
  if(element.checked){ xhr.open("GET", "/update?relay="+element.id+"&state=1", true); }
  else { xhr.open("GET", "/update?relay="+element.id+"&state=0", true); }
  xhr.send();
}
</script>
</body>
</html>
)rawliteral";

// Replaces placeholder with button section in your web page
String processor(const String& var)
{
  if(var == "TANKLEVEL")
  {
    return String(tankLevel);
  }
  if(var == "LEAKSTATUS1")
  {
    return String(leak_status1);
  }
  if(var == "LEAKSTATUS2")
  {
    return String(leak_status2);
  }
  if(var == "FLOWSTATUS")
  {
    return String(flow_status);
  }
  if(var == "SECURITYSTATUS")
  {
    return String(security_status);
  }
  if(var == "BUTTONPLACEHOLDER")
  {
   String buttons ="";
   for(int i=1; i<=NUM_RELAYS; i++)
   {
     String relayStateValue = relayState(i);
     buttons+= "<h4>WATER_LINE #" + String(i) + "</h4><label class=\"switch\"><input type=\"checkbox\" onchange=\"toggleCheckbox(this)\" id=\"" + String(i) + "\" "+ relayStateValue +"><span class=\"slider\"></span></label>";
   }
    return buttons;
  }
  return String();
}

String relayState(int numRelay){
  if(RELAY_NO){
    if(digitalRead(relayGPIOs[numRelay-1])){
      return "";
    }
    else {
      return "checked";
    }
  }
  else {
    if(digitalRead(relayGPIOs[numRelay-1])){
      return "checked";
    }
    else {
      return "";
    }
  }
  return "";
}

void setup(){
  // Serial port for debugging purposes
  Serial.begin(115200);
  pinMode(SWITCH_PIN,INPUT_PULLUP); 
  pinMode(buzzer, OUTPUT);
  pinMode(Level10_pin, INPUT_PULLUP);
  pinMode(Level90_pin, INPUT_PULLUP);
  pinMode(IR_PIN, INPUT);
  pinMode(SWITCH_PIN, INPUT_PULLUP);
  pinMode(SM_PIN, INPUT);
  pinMode(RS_PIN, INPUT);
  pinMode(FS_PIN, INPUT_PULLUP);
  pinMode(SM2_PIN, INPUT);
  digitalWrite(buzzer, LOW);
  // Set all relays to off when the program starts - if set to Normally Open (NO), the relay is off when you set the relay to HIGH
  for(int i=1; i<=NUM_RELAYS; i++){
    pinMode(relayGPIOs[i-1], OUTPUT);
    if(RELAY_NO){
      digitalWrite(relayGPIOs[i-1], HIGH);
    }
    else{
      digitalWrite(relayGPIOs[i-1], LOW);
    }
  }
  
  // Connect to Wi-Fi
  WiFi.softAP(ssid, password);
  // while (WiFi.status() != WL_CONNECTED) {
  //   delay(1000);
  //   Serial.println("Connecting to WiFi..");
  // }

  // Print ESP32 Local IP Address
  Serial.println(WiFi.localIP());

  // Route for root / web page
  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send_P(200, "text/html", index_html, processor);
  });

  server.on("/tankLevel", HTTP_GET, [](AsyncWebServerRequest *request)
  {
    request->send(200, "text/plain", String(tankLevel));
  });

  server.on("/leakstatus1", HTTP_GET, [](AsyncWebServerRequest *request)
  {
    request->send(200, "text/plain", String(leak_status1));
  });

   server.on("/leakstatus2", HTTP_GET, [](AsyncWebServerRequest *request)
  {
    request->send(200, "text/plain", String(leak_status2));
  });
  
  server.on("/flowstatus", HTTP_GET, [](AsyncWebServerRequest *request)
  {
    request->send(200, "text/plain", String(flow_status));
  });
  
  server.on("/securitystatus", HTTP_GET, [](AsyncWebServerRequest *request)
  {
    request->send(200, "text/plain", String(security_status));
  });
  
  // Send a GET request to <ESP_IP>/update?relay=<inputMessage>&state=<inputMessage2>
  server.on("/update", HTTP_GET, [] (AsyncWebServerRequest *request) {
    String inputMessage;
    String inputParam;
    String inputMessage2;
    String inputParam2;
    // GET input1 value on <ESP_IP>/update?relay=<inputMessage>
    if (request->hasParam(PARAM_INPUT_1) & request->hasParam(PARAM_INPUT_2)) {
      inputMessage = request->getParam(PARAM_INPUT_1)->value();
      inputParam = PARAM_INPUT_1;
      inputMessage2 = request->getParam(PARAM_INPUT_2)->value();
      inputParam2 = PARAM_INPUT_2;
      if(RELAY_NO){
        Serial.print("NO ");
        digitalWrite(relayGPIOs[inputMessage.toInt()-1], !inputMessage2.toInt());
      }
      else{
        Serial.print("NC ");
        digitalWrite(relayGPIOs[inputMessage.toInt()-1], inputMessage2.toInt());
      }
    }
    else {
      inputMessage = "No message sent";
      inputParam = "none";
    }
    Serial.println(inputMessage + inputMessage2);
    request->send(200, "text/plain", "OK");
  });
  // Start server
  server.begin();
}
  
void loop()
{
  int tank10 = digitalRead(Level10_pin);
  int tank90 = digitalRead(Level90_pin);
  if(tank10 == 1)
  {
    tankLevel = "Empty";
  }
  if(tank10 == 0 && tank90 == 1)
  {
    tankLevel = "Medium";
  }
  if(tank90 == 0)
  {
    tankLevel = "Full";
  }
  soilsensor_value1 = digitalRead(SM_PIN);
  soilsensor_value2 = digitalRead(SM2_PIN);
  rainsensor_value = digitalRead(RS_PIN);
  if(soilsensor_value1 == 0 && rainsensor_value == 1)
  {
   leak_status1 = "Water leaking";
   Serial.println("Water is leaked");
  }
  if(soilsensor_value2 == 0 && rainsensor_value == 1)
  {
   leak_status2 = "Water leaking";
   Serial.println("Water is leaked");
  }
  if(soilsensor_value1 == 1)
  {
   leak_status1 = "No leakage";
   Serial.println("No leakage");
  }
  if(soilsensor_value2 == 1)
  {
   leak_status2 = "No leakage";
   Serial.println("No leakage");
  }
  delay(10);

  flowsensor_value = digitalRead(FS_PIN);
  Serial.println(flowsensor_value);
  if(flowsensor_value==0)
  {
   flow_status = "Water is flowing";
   Serial.println("Water is flowing");
  }
  if(flowsensor_value==1)
  {
   flow_status = "No water flow";
   Serial.println("No water flow");
  }
  delay(10);

  irsensor_value = digitalRead(IR_PIN);
  switch_value = digitalRead(SWITCH_PIN);
  if(irsensor_value == 1 && switch_value == 1)
  {
   security_status = "Unauthorized open";
   Serial.println("Unauthorized open");
   digitalWrite(buzzer, HIGH);
  }
  if(irsensor_value == 1 && switch_value == 0)
  {
   security_status = "Authorized open";
   Serial.println("Authorized open");
   digitalWrite(buzzer, LOW);
  }
  if(irsensor_value == 0)
  {
   security_status = "Closed";
   Serial.println("Closed");
   digitalWrite(buzzer, LOW);
  }
  delay(10);
}
