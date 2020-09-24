#include <WebServer.h>
#include "esp_camera.h"
#include <WiFi.h>
#define CAMERA_MODEL_AI_THINKER
#include "camera_pins.h"
//const char* ssid = "Cloud-TPE-50";
//const char* password = "s1645s1645";
boolean takeNewPhoto = false;
//const char* ssid = "Genius Monkey";
//const char* password = "enhp4308";
const char* ssid = "CCRoom3";
const char* password = "cgshcgsh";
//(電腦IP)
WebServer server(80);
void startCameraServer();

const char index_html[] PROGMEM = R"rawliteral(
<!DOCTYPE HTML><html>
<head>
  <title>Robot control Web Server</title>
  <meta name="viewport" content="width=device-width, initial-scale=1">
  <style>
 html {
    font-family: Trebuchet MS;
    display: inline-block;
    margin: 0px auto;
    text-align: center;
  }
 h1 {
    color: #0F3376;
    padding: 2vh;
  }
 p {
    font-size: 1.5rem;
  }
 .button {
  border: none;
  color: white;
  padding: 15px 32px;
  text-align: center;
  text-decoration: none;
  display: inline-block;
  font-size: 16px;
  margin: 4px 2px;
  cursor: pointer;
}
.STOP {background-color: #1AFFB0;} 
.CP {background-color: #008CBA;}
.FWD {background-color: #2CEFF0;} 
.BWD {background-color: #FFE121;}
.L {background-color: #CF11FF;}
.R {background-color: #FE2020;}
  

   </style>
</head>
<body>
  <h1>RC Tank</h1>
  <p>
  <a href="/stop"><button class="button STOP">STOP</button></a>
  </p>
  <p>
  <a href="/forward"><button class="button FWD">FORWARD</button></a>
  </P>
  <p>  
  <a href="/left"><button class="button L">LEFT</button></a>
  <a href="/right"><button class="button R">RIGHT</button></a>
  </P>
  <p><a href="/backward"><button class="button BWD">BACKWARD</button></a></p>
</body>
</html>)rawliteral";
void Foward()
{
  digitalWrite(13,HIGH);
  digitalWrite(12,LOW);
  digitalWrite(15,HIGH);
  digitalWrite(14,LOW);
  //server.send(200, "text/html", ); 
}
void Backward()
{
  digitalWrite(13,LOW);
  digitalWrite(12,HIGH);
  digitalWrite(15,LOW);
  digitalWrite(14,HIGH);
  //server.send(200, "text/html", ); 
}
void Stop()
{
  digitalWrite(13,LOW);
  digitalWrite(12,LOW);
  digitalWrite(15,LOW);
  digitalWrite(14,LOW);
  //server.send(200, "text/html", ); 
}
void Left()
{
  digitalWrite(13,HIGH);
  digitalWrite(12,LOW);
  digitalWrite(15,LOW);
  digitalWrite(14,HIGH);
  //server.send(200, "text/html", ); 
}
void Right()
{
  digitalWrite(13,LOW);
  digitalWrite(12,HIGH);
  digitalWrite(15,HIGH);
  digitalWrite(14,LOW);
  //server.send(200, "text/html", ); 
}
void Get()
{
  /*String inputMessage;
  String inputParam;
    // GET input1 value on <ESP_IP>/get?input1=<inputMessage>
    if (request->hasParam(A)) {
      inputMessage = request->getParam(A)->value();
      inputParam = PARAM_INPUT_1;
    }
    Serial.println(inputMessage);
    //request->send(200, "text/html", "HTTP GET request sent to your ESP on input field (" 
                                     + inputParam + ") with value: " + inputMessage +
                                     "<br><a href=\"/\">Return to Home Page</a>");*/
}
void openn()
{
  server.send(200, "text/html", index_html);
}
void handle_NotFound(){
  server.send(404, "text/plain", "Not found");
}
void setup() {
  Serial.begin(115200);
  Serial.setDebugOutput(true);
  Serial.println(); 
  camera_config_t config;
  config.ledc_channel = LEDC_CHANNEL_0;
  config.ledc_timer = LEDC_TIMER_0;
  config.pin_d0 = Y2_GPIO_NUM;
  config.pin_d1 = Y3_GPIO_NUM;
  config.pin_d2 = Y4_GPIO_NUM;
  config.pin_d3 = Y5_GPIO_NUM;
  config.pin_d4 = Y6_GPIO_NUM;
  config.pin_d5 = Y7_GPIO_NUM;
  config.pin_d6 = Y8_GPIO_NUM;
  config.pin_d7 = Y9_GPIO_NUM;
  config.pin_xclk = XCLK_GPIO_NUM;
  config.pin_pclk = PCLK_GPIO_NUM;
  config.pin_vsync = VSYNC_GPIO_NUM;
  config.pin_href = HREF_GPIO_NUM;
  config.pin_sscb_sda = SIOD_GPIO_NUM;
  config.pin_sscb_scl = SIOC_GPIO_NUM;
  config.pin_pwdn = PWDN_GPIO_NUM;
  config.pin_reset = RESET_GPIO_NUM;
  config.xclk_freq_hz = 20000000;
  config.pixel_format = PIXFORMAT_JPEG;
  //init with high specs to pre-allocate larger buffers
  if(psramFound()){
    config.frame_size = FRAMESIZE_UXGA;
    config.jpeg_quality = 10;
    config.fb_count = 2;
  } else {
    config.frame_size = FRAMESIZE_SVGA;
    config.jpeg_quality = 12;
    config.fb_count = 1;
  }

#if defined(CAMERA_MODEL_ESP_EYE)
  pinMode(13, INPUT_PULLUP);
  pinMode(14, INPUT_PULLUP);
#endif

  // camera init
  esp_err_t err = esp_camera_init(&config);
  if (err != ESP_OK) {
    Serial.printf("Camera init failed with error 0x%x", err);
    return;
  }

  sensor_t * s = esp_camera_sensor_get();
  //initial sensors are flipped vertically and colors are a bit saturated
  if (s->id.PID == OV3660_PID) {
    s->set_vflip(s, 1);//flip it back
    s->set_brightness(s, 1);//up the blightness just a bit
    s->set_saturation(s, -2);//lower the saturation
  }
  //drop down frame size for higher initial frame rate
  s->set_framesize(s, FRAMESIZE_QVGA);

#if defined(CAMERA_MODEL_M5STACK_WIDE)
  s->set_vflip(s, 1);
  s->set_hmirror(s, 1);
#endif
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected");

  //startCameraServer();

  Serial.print("Camera Ready! Use 'http://");
  Serial.print(WiFi.localIP());
  Serial.println("' to connect");
  //WiFi.softAP(ssid, password);
  //WiFi.softAPConfig(local_ip, gateway, subnet);
  delay(100);
  server.on("/", HTTP_GET, openn);
  server.on("/forward", HTTP_GET, Foward);
  server.on("/backward", HTTP_GET, Backward);
  server.on("/stop", HTTP_GET, Stop);
  server.on("/shoot", HTTP_GET, Shoot);
  server.on("/right", HTTP_GET, Right);
  server.on("/left", HTTP_GET, Left);
  server.on("/shoot", HTTP_GET, Shoot);
  server.on("/kill", HTTP_GET, Kill);
  server.on("/get", HTTP_GET, Get);
  server.onNotFound(handle_NotFound);
  server.begin();
  Serial.println("HTTP server started");
  //右後
  pinMode(13, OUTPUT); //13 IN1 //12 IN2
  pinMode(12, OUTPUT);
  
  //左後 
  pinMode(15, OUTPUT); //IN3 15 //IN4 14
  pinMode(14, OUTPUT);
  digitalWrite(13,LOW);
  digitalWrite(12,LOW);
  digitalWrite(15,LOW);
  digitalWrite(14,LOW);
}  

void loop() {
  server.handleClient();
  delay(1);
  int dir;
  char LR;
  int dis;
  int v=65; //65cm/s
  double t=0.328659303138; //1圈的秒數

}
