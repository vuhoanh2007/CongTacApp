#include <FS.h>
#include <WiFiManager.h>
#include <Firebase_ESP_Client.h>
#include <addons/TokenHelper.h>

// Firebase credentials
#define API_KEY "AIzaSyCZzDcsuKPxjkpzJEcw38QdOKIkWHip0oI"
#define FIREBASE_PROJECT_ID "iot-devices-c0515"

#define USER_EMAIL "vuthaohoanh2007@gmail.com"
#define USER_PASSWORD "hoanhpro123"

const char* BASE_PATH = "/Congtac/";
const uint8_t D1_PIN = D1;
const uint8_t D2_PIN = D2;
const uint8_t D3_PIN = D5;
const uint8_t D4_PIN = D6;

// WiFiManager
WiFiManager wifiManager;
FirebaseJson payload;
FirebaseJsonData jsonData;
FirebaseConfig config;
FirebaseData fbdo;
FirebaseAuth auth;

void setup() {
  Serial.begin(115200);
  // Connect to WiFi
  if (!wifiManager.autoConnect("11A1-THPT_TRANPHU")) {
    Serial.println("Failed to connect and hit timeout");
    delay(3000);
    // Reset and try again
    ESP.reset();
    delay(5000);
  }
  pinMode(D1_PIN, OUTPUT); 
  pinMode(D2_PIN, OUTPUT); 
  pinMode(D3_PIN, OUTPUT); 
  pinMode(D4_PIN, OUTPUT); 

  auth.user.email = USER_EMAIL;
  auth.user.password = USER_PASSWORD;
  config.token_status_callback = tokenStatusCallback;
  Firebase.reconnectNetwork(true);

  Serial.println("Connected to WiFi");
  config.api_key = API_KEY;

  // Initialize Firebase
  Firebase.begin(&config, &auth); 
}

int i = 1;
int a[5] = {0,5,4,14,12};

void loop() {
  readFirestoreData(i, a[i]);
  i++;
  if (i>4) i = 1;
}

void readFirestoreData(const int& document, const uint8_t& num) {
  FirebaseJson payload;
  String path = BASE_PATH + String("D") + String(document);
  String mask = "state";
  if (Firebase.Firestore.getDocument(&fbdo, FIREBASE_PROJECT_ID, "", path.c_str(), mask.c_str())){
            payload.setJsonData(fbdo.payload().c_str());
            payload.get(jsonData, "fields/state/booleanValue", true);
            bool currentState = jsonData.boolValue; 

            digitalWrite(num, currentState ? HIGH : LOW);
            
  }
}

