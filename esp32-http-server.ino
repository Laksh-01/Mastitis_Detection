#include <WiFi.h>
#include <PubSubClient.h>
#include <HTTPClient.h>
#include <ArduinoJson.h>
#include "edge-impulse-sdk/classifier/ei_run_classifier.h"
#include "model-parameters/model_metadata.h"
#include "edge-impulse-sdk/dsp/numpy_types.h"
#include <WiFiClientSecure.h>
#include <DHT.h>

// // --- DHT22 Setup ---
// #define DHTPIN 25
// #define DHTTYPE DHT22
// DHT dht(DHTPIN, DHTTYPE);

// // --- Analog Pins for Udder Measurement (Simulated) ---
// #define IUFL_PIN 34
// #define EUFL_PIN 33
// #define IUFR_PIN 32
// // #define EUFR_PIN 26
// // #define IURL_PIN 4
// // #define EURL_PIN 12
// // #define IURR_PIN 13
// #define EURR_PIN 35
// String powerBiApiUrl = "https://api.powerbi.com/beta/bc5b2879-3fac-469a-b8c4-994705bc09d7/datasets/89882c82-6631-483a-8ade-4f406dc63f44/rows?experience=power-bi&key=n9VRdc8BRJzfD5ZJzbprIWlI5dQA%2BrNXvAMVnUcWf6KyETyGXp1KOgV%2FrViK%2B%2BKZyxYUUnihYHRnwC%2Bi3COzwA%3D%3D";

// // --- Wi-Fi + MQTT ---
// const char *ssid = "Wokwi-GUEST";
// const char *password = "";
// const char *mqtt_server = "be55c7a555ca4dd1bd6e5ed37a3e1cfe.s1.eu.hivemq.cloud";
// const int mqtt_port = 8883;
// const char *mqtt_user = "hello";
// const char *mqtt_password = "Laksh@4888";

// // MQTT & TLS
// WiFiClientSecure espClient;
// PubSubClient client(espClient);

// // --- Variables ---
// int Months_after_giving_birth = 1;
// unsigned long lastMsg = 0;

// struct ClassificationResult {
//   String label;
//   float confidence;
// };

// ClassificationResult classifyMastitis(
//   int Months_after_giving_birth, int IUFL, int EUFL, int IUFR, int EUFR,
//   int IURL, int EURL, int IURR, int EURR, int Temperature
// ) {
//   ClassificationResult resultData = { "Healthy", 1.0 };

//   // --- Average Pressure Calculation
//   float avgPressure = (IUFL + EUFL + IUFR + EUFR + IURL + EURL + IURR + EURR) / 8.0;

//   // --- Swelling Check (difference between inner and outer > threshold)
//   bool swellingDetected =
//     abs(IUFL - EUFL) > 50 || abs(IUFR - EUFR) > 50 ||
//     abs(IURL - EURL) > 50 || abs(IURR - EURR) > 50;

//   // --- Temperature Check
//   bool highTemp = Temperature > 39;

//   // --- Combined Decision Logic
//   if (highTemp && avgPressure > 360 && swellingDetected) {
//     resultData.label = "Severe Mastitis Suspected";
//     resultData.confidence = 0.95;
//   }
//   else if ((highTemp && avgPressure > 360) || (swellingDetected && highTemp)) {
//     resultData.label = "Moderate Mastitis Suspected";
//     resultData.confidence = 0.9;
//   }
//   else if (swellingDetected || avgPressure > 360 || avgPressure < 200 || highTemp) {
//     resultData.label = "Mild Mastitis Suspected";
//     resultData.confidence = 0.8;
//   }
//   else {
//     resultData.label = "Cow is Healthy";
//     resultData.confidence = 0.99;
//   }

//   return resultData;
// }







// // --- WiFi ---
// void setup_wifi() {
//   Serial.print("Connecting to ");
//   Serial.println(ssid);
//   WiFi.begin(ssid, password);
//   while (WiFi.status() != WL_CONNECTED) {
//     delay(500);
//     Serial.print(".");
//   }
//   Serial.println("\nWiFi connected. IP: ");
//   Serial.println(WiFi.localIP());
// }

// // --- MQTT ---
// void callback(char *topic, byte *payload, unsigned int length) {
//   Serial.print("Message [");
//   Serial.print(topic);
//   Serial.print("]: ");
//   for (int i = 0; i < length; i++) Serial.print((char)payload[i]);
//   Serial.println();
// }

// void reconnect() {
//   while (!client.connected()) {
//     Serial.print("Attempting MQTT connection...");
//     String clientId = "ESP32Client-" + String(random(0xffff), HEX);
//     if (client.connect(clientId.c_str(), mqtt_user, mqtt_password)) {
//       Serial.println("Connected to MQTT");
//       client.subscribe("/HealthCheck/Subscribe");
//     } else {
//       Serial.print("Failed, rc=");
//       Serial.print(client.state());
//       Serial.println(" try again in 5s");
//       delay(5000);
//     }
//   }
// }

