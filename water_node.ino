#include <WiFi.h>
#include <esp_now.h>
#include <esp_wifi.h>
#include <DallasTemperature.h>
#include <OneWire.h>

#define ESP_NOW_CHANNEL 10      // may need to change
#define V_LEVELS 4096           // voltage levels or adc resolution
#define V_REF 3.3               // ESP32C6 reference voltage
#define SCOUNT 10               // sample count for filtering
#define NODEID 1                // water node
#define NODELOCATION "Toronto"  // max character = 10

#define ORP_PIN 1
#define TEMP_PIN 2
#define TURB_PIN 3
#define DOS_PIN 4
#define PH_PIN 5
#define TDS_PIN 6

typedef struct struct_message {  //struct to pass to home node
  bool node_identifier;
  char node_location[10];
  float watertemp;
  float turbidity;
  float ph;
  float tds;
  float orp;
  float dos;
} struct_message;
struct_message myData;

OneWire oneWire(TEMP_PIN);        // reading data from temp probe
DallasTemperature sensors(&oneWire);  // interpreting temp data

esp_now_peer_info_t peerInfo;                                     // variable to store channel info
uint8_t homenodeMac[6] = { 0x32, 0xAE, 0xA4, 0x07, 0x0D, 0x66 };  // Store home node static MAC address

//Function Definitions
void OnDataSent(const uint8_t *mac_addr, esp_now_send_status_t status);  //call back function for ESP-NOW
void update_do();
void update_ph();
void update_orp();
void update_tds();
void update_turbidity();
void update_watertemp();

void setup() {

  Serial.begin(115200);
  myData.node_identifier = NODEID;             // set as water node
  strcpy(myData.node_location, NODELOCATION);  // set location to send

  WiFi.mode(WIFI_STA);  // set device as wifi station
  WiFi.disconnect();    // ensure clean start

  if (esp_now_init() != ESP_OK) {  // Initialize ESP-NOW
    Serial.println("Error initializing ESP-NOW");
    return;
  }
  esp_wifi_set_channel(ESP_NOW_CHANNEL, WIFI_SECOND_CHAN_NONE);

  esp_now_register_send_cb(OnDataSent);  // Register callback for send status

  // Automatically set the receiver's MAC to this home node's MAC
  memcpy(peerInfo.peer_addr, homenodeMac, 6);
  peerInfo.channel = 0;
  peerInfo.encrypt = false;

  // Add peer
  if (esp_now_add_peer(&peerInfo) != ESP_OK) {
    Serial.println("Failed to add peer");
    return;
  }

  pinMode(TURB_PIN, INPUT);
  pinMode(PH_PIN, INPUT);
  pinMode(ORP_PIN, INPUT);
  pinMode(TDS_PIN, INPUT);
  pinMode(DOS_PIN, INPUT);
  sensors.begin();  //water temperature
}

void loop() {

  update_do();
  update_orp();
  update_ph();
  update_tds();
  update_turbidity();
  update_watertemp();

  // Send message via ESP-NOW
  esp_err_t result = esp_now_send(peerInfo.peer_addr, (uint8_t *)&myData, sizeof(myData));

  if (result == ESP_OK) {
    //Serial.println("Sent with success");
  } else {
    Serial.println("Error sending the data");
  }
  delay(1000);  // send data every second
}

void update_do(){
  float raw = analogRead(DOS_PIN);
  float adc = (raw/V_LEVELS) * V_REF;
  myData.dos = (adc - 0.41) * 15.7; // (mV - 0% DO voltage) * slope
}

void update_turbidity(){
  float raw = analogRead(TURB_PIN);
  float adc = (raw/V_LEVELS) * V_REF;
  myData.turbidity = adc; // (mV - 0% DO voltage) * slope
}

void update_ph(){
  float samples[SCOUNT];
  for (int i = 0; i < SCOUNT; i++){
    float raw = analogRead(PH_PIN);
    samples[i] = (raw/(float)V_LEVELS) * V_REF;
    delay(10);
  }
  for (int i = 0; i < 10 - 1; i++) {
    for (int j = i + 1; j < 10; j++) {
      if (samples[i] > samples[j]) {
        float temp = samples[i];
        samples[i] = samples[j];
        samples[j] = temp;
      }
    }
  }
  float adcMedian = samples[SCOUNT / 2];  // Get median value
  myData.ph = 9.23 * adcMedian - 6.40; //slope = 9.23 || calibration value = -6.48
}

void update_orp(){
  float raw = analogRead(ORP_PIN);
  float adc = (raw / V_LEVELS) * V_REF;
  myData.orp = (adc - 1) * 1000;
}

void update_tds(){
    float raw = analogRead(TDS_PIN);
    float adc = raw * V_REF / V_LEVELS;

    // Temperature Compensation
    update_watertemp();
    float compensationCoefficient = 1.0 + 0.02 * (myData.watertemp - 25.0);
    float compensationVoltage = adc / compensationCoefficient;

    // Convert voltage to TDS (Based on calibration curve)
    myData.tds = (133.42 * compensationVoltage * compensationVoltage * compensationVoltage
               - 255.86 * compensationVoltage * compensationVoltage
               + 857.39 * compensationVoltage) * 0.5;
}

void update_watertemp(){
  sensors.requestTemperatures();
  myData.watertemp = sensors.getTempCByIndex(0);  // Get temperature in Celsius  
}

void OnDataSent(const uint8_t *mac_addr, esp_now_send_status_t status) {
  //Serial.print("\r\nLast Packet Send Status:\t");
  Serial.println(status == ESP_NOW_SEND_SUCCESS ? "Delivery Success" : "Delivery Fail");
}