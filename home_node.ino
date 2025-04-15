// #include <WiFi.h>
// #include <esp_wifi.h>
// #include <ESPAsyncWebServer.h>
// #include <esp_now.h>
// #include <LittleFS.h>

// //struct to receive messages from nodes
// typedef struct struct_message {
//   bool node_identifier;  // 0 = air | 1 = water
//   char location[10];
//   float temperature;
//   float humi_turb;
//   float co_ph;
//   float o3_tds;
//   float no2_orp;
//   float voc_dos;
//   float pm25_xx;
// } struct_message;
// struct_message myData;

// // WiFi and server setup
// // To run server: ngrok http 192.168.2.75:80 --host-header=rewrite
// AsyncWebServer server(80);
// AsyncWebSocket ws("/ws");  // WebSocket endpoint
// const char *ssid = "bingbong";
// const char *password = "bongbing123";
// // const char *ssid = "DBM 2.0";
// // const char *password = "bddd1234";
// // const char *ssid = "engbois";
// // const char *password = "dejkwmms43#";
// // const char* ssid ="Dev Mistry's iPhone";
// // const char* password = "12345678";

// //Custom MAC address
// uint8_t newMACAddress[] = { 0x32, 0xAE, 0xA4, 0x07, 0x0D, 0x66 };

// // Global Variables to store received data
// char airNode_location[10] = "Unknown";
// float co = 0;
// float voc = 0;
// float pm25 = 0;
// float no2 = 0;
// float o3 = 0;
// float humidity = 0;
// float airtemp = 0;
// char waterNode_location[10] = "Unknown";
// float tds = 0;
// float orp = 0;
// float ph = 0;
// float turbidity = 0;
// float dos = 0;
// float watertemp = 0;


// // Function Definitions
// void initWifi();
// void initEspNow();
// void initServer();
// void OnDataRecv(const uint8_t *mac, const uint8_t *incomingData, int len);  // Callback function for ESP-NOW
// void sendSensorData();                                                      // sending data to web server
// void printData();

// void setup() {
//   Serial.begin(115200);
//   initWifi();
//   initEspNow();
//   initServer();
// }

// void loop() {
//   Serial.print("WiFi Channel: ");
//   Serial.println(WiFi.channel());
//   Serial.print("IP Address: ");
//   Serial.println(WiFi.localIP());
//   sendSensorData();
//   ws.cleanupClients();
//   delay(2000);  // send data to server every second
// }

// void OnDataRecv(const uint8_t *mac, const uint8_t *incomingData, int len) {
//   memcpy(&myData, incomingData, sizeof(myData));
//   separateData();
//   printData();
// }

// void separateData() {

//   if (!myData.node_identifier) {  // 0 = air
//     strcpy(airNode_location, myData.location);
//     airtemp = myData.temperature;
//     humidity = myData.humi_turb;
//     co = myData.co_ph;
//     o3 = myData.o3_tds;
//     no2 = myData.no2_orp;
//     voc = myData.voc_dos;
//     pm25 = myData.pm25_xx;
//   }
//   if (myData.node_identifier) {  // 1 = water
//     strcpy(waterNode_location, myData.location);
//     watertemp = myData.temperature;
//     turbidity = myData.humi_turb;
//     ph = myData.co_ph;
//     tds = myData.o3_tds;
//     orp = myData.no2_orp;
//     dos = myData.voc_dos;
//   }
// }

// void sendSensorData() {
//   String jsonData = "{\"co\": " + String(co) + ", \"turbidity\": " + String(turbidity) + ", \"ph\": " + String(ph) + ", \"orp\": " + String(orp) + ", \"tds\": " + String(tds) + ", \"dos\": " + String(dos) + ", \"voc\": " + String(voc) + ", \"pm25\": " + String(pm25) + ", \"no2\": " + String(no2) + ", \"o3\": " + String(o3) + ", \"humidity\": " + String(humidity) + ", \"watertemp\": " + String(watertemp) + ", \"airtemp\": " + String(airtemp) + "}";
//   ws.textAll(jsonData);  // Send JSON to all WebSocket clients
// }

// void initWifi() {

//   WiFi.mode(WIFI_AP_STA);  // set wifi mode as access point and station
//   esp_wifi_set_channel(11, WIFI_SECOND_CHAN_NONE);
//   esp_err_t err = esp_wifi_set_mac(WIFI_IF_STA, &newMACAddress[0]);  // set custom mac address
//   if (err == ESP_OK) {
//     Serial.println("Success changing Mac Address");
//   }

//   // Set Static IP address
//   IPAddress local_IP(192, 168, 2, 75);
//   IPAddress gateway(192, 168, 1, 1);
//   IPAddress subnet(255, 255, 0, 0);

//   // Configures static IP address***********may cause issues
//   // if (!WiFi.config(local_IP, gateway, subnet)) {
//   //   Serial.println("STA Failed to configure");
//   // }

//   Serial.print("IP Address: ");
//   Serial.println(WiFi.localIP());