// // --- Setup ---
// void setup() {
//   Serial.begin(115200);
//   setup_wifi();
//   dht.begin();
//   espClient.setInsecure();
//   client.setServer(mqtt_server, mqtt_port);
//   client.setCallback(callback);
// }



// void sendToPowerBI(
//   String timestamp,        // Matches Image: String
//   float temperature,       // Matches Image: Number (float)
//   float IUFL,             // Matches Image: String
//   float EUFL,             // Matches Image: String
//   float IUFR,              // Matches Image: Number (float)
//   float EUFR,              // Matches Image: Number (float)
//   float IURL,              // Matches Image: Number (float)
//   float IURR,              // Matches Image: Number (float)
//   float EURR,              // Matches Image: Number (float)
  
//   int monthsAfterBirth,       // Logically makes more sense (e.g., 1, 2, 3)
//   // --- End Option ---
//   String classification,   // Matches Image: String
//   String confidence         // Matches Image: Number (float)
// ) {
// if (WiFi.status() != WL_CONNECTED) {
//     Serial.println("WiFi not connected. Skipping Power BI send.");
//     return;
// }

// HTTPClient http;
// Serial.print("Attempting POST to Power BI: ");
// Serial.println(powerBiApiUrl);

// if (!http.begin(powerBiApiUrl.c_str())) { // Use .c_str() for safety
//     Serial.println("HTTPClient Begin failed! Check URL/format and WiFi connection.");
//     return;
// }

// http.addHeader("Content-Type", "application/json");

// // --- Create JSON payload MATCHING THE IMAGE ---
// String payload = "[{";
// payload += "\"timestamp\":\"" + timestamp + "\",";
// payload += "\"temperature\":" + String(temperature, 1) + ","; // Send float with 1 decimal place
// payload += "\"IUFL\":\"" +  String(IUFL,1) + "\",";                     // Send String WITH quotes
// payload += "\"EUFL\":\"" + String(EUFL,1) + "\",";                     // Send String WITH quotes
// payload += "\"IUFR\":" + String(IUFR, 1) + ",";
// payload += "\"EUFR\":" + String(EUFR, 1) + ",";
// payload += "\"IURL\":" + String(IURL, 1) + ",";
// payload += "\"IURR\":" + String(IURR, 1) + ",";
// payload += "\"EURR\":" + String(EURR, 1) + ",";
// // --- Option for monthsAfterBirth ---
// // Choose the matching line based on the function signature choice above:
// // payload += "\"monthsAfterBirth\":" + String(monthsAfterBirth, 1) + ","; // If float
// payload += "\"monthsAfterBirth\":" + String(monthsAfterBirth) + ",";    // If int
// // --- End Option ---
// payload += "\"classification\":\"" + classification + "\",";         // Send String WITH quotes
// payload += "\"confidence\":\"" + confidence + "\"";                // Send float with 2 decimal places (e.g., 80.00)
// payload += "}]";

// Serial.println("Sending Payload to Power BI (Matching Image Types):");
// Serial.println(payload);

// int httpResponseCode = http.POST(payload);

// if (httpResponseCode > 0) {
//   Serial.print("Data sent to Power BI successfully. HTTP Response code: ");
//   Serial.println(httpResponseCode);
//   String responsePayload = http.getString();
//   if (responsePayload.length() > 0) {
//       Serial.println("Power BI Response payload:");
//       Serial.println(responsePayload);
//   }
// } else {
//   Serial.print("Error sending data to Power BI. HTTP Error code: ");
//   Serial.println(httpResponseCode);
// }
// http.end();
// }


// String getTimestamp() {
//   struct tm timeinfo;
//   if (!getLocalTime(&timeinfo)) {
//     Serial.println("Failed to obtain time");
//     // Return a default or indicate error, maybe use millis as fallback?
//     // Using millis() converted to String might be acceptable if exact time isn't critical
//     // return String(millis()); // Alternative if proper time fails
//     return "1970-01-01T00:00:00.000Z"; // Default placeholder
//   }
//   char buffer[35];
//   long milliseconds = (millis() % 1000); // Approximate milliseconds
//   snprintf(buffer, sizeof(buffer), "%d-%02d-%02dT%02d:%02d:%02d.%03ldZ",
//            timeinfo.tm_year + 1900, timeinfo.tm_mon + 1, timeinfo.tm_mday,
//            timeinfo.tm_hour, timeinfo.tm_min, timeinfo.tm_sec, milliseconds);
//   return String(buffer);
// }

