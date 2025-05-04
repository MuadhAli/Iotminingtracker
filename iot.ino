#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <MFRC522v2.h>
#include <MFRC522DriverSPI.h>
#include <MFRC522DriverPinSimple.h>
#include <MFRC522Debug.h>
#include <DHT.h>
#include <bits/stdc++.h>

// WiFi Credentials
const char* ssid = "Airtel_Zerotouch";
const char* password = "Airtel@123";

// Web server on port 80
ESP8266WebServer server(80);

// RFID Setup
MFRC522DriverPinSimple ss_pin(15); // D8
MFRC522DriverSPI driver{ss_pin};
MFRC522 mfrc522{driver};

// DHT11 Setup
#define DHTPIN 2  // D4
#define DHTTYPE DHT11
#define BUZZER_PIN 5  // D1

DHT dht(DHTPIN, DHTTYPE);

// Card and User Info
String card_list[2] = {"24120DA7", "63C746E4"};
String user_info[2][3] = {
  {"Abdul", "Sector1", "Beta"},
  {"Abdul", "Sector2", "Alpha"}
};
int total_card = sizeof(card_list) / sizeof(card_list[0]);

// Last scanned info
String last_card_id = "";
String last_name = "";
String last_location = "";
String last_block = "";

String helmet_id = "0x1";
bool toggle_24120DA7 = false;  // false = Block 2, true = Block 1
bool toggle_63C746E4 = false;  // false = Not in any block, true = Block 1

struct BlockData {
  String block_id;
  std::vector<String> helmets;
};

std::map<String, std::vector<String>> block_map = {
  {"1", {}},
  {"2", {}}
};

void setup() {
  Serial.begin(115200);
  while (!Serial);

  WiFi.begin(ssid, password);
  Serial.print("Connecting to WiFi");
  

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("\nWiFi Connected!");
  Serial.print("IP Address: ");
  Serial.println(WiFi.localIP());

  mfrc522.PCD_Init();
  dht.begin();
  pinMode(BUZZER_PIN, OUTPUT);
  digitalWrite(BUZZER_PIN, LOW); // Make sure buzzer is off at start


  server.on("/data", []() {
  // Read temperature and humidity
  float temp = dht.readTemperature();
  float hum = dht.readHumidity();

  // Fallback in case of read error
  if (isnan(temp)) temp = -1;
  if (isnan(hum)) hum = -1;

  String json = "{ \"blocks\": {";

  bool first_block = true;
  for (auto& pair : block_map) {
    if (!first_block) json += ",";
    first_block = false;

    json += "\"" + pair.first + "\":[";
    for (size_t i = 0; i < pair.second.size(); i++) {
      if (i > 0) json += ",";
      json += "\"" + pair.second[i] + "\"";
    }
    json += "]";
  }

  json += "}, ";
  json += "\"temperature\": " + String(temp, 1) + ", ";
  json += "\"humidity\": " + String(hum, 1);
  json += " }";

  server.sendHeader("Access-Control-Allow-Origin", "*");
  server.send(200, "application/json", json);
});


  // Add these:
  server.on("/buzzon", []() {
    digitalWrite(BUZZER_PIN, HIGH);
    server.send(200, "text/plain", "Buzzer ON");
    server.sendHeader("Access-Control-Allow-Origin", "*");
  });

  server.on("/buzzoff", []() {
    digitalWrite(BUZZER_PIN, LOW);
    server.send(200, "text/plain", "Buzzer OFF");
    server.sendHeader("Access-Control-Allow-Origin", "*");
  });

  server.begin();
  Serial.println("HTTP server started");
}

void loop() {
  server.handleClient();

  // RFID logic
  if (!mfrc522.PICC_IsNewCardPresent()) return;
  if (!mfrc522.PICC_ReadCardSerial()) return;

  String uidString = "";
  for (byte i = 0; i < mfrc522.uid.size; i++) {
    if (mfrc522.uid.uidByte[i] < 0x10) uidString += "0";
    uidString += String(mfrc522.uid.uidByte[i], HEX);
  }
  uidString.toUpperCase();

  showData(uidString);
  delay(2000);
}

void showData(String card_num) {
  for (int i = 0; i < total_card; i++) {
    if (card_num == card_list[i]) {
      last_card_id = card_num;
      last_name = user_info[i][0];
      last_location = user_info[i][1];
      last_block = user_info[i][2];

      // Clear helmet_id from all blocks first
      for (auto& pair : block_map) {
        pair.second.erase(std::remove(pair.second.begin(), pair.second.end(), helmet_id), pair.second.end());
      }

      String block_key = "";

      if (card_num == "24120DA7") {
        toggle_24120DA7 = !toggle_24120DA7;
        block_key = toggle_24120DA7 ? "1" : "2";
      } 
      else if (card_num == "63C746E4") {
        toggle_63C746E4 = !toggle_63C746E4;
        if (toggle_63C746E4) {
          block_key = "1";  // Add to block 1
        } else {
          block_key = "";   // Remove from all blocks
        }
      } 
      else {
        // Default behavior for any other registered card
        block_key = (last_block == "Alpha") ? "1" : (last_block == "Beta") ? "2" : "";
      }

      if (block_key != "") {
        block_map[block_key].push_back(helmet_id);
      }

      Serial.println("------------");
      Serial.print("Card ID : "); Serial.println(last_card_id);
      Serial.print("Name    : "); Serial.println(last_name);
      Serial.print("Location: "); Serial.println(last_location);
      Serial.print("Block   : "); Serial.println(block_key == "" ? "-" : block_key);
      Serial.println("------------");
      return;
    }
  }

  // Unknown card
  last_card_id = card_num;
  last_name = "Unknown";
  last_location = "-";
  last_block = "-";

  Serial.println("------------");
  Serial.print("Card ID "); Serial.print(card_num);
  Serial.println(" is not registered.");
  Serial.println("------------");
}




// Main UI Page
void handleRoot() {
  String html = "<html><head><title>RFID Status</title></head><body>";
  html += "<h2>RFID Last Scanned Card</h2>";
  html += "<p><strong>Card ID:</strong> " + last_card_id + "</p>";
  html += "<p><strong>Name:</strong> " + last_name + "</p>";
  html += "<p><strong>Location:</strong> " + last_location + "</p>";
  html += "<p><strong>Block:</strong> " + last_block + "</p>";
  html += "<p><a href=\"/temphumidity\">View Temperature & Humidity</a></p>";
  html += "<p><a href=\"/data\">Get JSON Data</a></p>";
  html += "</body></html>";

  server.send(200, "text/html", html);
}

// Temperature and Humidity Page
void handleTempHumidity() {
  float temp = dht.readTemperature();
  float hum = dht.readHumidity();

  if (isnan(temp) || isnan(hum)) {
    server.send(500, "text/plain", "DHT11 sensor read error!");
    return;
  }

  String result = "Temperature: " + String(temp) + " °C\n";
  result += "Humidity: " + String(hum) + " %";
  server.send(200, "text/plain", result);
}