//   // Connect to WiFi
//   WiFi.begin(ssid, password);
//   while (WiFi.status() != WL_CONNECTED) {
//     delay(500);
//     Serial.print(".");
//   }
// }

// void initEspNow() {
//   // Init ESP-NOW
//   if (esp_now_init() != ESP_OK) {
//     Serial.println("Error initializing ESP-NOW");
//     return;
//   }
//   Serial.println("ESP-NOW initialized!");
//   // Once ESPNow is successfully Init, we will register for recv CB to get recv packer info
//   esp_now_register_recv_cb(esp_now_recv_cb_t(OnDataRecv));
// }

// void initServer() {
//   // start file server
//   if (!LittleFS.begin(true)) {
//     Serial.println("SPIFFS initialization failed!");
//     return;
//   }

//   // Serve HTML page on accessing the IP
//   server.on("/", HTTP_GET, [](AsyncWebServerRequest *request) {
//     request->send(LittleFS, "/index.html", "text/html");
//   });
//   // Serve JS page on accessing the IP
//   server.on("/script.js", HTTP_GET, [](AsyncWebServerRequest *request) {
//     request->send(LittleFS, "/script.js", "application/javascript");
//   });
//   // Serve CSS page on accessing the IP
//   server.on("/style.css", HTTP_GET, [](AsyncWebServerRequest *request) {
//     request->send(LittleFS, "/style.css", "text/css");
//   });

//   //WebSocket event handling
//   ws.onEvent([](AsyncWebSocket *server, AsyncWebSocketClient *client,
//                 AwsEventType type, void *arg, uint8_t *data, size_t len) {
//     if (type == WS_EVT_CONNECT) {
//       Serial.println("New WebSocket Client Connected!");
//     }
//   });

//   server.addHandler(&ws);
//   server.begin();
//   Serial.println("Server started!");
// }

// void printData() {
//   Serial.print("ID, ");
//   Serial.print(myData.node_identifier);
//   Serial.print(", ");

//   Serial.print("Location, ");
//   Serial.print(myData.location);
//   Serial.print(", ");

//   Serial.print("O3/TDS, ");
//   Serial.print(myData.o3_tds);
//   Serial.print(", ");

//   Serial.print("NO2/ORP, ");
//   Serial.print(myData.no2_orp);
//   Serial.print(", ");

//   Serial.print("CO/pH, ");
//   Serial.print(myData.co_ph);
//   Serial.print(", ");

//   Serial.print("VOC/DO, ");
//   Serial.print(myData.voc_dos);
//   Serial.print(", ");

//   Serial.print("Temperature, ");
//   Serial.print(myData.temperature);
//   Serial.print(", ");

//   Serial.print("Humidity/Turbidity, ");
//   Serial.print(myData.humi_turb);
//   Serial.print(", ");

//   Serial.print("PM2.5, ");
//   Serial.print(myData.pm25_xx);
//   Serial.println();
// }

#include <WiFi.h>
#include <esp_wifi.h>
#include <ESPAsyncWebServer.h>
#include <esp_now.h>
#include <LittleFS.h>
 
//struct to receive messages from nodes
typedef struct struct_message {
  bool node_identifier;  // 0 = air | 1 = water
  char location[10];
  float temperature;
  float humi_turb;
  float co_ph;
  float o3_tds;
  float no2_orp;
  float voc_dos;
  float pm25_xx;
} struct_message;
struct_message myData;
 
// WiFi and server setup
// To run server: ngrok http 192.168.2.75:80 --host-header=rewrite
AsyncWebServer server(80);
AsyncWebSocket ws("/ws");  // WebSocket endpoint
// const char *ssid = "d";
// const char *password = "12345678";
// const char *ssid = "DBM 2.0";
// const char *password = "bddd1234";
// const char* ssid ="engbois";
// const char* password = "dejkwmms43#";
// const char* ssid ="DESKTOP-5P630LH 3872";
// const char* password = "12345678";
// const char* ssid = "wirelessguy";
// const char* password = "coolguybuddyguy";
const char* ssid = "bingbong";
const char* password = "bongbing123";
 
//Custom MAC address
uint8_t newMACAddress[] = { 0x32, 0xAE, 0xA4, 0x07, 0x0D, 0x66 };
 
// Global Variables to store received data
char airNode_location[10] = "Unknown";
float co = 0;
float voc = 0;
float pm25 = 0;
float no2 = 0;
float o3 = 0;
float humidity = 0;
float airtemp = 0;
char waterNode_location[10] = "Unknown";
float tds = 0;
float orp = 0;
float ph = 0;
float turbidity = 0;
float dos = 0;
float watertemp = 0;
 
 
// Function Definitions
void initWifi();
void initEspNow();
void initServer();
void OnDataRecv(const uint8_t *mac, const uint8_t *incomingData, int len);  // Callback function for ESP-NOW
void sendSensorData();                                                      // sending data to web server
void printData();
 
void setup() {
  Serial.begin(115200);
  initWifi();
  initEspNow();
  initServer();
}
 
void loop() {
  Serial.print("WiFi Channel: ");
  Serial.println(WiFi.channel());
  Serial.print("IP Address: ");
  Serial.println(WiFi.localIP());
  sendSensorData();
  ws.cleanupClients();
  delay(2000);  // send data to server every second
}
 