// // --- Loop ---
// void loop() {
  

//   unsigned long now = millis();
//   if (now - lastMsg > 6000) {
//     lastMsg = now;
//     Serial.println("Configuring time using NTP...");
//     const long gmtOffset_sec = 0;
//     const int daylightOffset_sec = 0;
//     configTime(gmtOffset_sec, daylightOffset_sec, "pool.ntp.org", "time.nist.gov");
//     if (!client.connected()) reconnect();
//     client.loop();


//     float tempRead = dht.readTemperature();
//     // ... handle isnan(tempRead) ...
//     // Keep temperature as float if possible, or cast later
//     float temperature_f = tempRead; // Use a float variable
//     int temperature_i = (int)(tempRead + 0.5); // Integer version if needed elsewhere

//     int Months_after_giving_birth = 1;

//     // --- Use FLOAT variables for sensor values ---
//     float potIUFL = analogRead(IUFL_PIN);
//     float IUFL_val = ((potIUFL / 4095.0) * (380.0 - 160.0)) + 160.0;
//     float potEUFL = analogRead(EUFL_PIN);
//     float EUFL_val = ((potEUFL / 4095.0) * (380.0 - 160.0)) + 160.0;
//     float potIUFR = analogRead(IUFR_PIN);
//     float IUFR_val = ((potIUFR / 4095.0) * (380.0 - 160.0)) + 160.0;
//     float EUFR_val = 250.0; // Use float
//     float IURL_val = 229.0; // Use float
//     float EURL_val = 277.0; // Use float (for classifier)
//     float IURR_val = 170.0; // Use float
//     float potEURR = analogRead(EURR_PIN);
//     float EURR_val = ((potEURR / 4095.0) * (380.0 - 160.0)) + 160.0;


//     // --- Run Classifier (using ints as previously defined) ---
//      ClassificationResult result = classifyMastitis(
//        Months_after_giving_birth, (int)IUFL_val, (int)EUFL_val, (int)IUFR_val, (int)EUFR_val,
//        (int)IURL_val, (int)EURL_val, (int)IURR_val, (int)EURR_val, temperature_i // Pass int temp
//      );

//     String currentTimestamp = getTimestamp();
//     // Prepare the confidence STRING (as used in your call)
//     String confidenceString = String(result.confidence * 100.0, 2);


//     // --- Call sendToPowerBI MATCHING YOUR SNIPPET ---
//     sendToPowerBI(
//       currentTimestamp,     // String
//       temperature_f,        // Float temperature
//       IUFL_val,             // Float IUFL
//       EUFL_val,             // Float EUFL
//       IUFR_val,             // Float IUFR
//       EUFR_val,             // Float EUFR
//       IURL_val,             // Float IURL
//       IURR_val,             // Float IURR
//       EURR_val,             // Float EURR
//       Months_after_giving_birth, // Int
//       result.label,         // String classification (implicit conversion OK)
//       confidenceString      // String confidence (e.g., "80.00")
//     );


//     // Publish vitals
//     client.publish("/HealthCheck/udder/temp", String(temperature_i).c_str());
//     client.publish("/HealthCheck/udder/IUFL", String(IUFL_val).c_str());
//     client.publish("/HealthCheck/udder/EUFL", String(EUFL_val).c_str());
//     client.publish("/HealthCheck/udder/IUFR", String(IUFR_val).c_str());
//     client.publish("/HealthCheck/udder/EUFR", String(EUFR_val).c_str());
//     client.publish("/HealthCheck/udder/IURL", String(IURL_val).c_str());
//     client.publish("/HealthCheck/udder/EURL", String(EURL_val).c_str());
//     client.publish("/HealthCheck/udder/IURR", String(IURR_val).c_str());
//     client.publish("/HealthCheck/udder/EURR", String(EURR_val).c_str());
//     client.publish("/HealthCheck/udder/monthsAfterBirth", String(Months_after_giving_birth).c_str());

//     // Publish ML result
//     client.publish("/HealthCheck/classification", result.label.c_str());
//     client.publish("/HealthCheck/confidence", String(result.confidence * 100).c_str());

//     Serial.println("--- Udder Measurements ---");
//     Serial.println("Months_after_giving_birth: " + String(Months_after_giving_birth));
//     Serial.println("IUFL: " + String(IUFL_val));
//     Serial.println("EUFL: " + String(EUFL_val));
//     Serial.println("IUFR: " + String(IUFR_val));
//     Serial.println("EUFR: " + String(EUFR_val));
//     Serial.println("IURL: " + String(IURL_val));
//     Serial.println("EURL: " + String(EURL_val));
//     Serial.println("IURR: " + String(IURR_val));
//     Serial.println("EURR: " + String(EURR_val));
//     Serial.println("Temperature " + String(temperature_i));



