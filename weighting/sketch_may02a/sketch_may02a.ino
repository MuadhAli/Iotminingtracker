#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include "HX711.h"

// WiFi credentials
const char* ssid = "Airtel_Zerotouch";
const char* password = "Airtel@123";
float initialWeight = 0;
bool monitoringStarted = false;
bool halfAlertSent = false;
bool emptyAlertSent = false;

// HX711 circuit wiring
const int LOADCELL_DOUT_PIN = 12;  // D6
const int LOADCELL_SCK_PIN = 13;   // D7
unsigned long lastPrintTime = 0; // Global variable to track last print time

// Buzzer pin
const int BUZZER_PIN = D1;

HX711 scale;
ESP8266WebServer server(80);

void handleRoot() {
  float weight = scale.get_units(5);
  String html = "<!DOCTYPE html><html><head><meta http-equiv='refresh' content='2'/>"
                "<title>Weight Scale</title></head><body>"
                "<h1>Live Weight Reading</h1>"
                "<p>Weight: " + String(weight, 2) + " kg</p>"
                "</body></html>";
  server.send(200, "text/html", html);
}

void handleStartMonitoring() {
  initialWeight = scale.get_units(5);
  monitoringStarted = true;
  halfAlertSent = false;
  emptyAlertSent = false;
  Serial.println("Monitoring started. Initial weight: " + String(initialWeight));
  
  String json = "{ \"initial\": " + String(initialWeight, 2) + " }";
  server.sendHeader("Access-Control-Allow-Origin", "*");
  server.send(200, "application/json", json);
}


void handleStatus() {
  float currentWeight = scale.get_units(5);
  String status = "Monitoring...";

  if (monitoringStarted) {
    float halfWeight = initialWeight / 2.0;
    if (currentWeight <= 0.1) {
      status = "Almost Empty";
    } else if (currentWeight <= halfWeight) {
      status = "Half Finished";
    }
  }

  String json = "{ \"weight\": " + String(currentWeight, 2) + 
                ", \"initial\": " + String(initialWeight, 2) +
                ", \"status\": \"" + status + "\" }";
  server.sendHeader("Access-Control-Allow-Origin", "*");
  server.send(200, "application/json", json);
}



void handleJSON() {
  float weight = scale.get_units(5);
  String json = "{ \"weight\": " + String(weight, 2) + " }";
  server.sendHeader("Access-Control-Allow-Origin", "*");
  server.send(200, "application/json", json);
}


void handleBuzzOn() {
  digitalWrite(BUZZER_PIN, HIGH);
  server.sendHeader("Access-Control-Allow-Origin", "*");
  server.send(200, "text/plain", "Buzzer ON");
}

void handleBuzzOff() {
  digitalWrite(BUZZER_PIN, LOW);
  server.sendHeader("Access-Control-Allow-Origin", "*");
  server.send(200, "text/plain", "Buzzer OFF");
}

void setup() {
  Serial.begin(115200);
  delay(100);

  pinMode(BUZZER_PIN, OUTPUT);
  digitalWrite(BUZZER_PIN, LOW); // start with buzzer OFF

  scale.begin(LOADCELL_DOUT_PIN, LOADCELL_SCK_PIN);
  scale.set_scale(700.507);  // Calibrated value
  scale.tare();

  WiFi.begin(ssid, password);
  Serial.print("Connecting to WiFi...");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nWiFi connected");
  Serial.print("Web server running at: http://");
  Serial.println(WiFi.localIP());

  // Route definitions
  server.on("/", handleRoot);
  server.on("/data", handleJSON);
  server.on("/buzzon", handleBuzzOn);
  server.on("/buzzoff", handleBuzzOff);
  server.on("/start", handleStartMonitoring);
  server.on("/status", handleStatus);



  server.begin();
  Serial.println("HTTP server started");
}

void loop() {
  server.handleClient();

  unsigned long currentTime = millis();
  if (currentTime - lastPrintTime >= 1000) {  // Every 1000 ms (1 second)
    float weight = scale.get_units(5);       // Read weight from scale
    Serial.print("Weight: ");
    Serial.print(weight, 2);                 // Print with 2 decimal places
    Serial.println(" kg");
    lastPrintTime = currentTime;             // Update last print time
  }
  
  if (monitoringStarted) {
  float currentWeight = scale.get_units(5);
  float halfWeight = initialWeight / 2.0;

  if (!halfAlertSent && currentWeight <= halfWeight) {
    Serial.println("Alert: Half finished!");
    digitalWrite(BUZZER_PIN, HIGH);
    delay(500);
    digitalWrite(BUZZER_PIN, LOW);
    halfAlertSent = true;
  }

  if (!emptyAlertSent && currentWeight <= 0.1) {
    Serial.println("Alert: IV empty!");
    digitalWrite(BUZZER_PIN, HIGH);
    delay(1000);
    digitalWrite(BUZZER_PIN, LOW);
    emptyAlertSent = true;
  }
}

}