void OnDataRecv(const uint8_t *mac, const uint8_t *incomingData, int len) {
  memcpy(&myData, incomingData, sizeof(myData));
  separateData();
  //printData();
}
 
void separateData() {
 
  if (!myData.node_identifier) {  // 0 = air
    strcpy(airNode_location, myData.location);
    airtemp = myData.temperature;
    humidity = myData.humi_turb;
    co = myData.co_ph;
    o3 = myData.o3_tds;
    no2 = myData.no2_orp;
    voc = myData.voc_dos;
    pm25 = myData.pm25_xx;
  }
  if (myData.node_identifier) { // 1 = water
    strcpy(waterNode_location, myData.location);
    watertemp = myData.temperature;
    turbidity = myData.humi_turb;
    ph = myData.co_ph;
    tds = myData.o3_tds;
    orp = myData.no2_orp;
    dos = myData.voc_dos;
  }
}
 
void sendSensorData() {
    String jsonData = "{\"co\": " + String(co) +
                      ", \"turbidity\": " + String(turbidity) +
                      ", \"ph\": " + String(ph) +
                      ", \"orp\": " + String(orp) +
                      ", \"tds\": " + String(tds) +
                      ", \"dos\": " + String(dos) +
                      ", \"voc\": " + String(voc) +
                      ", \"pm25\": " + String(pm25) +
                      ", \"no2\": " + String(no2) +
                      ", \"o3\": " + String(o3) +
                      ", \"humidity\": " + String(humidity) +
                      ", \"watertemp\": " + String(watertemp) +
                      ", \"airtemp\": " + String(airtemp) +
                      "}";
    ws.textAll(jsonData);  // Send JSON to all WebSocket clients
}
 
void initWifi() {
 
  WiFi.mode(WIFI_AP_STA);  // set wifi mode as access point and station
 
  esp_err_t err = esp_wifi_set_mac(WIFI_IF_STA, &newMACAddress[0]);  // set custom mac address
  if (err == ESP_OK) {
    Serial.println("Success changing Mac Address");
  }
 
  // Set Static IP address
  IPAddress local_IP(192, 168, 2, 75);
  IPAddress gateway(192, 168, 1, 1);
  IPAddress subnet(255, 255, 0, 0);
 
  // Configures static IP address***********may cause issues
  // if (!WiFi.config(local_IP, gateway, subnet)) {
  //   Serial.println("STA Failed to configure");
  // }
 
  Serial.print("IP Address: ");
  Serial.println(WiFi.localIP());
 
  // Connect to WiFi
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1500);
    Serial.print(".");
  }
}
 
void initEspNow() {
  // Init ESP-NOW
  if (esp_now_init() != ESP_OK) {
    Serial.println("Error initializing ESP-NOW");
    return;
  }
  Serial.println("ESP-NOW initialized!");
  // Once ESPNow is successfully Init, we will register for recv CB to get recv packer info
  esp_now_register_recv_cb(esp_now_recv_cb_t(OnDataRecv));
}
 
void initServer() {
  // start file server
  if (!LittleFS.begin(true)) {
    Serial.println("SPIFFS initialization failed!");
    return;
  }
 
  // Serve HTML page on accessing the IP
  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request) {
    request->send(LittleFS, "/index.html", "text/html");
  });
  // Serve JS page on accessing the IP
  server.on("/script.js", HTTP_GET, [](AsyncWebServerRequest *request) {
    request->send(LittleFS, "/script.js", "application/javascript");
  });
  // Serve CSS page on accessing the IP
  server.on("/style.css", HTTP_GET, [](AsyncWebServerRequest *request) {
    request->send(LittleFS, "/style.css", "text/css");
  });
 
  //WebSocket event handling
  ws.onEvent([](AsyncWebSocket *server, AsyncWebSocketClient *client,
                AwsEventType type, void *arg, uint8_t *data, size_t len) {
    if (type == WS_EVT_CONNECT) {
      Serial.println("New WebSocket Client Connected!");
    }
  });
 
  server.addHandler(&ws);
  server.begin();
  Serial.println("Server started!");
}
 
void printData(){
  Serial.print("ID, ");
  Serial.print(myData.node_identifier);
  Serial.print(", ");
 
  Serial.print("Location, ");
  Serial.print(myData.location);
  Serial.print(", ");
 
  Serial.print("TDS, ");
  Serial.print(myData.o3_tds);
  Serial.print(", ");
 
  Serial.print("ORP, ");
  Serial.print(myData.no2_orp);
  Serial.print(", ");
 
  Serial.print("pH, ");
  Serial.print(myData.co_ph);
  Serial.print(", ");
 
  Serial.print("DO, ");
  Serial.print(myData.voc_dos);
  Serial.print(", ");
 
  Serial.print("Water Temperature, ");
  Serial.print(myData.temperature);
  Serial.print(", ");
 
  Serial.print("Turbidity, ");
  Serial.print(myData.humi_turb);
  Serial.println();
}