//     Serial.print("Predicted Condition: ");
//     Serial.print(result.label);
//     Serial.print(" (Confidence: ");
//     Serial.print(result.confidence * 100, 2);
//     Serial.println("%)");
//   }
// }




// #include <WiFi.h>           // For WiFi connectivity
// #include <PubSubClient.h>   // For MQTT communication
// #include <HTTPClient.h>     // For sending data to ThingSpeak
// #include <WiFiClientSecure.h> // For MQTT TLS/SSL
// #include <DHT.h>            // For DHT temperature/humidity sensor
// #include "time.h"           // For NTP time synchronization

// --- DHT22 Setup ---
#define DHTPIN 25         // Digital pin connected to the DHT sensor
#define DHTTYPE DHT22     // Type of DHT sensor
DHT dht(DHTPIN, DHTTYPE);

// --- Analog Pins for Udder Measurement ---
// Adjust these pins based on your actual ESP32 wiring
#define IUFL_PIN 34
#define EUFL_PIN 33
#define IUFR_PIN 32
#define EURR_PIN 35       // Pin for EURR (Field 7)
// Other pins might be needed if EUFR, IURL, IURR are not fixed values
// #define EUFR_PIN 26
// #define IURL_PIN 4
// #define IURR_PIN 13

// --- Wi-Fi Configuration ---
const char *ssid = "Wokwi-GUEST"; // Replace with your network SSID if not using Wokwi
const char *password = "";        // Replace with your network password

// --- MQTT Configuration ---
const char *mqtt_server = "be55c7a555ca4dd1bd6e5ed37a3e1cfe.s1.eu.hivemq.cloud"; // Your HiveMQ Cloud broker hostname
const int mqtt_port = 8883;              // Standard port for MQTT over TLS/SSL
const char *mqtt_user = "hello";         // Your HiveMQ username
const char *mqtt_password = "Laksh@4888"; // Your HiveMQ password

WiFiClientSecure espClient; // Use secure client for MQTT TLS/SSL
PubSubClient client(espClient);

// --- ThingSpeak Configuration ---
#define THINGSPEAK_API_KEY "B02JM4D4M73T9NTF" // *** YOUR ACTUAL WRITE API KEY ***
const char* thingspeakServer = "api.thingspeak.com";


String powerBiApiUrl = "https://api.powerbi.com/beta/bc5b2879-3fac-469a-b8c4-994705bc09d7/datasets/89882c82-6631-483a-8ade-4f406dc63f44/rows?experience=power-bi&key=n9VRdc8BRJzfD5ZJzbprIWlI5dQA%2BrNXvAMVnUcWf6KyETyGXp1KOgV%2FrViK%2B%2BKZyxYUUnihYHRnwC%2Bi3COzwA%3D%3D";

// --- Timing Variables ---
unsigned long lastMsg = 0;                 // Timer for main sensor read/classify loop
unsigned long lastThingSpeakUpdate = 0;    // Separate timer for ThingSpeak updates
const long mainLoopInterval = 6000;        // Interval for main loop (milliseconds)
const long thingSpeakUpdateInterval = 20000; // Interval for ThingSpeak (>= 15000ms for free accounts)

// --- Structs ---
// Structure to hold the classification result
struct ClassificationResult {
  String label;
  float confidence;
};

// --- Forward Declarations ---
// Declare functions before they are called if definition is later
String getTimestamp();
void setup_wifi();
void callback(char *topic, byte *payload, unsigned int length);
void reconnect();
ClassificationResult classifyMastitis(int M, int IFL, int EFL, int IFR, int EFR, int IRL, int ERL, int IRR, int ERR, int T);
void sendToThingSpeak(float temp, float iufl, float eufl, float iufr, float eufr, float iurl, float eurr, int months);



// --- Functions ---

/**
 * @brief Simple rule-based classification for Mastitis based on sensor inputs.
 * @param M Months after giving birth
 * @param IFL Inner Udder Front Left pressure/reading
 * @param EFL Outer Udder Front Left pressure/reading
 * @param IFR Inner Udder Front Right pressure/reading
 * @param EFR Outer Udder Front Right pressure/reading
 * @param IRL Inner Udder Rear Left pressure/reading
 * @param ERL Outer Udder Rear Left pressure/reading
 * @param IRR Inner Udder Rear Right pressure/reading
 * @param ERR Outer Udder Rear Right pressure/reading
 * @param T Temperature reading
 * @return ClassificationResult struct with label and confidence.
 */
