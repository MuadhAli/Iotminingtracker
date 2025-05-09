#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <Servo.h>

// Wi-Fi Credentials
const char* ssid = "Airtel_Zerotouch";
const char* password = "Airtel@123";

ESP8266WebServer server(80);
Servo myservo;

int pressAngle = 0;
int interval = 1000;
bool running = false;
unsigned long lastMoveTime = 0;

void handleStart() {
  if (server.hasArg("angle") && server.hasArg("interval")) {
    pressAngle = constrain(server.arg("angle").toInt(), 0, 180);
    interval = server.arg("interval").toInt();
    running = true;
    server.send(200, "text/plain", "Started with angle: " + String(pressAngle) + ", interval: " + String(interval));
    Serial.println("Servo started.");
  } else {
    server.send(400, "text/plain", "Missing 'angle' or 'interval' parameter.");
  }
}

void handleStop() {
  running = false;
  myservo.write(0); // Reset to rest position
  server.send(200, "text/plain", "Stopped");
  Serial.println("Servo stopped.");
}

void handleRoot() {
  server.send(200, "text/plain", "ESP8266 Ventilator Control Ready. Use /start?angle=60&interval=2000");
}

void setup() {
  Serial.begin(115200);
  myservo.attach(D2);  // Servo connected to D2 (GPIO 4)
  myservo.write(0);    // Initial position

  Serial.print("Connecting to Wi-Fi");
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nWi-Fi Connected!");
  Serial.print("IP Address: ");
  Serial.println(WiFi.localIP());

  // Server routes
  server.on("/", handleRoot);
  server.on("/start", handleStart);
  server.on("/stop", handleStop);
  server.begin();
  Serial.println("HTTP server started.");
}

void loop() {
  server.handleClient();

  if (running && millis() - lastMoveTime >= interval) {
    myservo.write(pressAngle);  // Inhale (press)
    delay(200);                 // Hold
    myservo.write(0);           // Exhale (release)
    lastMoveTime = millis();
  }
}
