/*
	Name: RGB slider for ESP32-S3 Dev board
	Author: Stephen J. Heilman
	Date: 01/15/2026
	
	This code is used to demostrate web page sliders
	to operate the RGB led on the ESP32-S3 dev board. 
	By moving the red, green, and blue sliders the rgb 
	led on the esp32 board as well as the simulated led
	on the web page. You must set up your esp32 to your 
	ssid and password .
*/
#include <Arduino.h>
#include <WiFi.h>
#include <WebServer.h>

const char* ssid = "Your SSID";
const char* password = "Your Password";

WebServer server(80);

/*
const int redPin = RGB_BUILTIN;
const int greenPin = 2;
const int bluePin = 4;
*/
const char* htmlPage = R"rawliteral(
<!DOCTYPE html>
<html>
<head>
  <title>ESP32 RGB LED Control</title>

<!-- Set up sliders with same color background and yellow thumb -->

  <style>
    body { background-color: white; }


  .sliderRed { -webkit-appearance: none; value="0"; margin: 14px; width: 800px; height: 15px; border-radius: 5px; background: #FF0000; outline: none; -webkit-transition: .2s; transition: opacity .2s;}
  .sliderRed::-webkit-slider-thumb {-webkit-appearance: none; appearance: none; width: 25px; height: 25px; border-radius: 12px; background: yellow; cursor: pointer;}
  .sliderRed::-moz-range-thumb { width: 25px; height: 25px; border-radius: 12px; background: #F74D4D; cursor: pointer; } 

  .sliderGreen { -webkit-appearance: none; margin: 14px; width: 800px; height: 15px; border-radius: 5px; background: #00ff00; outline: none; -webkit-transition: .2s; transition: opacity .2s;}
  .sliderGreen::-webkit-slider-thumb {-webkit-appearance: none; appearance: none; width: 25px; height: 25px; border-radius: 12px; background: yellow; cursor: pointer;}
  .sliderGreen::-moz-range-thumb { width: 25px; height: 25px; border-radius: 12px; background: #F74D4D; cursor: pointer; } 

  .sliderBlue { -webkit-appearance: none; margin: 14px; width: 800px; height: 15px; border-radius: 5px; background: #0000ff; outline: none; -webkit-transition: .2s; transition: opacity .2s;}
  .sliderBlue::-webkit-slider-thumb {-webkit-appearance: none; appearance: none; width: 25px; height: 25px; border-radius: 12px; background: yellow; cursor: pointer;}
  .sliderBlue::-moz-range-thumb { width: 25px; height: 25px; border-radius: 12px; background: #F74D4D; cursor: pointer; } 

  .spanClass { font-size: 30px; }

  .cButton { -webkit-appearance: none; margin-left: 400px; margin-top: 20px; width: 30px; height: 30px; border-radius: 15px; background: #777777; display: block; outline: none; -webkit-transition: .2s; transition: opacity .2s;}
  
  </style>
</head>
<body>
  <h1>RGB LED Color Control</h1>

<!-- Display Red slider -->
  <label for="red">Red:</label><br>
  <input type="range" id="red" class="sliderRed" min="0" max="255" value="0" oninput="updateColor()">
  <span id="sliderRedvalue" class= "spanClass">0</span><br>

<!-- Display Green slider -->
  <label for="green">Green:</label><br>
  <input type="range" id="green" class="sliderGreen" min="0" max="255" value="0" oninput="updateColor()">
  <span id="sliderGreenvalue" class= "spanClass">0</span><br>

<!-- Display Blue slider -->
  <label for="blue">Blue:</label><br>
  <input type="range" id="blue" class="sliderBlue" min="0" max="255" value="0" oninput="updateColor()">
  <span id="sliderBluevalue" class= "spanClass">0</span><br>

<!-- Display a button to be used like an LED -->  
  <input type="button" id="cButton" name="cButton" class="cButton"  margin = "auto";><br><br>

  <script>
    function updateColor() {
      const redSlider = document.getElementById('red');
      const greenSlider = document.getElementById('green');
      const blueSlider = document.getElementById('blue');
      const cButton = document.getElementById("cButton");

      const redValueSpan = document.getElementById('sliderRedvalue');
      const greenValueSpan = document.getElementById('sliderGreenvalue');
      const blueValueSpan = document.getElementById('sliderBluevalue');

      // Update span text content with corresponding slider values
      redValueSpan.textContent = redSlider.value;
      greenValueSpan.textContent = greenSlider.value;
      blueValueSpan.textContent = blueSlider.value;

      // Prepare RGB values for sending to server
      var r = redSlider.value;
      var g = greenSlider.value;
      var b = blueSlider.value;
	  
      // Change button to work like LED by changing color to what is selected by sliders
      cButton.style.backgroundColor = `rgb(${r}, ${g}, ${b})`; 

      // Send RGB values via XMLHttpRequest
      var xhr = new XMLHttpRequest();
      xhr.open("GET", `/setColor?r=${r}&g=${g}&b=${b}`, true);
      xhr.send();
    }

    // Initialize on page load
    updateColor();
  </script>
</body>
</html>
)rawliteral";

void handleRoot() {
  server.send(200, "text/html", htmlPage);
}

void handleSetColor() {
  if (server.hasArg("r") && server.hasArg("g") && server.hasArg("b")) {
    // retrieve RGB values returned from Webpage
    int r = server.arg("r").toInt();
    int g = server.arg("g").toInt();
    int b = server.arg("b").toInt();

  rgbLedWrite(RGB_BUILTIN, r, g, b);

    server.send(200, "text/plain", "OK");
  } else {
    server.send(400, "text/plain", "Bad Request");
  }
}

void setup() {
  Serial.begin(115200);

  // Start WiFi
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.print("WiFi connected ");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());


  server.on("/", handleRoot);
  server.on("/setColor", handleSetColor);

  server.begin();
  Serial.println("HTTP server started");

  rgbLedWrite(RGB_BUILTIN, 0, 0, 0);  
}

void loop() {
  server.handleClient();
}