ClassificationResult classifyMastitis(
  int Months_after_giving_birth, int IUFL, int EUFL, int IUFR, int EUFR,
  int IURL, int EURL, int IURR, int EURR, int Temperature)
{
    ClassificationResult resultData = { "Healthy", 1.0 }; // Default to healthy
    // Basic validation (optional)
    if (Temperature <= 0 || Temperature > 45) {
        Serial.println("Warning: Unreasonable temperature reading for classification.");
    }

    // --- Average Pressure Calculation (Example) ---
    // Note: EURL and IURR are used here, even if not sent to ThingSpeak
    float avgPressure = (IUFL + EUFL + IUFR + EUFR + IURL + EURL + IURR + EURR) / 8.0;

    // --- Swelling Check (Example) ---
    const int swellingThreshold = 50;
    bool swellingDetected =
        abs(IUFL - EUFL) > swellingThreshold || abs(IUFR - EUFR) > swellingThreshold ||
        abs(IURL - EURL) > swellingThreshold || abs(IURR - EURR) > swellingThreshold;

    // --- Temperature Check (Example threshold for cattle) ---
    bool highTemp = Temperature > 39;

    // --- Combined Decision Logic (Example) ---
    if (highTemp && avgPressure > 360 && swellingDetected) {
        resultData = {"Severe Mastitis Suspected", 0.95};
    } else if ((highTemp && avgPressure > 360) || (swellingDetected && highTemp)) {
        resultData = {"Moderate Mastitis Suspected", 0.90};
    } else if (swellingDetected || avgPressure > 360 || avgPressure < 200 || highTemp) {
        // Added check for abnormally low pressure potentially indicating issue
        resultData = {"Mild Mastitis Suspected", 0.80};
    } else {
        resultData = {"Cow is Healthy", 0.99};
    }
    return resultData;
}

/**
 * @brief Connects the ESP32 to the configured Wi-Fi network.
 */
void setup_wifi() {
    delay(10);
    Serial.println();
    Serial.print("Connecting to "); Serial.println(ssid);
    WiFi.mode(WIFI_STA); // Set ESP32 to Wi-Fi Station mode
    WiFi.begin(ssid, password);
    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.print(".");
    }
    Serial.println("\nWiFi connected");
    Serial.print("IP address: "); Serial.println(WiFi.localIP());
}

/**
 * @brief Callback function for handling incoming MQTT messages.
 * @param topic The topic the message arrived on.
 * @param payload The message payload.
 * @param length The length of the payload.
 */
void callback(char *topic, byte *payload, unsigned int length) {
    Serial.print("Message arrived ["); Serial.print(topic); Serial.print("] ");
    String message;
    for (int i = 0; i < length; i++) {
        message += (char)payload[i];
    }
    Serial.println(message);
    // Add logic here if you need to act on received MQTT messages
}

/**
 * @brief Connects/reconnects to the MQTT broker.
 */
void reconnect() {
    // Loop until we're reconnected
    while (!client.connected()) {
        Serial.print("Attempting MQTT connection...");
        String clientId = "ESP32Client-HealthCheck-" + String(random(0xffff), HEX); // Create a random client ID
        // Attempt to connect
        if (client.connect(clientId.c_str(), mqtt_user, mqtt_password)) {
            Serial.println("Connected to MQTT Broker");
            // Subscribe to topics once connected (optional)
            client.subscribe("/HealthCheck/Subscribe"); // Example topic
        } else {
            Serial.print("Failed, rc="); Serial.print(client.state());
            Serial.println(" try again in 5 seconds");
            // Wait 5 seconds before retrying
            delay(5000);
        }
    }
}

/**
 * @brief Gets the current time formatted as an ISO 8601 string.
 * Requires NTP to be configured and synchronized.
 * @return String containing the formatted timestamp or a default value on failure.
 */
String getTimestamp() {
    struct tm timeinfo;
    if (!getLocalTime(&timeinfo)) {
        Serial.println("Failed to obtain time via NTP");
        return "1970-01-01T00:00:00.000Z"; // Return default placeholder
    }
    // Format the time into an ISO 8601 string
    char buffer[35];
    // strftime doesn't reliably handle milliseconds, construct manually
    long milliseconds = (millis() % 1000);
    snprintf(buffer, sizeof(buffer), "%d-%02d-%02dT%02d:%02d:%02d.%03ldZ",
             timeinfo.tm_year + 1900, timeinfo.tm_mon + 1, timeinfo.tm_mday,
             timeinfo.tm_hour, timeinfo.tm_min, timeinfo.tm_sec, milliseconds);
    return String(buffer);
}

/**
 * @brief Sends sensor data to a configured ThingSpeak channel via HTTP GET.
 * @param temp Temperature value (Field 1).
 * @param iufl IUFL value (Field 2).
 * @param eufl EUFL value (Field 3).
 * @param iufr IUFR value (Field 4).
 * @param eufr EUFR value (Field 5).
 * @param iurl IURL value (Field 6).
 * @param eurr EURR value (Field 7).
 * @param months Months after birth value (Field 8).
 */
