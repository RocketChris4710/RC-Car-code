# RC Car Project
###### tags: `ESP32-CAM` `Arduino` `RC Car`

## Links
### [Code in Github](https://github.com/SpiderZoomX/RC-Car-code/blob/master/RC-car.ino)
### [ESP32-CAM Intro](/i70c47OYTjuR84NdEAQp-A)

## Steps
 1) 繪圖 => 雷射切割木板
 2) 測試馬達
 3) 馬達焊上電線
 4) 鎖入電橋
 5) 木板黏上馬達
 6) ESP32-CAM編程
 7) Test
 8) Wi-Fi controlling
 9) BB bullet ejector
 10) ESP32接上馬達，雷射，揚聲器，LED (Assemble)
 11) 測試

## Turning
兩輪反向轉
```java
    delay((角度/15)*t/2)
```
好處: 轉動後仍在原地

## Code Structure
![](https://i.imgur.com/S8ALabv.png)



```java
//include
const char* ssid = ...;
const char* password = ...;
//(電腦IP)
WebServer server(80);
void startCameraServer();
const char index_html[] PROGMEM = R"rawliteral(
<!DOCTYPE HTML><html>
...
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
....
void setup() {
  //Serial,config
  //pinMode
  server.on("/forward", HTTP_GET, Foward);
  digitalWrite(13,LOW);
  digitalWrite(12,LOW);
  digitalWrite(15,LOW);
  digitalWrite(14,LOW);
}  

void loop() {
  server.handleClient();
  delay(1);
}
```


### header
```cpp
#include <WiFi.h> 
#include <WebServer.h>
```
### html
>In HTML, the <form> tag is used to create an HTML form for user input
```htmlembedded
<form action="/get">
  input1: <input type="text" name="input1">
  <input type="submit" value="Submit">
</form><br>
<form action="/get">
  input2: <input type="text" name="input2">
  <input type="submit" value="Submit">
</form><br>
<form action="/get">
  input3: <input type="text" name="input3">
  <input type="submit" value="Submit">
</form>
```

>**The action attribute specifies where to send the data inserted on the form after pressing submit. In this case, it makes an HTTP GET request to /get?input1=value. The value refers to the text you enter in the input field**

>**The type attribute specifies we want a text input field, and the name attribute specifies the name of the input element**
EX:
type="text"
type="submit"

```htmlembedded
<form action="/get">
  input1: <input type="text" name="input1">
  <input type="submit" value="Submit">
</form>
```

## Code

>connect to the local network in setup() 
```java
Serial.begin(115200);
WiFi.mode(WIFI_STA);
WiFi.begin(ssid, password);
if (WiFi.waitForConnectResult() != WL_CONNECTED) {
  Serial.println("WiFi Failed!");
  return;
}
Serial.println();
Serial.print("IP Address: ");
Serial.println(WiFi.localIP()); //ESP32的IP
```

>send the HTML page to the client
GET: server => client
POST: client => server
```java
server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
  request->send_P(200, "text/html", index_html);
});
```

>Write HTML in Arduino:The Webpage
```htmlmixed
const char index_html[](variable name of HTML Webpage) PROGMEM = R"rawliteral(specialize for html)(
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

void notFound(AsyncWebServerRequest *request) {
  request->send(404, "text/plain", "Not found");
}
```



