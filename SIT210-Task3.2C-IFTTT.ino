#include <WiFiNINA.h>
#include <BH1750.h>
#include <Wire.h>

// Defining my WiFi credentials
char ssid[] = "Rishi";
char pass[] = "12345678";

WiFiClient client;
BH1750 lightMeter;

// Defining the IFTTT server hostname, path, and query parameters
char   HOST_NAME[] = "maker.ifttt.com";
String PATH_NAME  = "/trigger/Light/with/key/sjanjdnundUENFONDI";
String queryString = "?value1=57&value2=25";

void setup() {
  Serial.begin(9600);
  Serial.print("START");
  while (!Serial);

  // Connect to the WiFi network
  WiFi.begin(ssid, pass);
  Wire.begin();

  // Trying to establish a connection to the server
  while(true) {
    if (client.connect(HOST_NAME, 80)) {
      Serial.println("Connected to server");
      break;
    } else {
      Serial.println("Connection failed");
    }
    delay(500);
  }

  lightMeter.begin();
  Serial.println("Connected to server");
}

void loop() {
  Serial.print("START");

  //light level from the BH1750 sensor
  float lux = lightMeter.readLightLevel();

  // Build the query string with the light level
  queryString += "?value1=";
  queryString += lux;
  Serial.println(queryString);

  // If the light level is greater than 500, send a GET request to the server
  if (lux > 500) {
    client.println("GET " + PATH_NAME + queryString + " HTTP/1.1");
    client.println("Host: " + String(HOST_NAME));
    client.println("Connection: close");
    client.println(); // End HTTP header

    while (client.connected()) {
      if (client.available()) {
        char c = client.read();
        Serial.print(c);
      }
    }

    client.stop();
    Serial.println();
    Serial.println("Disconnected");
  }

  // Reset the query string and wait for a while before the next loop iteration
  queryString = "";
  delay(3000);
}