void sendToThingSpeak(float temp, float iufl, float eufl, float iufr, float eufr, float iurl, float eurr, int months) {
    if (WiFi.status() != WL_CONNECTED) {
        Serial.println("WiFi not connected. Skipping ThingSpeak send.");
        return;
    }

    HTTPClient http; // Use a local instance
    String url = "/update?api_key=";
    url += THINGSPEAK_API_KEY;
    // --- Construct URL based on the specified fields ---
    url += "&field1=" + String(temp, 2);    // Field 1: temperature
    url += "&field2=" + String(iufl, 2);    // Field 2: IUFL
    url += "&field3=" + String(eufl, 2);    // Field 3: EUFL
    url += "&field4=" + String(iufr, 2);    // Field 4: IUFR
    url += "&field5=" + String(eufr, 2);    // Field 5: EUFR
    url += "&field6=" + String(iurl, 2);    // Field 6: IURL
    url += "&field7=" + String(eurr, 2);    // Field 7: EURR
    url += "&field8=" + String(months);   // Field 8: months_after_giving_birth

    // Use HTTP for simplicity with ThingSpeak (HTTPS is also possible but needs WiFiClientSecure)
    String fullUrl = "http://" + String(thingspeakServer) + url;

    Serial.print("Attempting GET to ThingSpeak: ");
    Serial.println(fullUrl);

    if (!http.begin(fullUrl)) { // Use the full URL string directly
         Serial.println("HTTPClient Begin failed! Check URL.");
         return;
    }

    // Send the HTTP GET request
    int httpResponseCode = http.GET();
    String responsePayload = "N/A";

    // Check the response
    if (httpResponseCode > 0) {
        responsePayload = http.getString(); // Get response (entry number or 0)
        Serial.print("HTTP Response code: "); Serial.println(httpResponseCode);
        Serial.print("ThingSpeak Response: "); Serial.println(responsePayload);
        // ThingSpeak returns '0' if the update fails (e.g., rate limit exceeded, bad API key)
        if (responsePayload == "0") {
             Serial.println("ThingSpeak update failed (check API key, ensure interval > 15s).");
        }
    } else {
        Serial.print("Error sending GET request. HTTP Error code: "); Serial.println(httpResponseCode);
        // Use http.errorToString(httpResponseCode).c_str() on ESP32 core v2+ for details
    }

    // Free resources
    http.end();
}



void sendToPowerBI(
  String timestamp,        // Matches Image: String
  float temperature,       // Matches Image: Number (float)
  float IUFL,             // Matches Image: String
  float EUFL,             // Matches Image: String
  float IUFR,              // Matches Image: Number (float)
  float EUFR,              // Matches Image: Number (float)
  float IURL,              // Matches Image: Number (float)
  float IURR,              // Matches Image: Number (float)
  float EURR,              // Matches Image: Number (float)
  
  int monthsAfterBirth,       // Logically makes more sense (e.g., 1, 2, 3)
  // --- End Option ---
  String classification,   // Matches Image: String
  String confidence         // Matches Image: Number (float)
) {
if (WiFi.status() != WL_CONNECTED) {
    Serial.println("WiFi not connected. Skipping Power BI send.");
    return;
}

HTTPClient http;
Serial.print("Attempting POST to Power BI: ");
Serial.println(powerBiApiUrl);

if (!http.begin(powerBiApiUrl.c_str())) { // Use .c_str() for safety
    Serial.println("HTTPClient Begin failed! Check URL/format and WiFi connection.");
    return;
}

http.addHeader("Content-Type", "application/json");

// --- Create JSON payload MATCHING THE IMAGE ---
String payload = "[{";
payload += "\"timestamp\":\"" + timestamp + "\",";
payload += "\"temperature\":" + String(temperature, 1) + ","; // Send float with 1 decimal place
payload += "\"IUFL\":\"" +  String(IUFL,1) + "\",";                     // Send String WITH quotes
payload += "\"EUFL\":\"" + String(EUFL,1) + "\",";                     // Send String WITH quotes
payload += "\"IUFR\":" + String(IUFR, 1) + ",";
payload += "\"EUFR\":" + String(EUFR, 1) + ",";
payload += "\"IURL\":" + String(IURL, 1) + ",";
payload += "\"IURR\":" + String(IURR, 1) + ",";
payload += "\"EURR\":" + String(EURR, 1) + ",";
// --- Option for monthsAfterBirth ---
// Choose the matching line based on the function signature choice above:
// payload += "\"monthsAfterBirth\":" + String(monthsAfterBirth, 1) + ","; // If float
payload += "\"monthsAfterBirth\":" + String(monthsAfterBirth) + ",";    // If int
// --- End Option ---
payload += "\"classification\":\"" + classification + "\",";         // Send String WITH quotes
payload += "\"confidence\":\"" + confidence + "\"";                // Send float with 2 decimal places (e.g., 80.00)
payload += "}]";

Serial.println("Sending Payload to Power BI (Matching Image Types):");
Serial.println(payload);

int httpResponseCode = http.POST(payload);

if (httpResponseCode > 0) {
  Serial.print("Data sent to Power BI successfully. HTTP Response code: ");
  Serial.println(httpResponseCode);
  String responsePayload = http.getString();
  if (responsePayload.length() > 0) {
      Serial.println("Power BI Response payload:");
      Serial.println(responsePayload);
  }
} else {
  Serial.print("Error sending data to Power BI. HTTP Error code: ");
  Serial.println(httpResponseCode);
}
http.end();
}


// --- Setup Function ---
void setup() {
    Serial.begin(115200);
    while (!Serial); // Wait for serial connection (optional)
    Serial.println("Mastitis Detection System Starting...");

    setup_wifi(); // Connect to WiFi

    // Configure NTP Time ONCE after WiFi connected
    Serial.println("Configuring time using NTP...");
    // Set timezone to UTC (GMT+0) with no daylight saving offset
    configTime(0, 0, "pool.ntp.org", "time.nist.gov");
    delay(1000); // Give NTP a moment to potentially sync

    dht.begin(); // Initialize DHT sensor

    // --- MQTT Setup ---
    Serial.println("Setting up MQTT...");
    // WARNING: setInsecure() disables certificate validation - ONLY for testing/Wokwi
    // For production, load the appropriate CA certificate for your MQTT broker
    espClient.setInsecure();
    client.setServer(mqtt_server, mqtt_port);
    client.setCallback(callback);

    Serial.println("Setup Complete.");
}


// --- Main Loop ---
void loop() {
    // Maintain MQTT Connection (check before blocking loops)
    if (!client.connected()) {
        reconnect();
    }
    client.loop(); // Allow the MQTT client to process messages

    // --- Timed Actions ---
    unsigned long now = millis();

    // --- Main processing block (every `mainLoopInterval` ms) ---
    if (now - lastMsg > mainLoopInterval) {
        lastMsg = now;
        // Serial.println("\n--- Reading Sensors and Classifying ---"); // Reduce serial noise if desired

        // --- Read Sensors ---
        float tempRead = dht.readTemperature();
        float temperature_f; // Value for ThingSpeak Field 1
        int temperature_i;   // Integer value for classifier
        if (isnan(tempRead) || tempRead < -20 || tempRead > 60 ) { // Basic plausibility check
            Serial.println("Failed/Invalid read from DHT sensor!");
            // Use a default/error value or skip this cycle's update
            temperature_f = -99.0; // Or some other indicator of error
            temperature_i = -99;
        } else {
            temperature_f = tempRead;
            temperature_i = (int)(temperature_f + 0.5); // Round for classifier
        }

        int Months_after_giving_birth = 1; // Example value, Field 8

        // --- Read/Calculate Udder Values (use FLOAT) ---
        float potIUFL = analogRead(IUFL_PIN); float IUFL_val = ((potIUFL / 4095.0) * (380.0 - 160.0)) + 160.0; // Field 2
        float potEUFL = analogRead(EUFL_PIN); float EUFL_val = ((potEUFL / 4095.0) * (380.0 - 160.0)) + 160.0; // Field 3
        float potIUFR = analogRead(IUFR_PIN); float IUFR_val = ((potIUFR / 4095.0) * (380.0 - 160.0)) + 160.0; // Field 4
        float EUFR_val = 250.0; // Fixed example value, Field 5
        float IURL_val = 229.0; // Fixed example value, Field 6
        float EURL_val = 277.0; // Needed for classifier
        float IURR_val = 170.0; // Needed for classifier
        float potEURR = analogRead(EURR_PIN); float EURR_val = ((potEURR / 4095.0) * (380.0 - 160.0)) + 160.0; // Field 7


        // --- Run Classifier ---
        // Pass integer versions of sensor data to the classifier
        ClassificationResult result = classifyMastitis(
            Months_after_giving_birth, (int)IUFL_val, (int)EUFL_val, (int)IUFR_val, (int)EUFR_val,
            (int)IURL_val, (int)EURL_val, (int)IURR_val, (int)EURR_val, temperature_i
        );




        // Prepare confidence string (optional, not sent to ThingSpeak fields)
        String confidenceString = String(result.confidence * 100.0, 2);

        // Serial print results (optional)
        Serial.printf("Temp: %.1fC, IUFL: %.1f, EUFL: %.1f, IUFR: %.1f, EUFR: %.1f, IURL: %.1f, EURR: %.1f, Months: %d\n",
                      temperature_f, IUFL_val, EUFL_val, IUFR_val, EUFR_val, IURL_val, EURR_val, Months_after_giving_birth);
        Serial.print("Predicted: "); Serial.print(result.label); Serial.print(" ("); Serial.print(confidenceString); Serial.println("%)");

        String currentTimestamp = getTimestamp();


        sendToPowerBI(
                currentTimestamp,     // String
                temperature_f,        // Float temperature
                IUFL_val,             // Float IUFL
                EUFL_val,             // Float EUFL
                IUFR_val,             // Float IUFR
                EUFR_val,             // Float EUFR
                IURL_val,             // Float IURL
                IURR_val,             // Float IURR
                EURR_val,             // Float EURR
                Months_after_giving_birth, // Int
                result.label,         // String classification (implicit conversion OK)
                confidenceString      // String confidence (e.g., "80.00")
              );

        // --- Publish ALL Vitals via MQTT (if needed) ---
        if (client.connected()) {
            // Publish floats with desired precision
            client.publish("/HealthCheck/udder/temp", String(temperature_f, 1).c_str());
            client.publish("/HealthCheck/udder/IUFL", String(IUFL_val, 1).c_str());
            client.publish("/HealthCheck/udder/EUFL", String(EUFL_val, 1).c_str());
            client.publish("/HealthCheck/udder/IUFR", String(IUFR_val, 1).c_str());
            client.publish("/HealthCheck/udder/EUFR", String(EUFR_val, 1).c_str());
            client.publish("/HealthCheck/udder/IURL", String(IURL_val, 1).c_str());
            client.publish("/HealthCheck/udder/EURL", String(EURL_val, 1).c_str()); // Publish EURL
            client.publish("/HealthCheck/udder/IURR", String(IURR_val, 1).c_str()); // Publish IURR
            client.publish("/HealthCheck/udder/EURR", String(EURR_val, 1).c_str());
            client.publish("/HealthCheck/udder/monthsAfterBirth", String(Months_after_giving_birth).c_str());
            // Publish ML result
            client.publish("/HealthCheck/classification", result.label.c_str());
            client.publish("/HealthCheck/confidence", confidenceString.c_str());
        } else {
            Serial.println("MQTT not connected. Skipping MQTT publish.");
        }
        // Serial.println("---------------------------------"); // Separator if desired

    } // End of main 6-second block


    // --- Send data to ThingSpeak based on its separate interval ---
    // Check if it's time to send data to ThingSpeak
    if (now - lastThingSpeakUpdate > thingSpeakUpdateInterval) {
        lastThingSpeakUpdate = now; // Reset the ThingSpeak timer

        Serial.println("--- Sending Data to ThingSpeak ---");

        // --- Get CURRENT values specifically for ThingSpeak ---
        // Re-reading sensors ensures the data sent to ThingSpeak is fresh
        float ts_temperature_f = dht.readTemperature();
        if (isnan(ts_temperature_f) || ts_temperature_f < -20 || ts_temperature_f > 60) {
             Serial.println("Failed/Invalid DHT read for ThingSpeak, sending default -99");
             ts_temperature_f = -99.0;
        }

        int ts_Months_after_giving_birth = 1; // Get current value if it changes

        float ts_potIUFL = analogRead(IUFL_PIN); float ts_IUFL_val = ((ts_potIUFL / 4095.0) * (380.0 - 160.0)) + 160.0;
        float ts_potEUFL = analogRead(EUFL_PIN); float ts_EUFL_val = ((ts_potEUFL / 4095.0) * (380.0 - 160.0)) + 160.0;
        float ts_potIUFR = analogRead(IUFR_PIN); float ts_IUFR_val = ((ts_potIUFR / 4095.0) * (380.0 - 160.0)) + 160.0;
        float ts_EUFR_val = 250.0; // Use the same fixed value or re-read if applicable
        float ts_IURL_val = 229.0; // Use the same fixed value or re-read if applicable
        float ts_potEURR = analogRead(EURR_PIN); float ts_EURR_val = ((ts_potEURR / 4095.0) * (380.0 - 160.0)) + 160.0;

        // --- Call ThingSpeak Function with the 8 required fields ---
        sendToThingSpeak(
            ts_temperature_f,   // Field 1
            ts_IUFL_val,        // Field 2
            ts_EUFL_val,        // Field 3
            ts_IUFR_val,        // Field 4
            ts_EUFR_val,        // Field 5
            ts_IURL_val,        // Field 6
            ts_EURR_val,        // Field 7
            ts_Months_after_giving_birth // Field 8
        );
         Serial.println("---------------------------------"); // Separator
    } // End of ThingSpeak update block

} // End of loop