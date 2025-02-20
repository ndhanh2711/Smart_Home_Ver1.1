#include <Arduino.h>

#include "DHT.h"
#define DHTPIN 13     // what digital pin the DHT sensor is connected to
#define DHTTYPE DHT11   // there are multiple kinds of DHT sensors
DHT dht(DHTPIN, DHTTYPE);




//****************************************************RETURN MQ2 VALUE
const int sensorPin1 = 25;
const int sensorPin2 = 26;
const int sensorPin3 = 27;
uint8_t MQ21_getValue(){
  uint8_t value1 = analogRead(sensorPin1);
  Serial.print("Analog 25: ");
  Serial.print(value1);
  return value1;
}
uint8_t MQ22_getValue(){
  uint8_t value2 = analogRead(sensorPin2);
  Serial.print(" | Analog 26: ");
  Serial.print(value2);
  return value2;
}
uint8_t MQ23_getValue(){
  uint8_t value3 = analogRead(sensorPin3);
  Serial.print(" | Analog 27: ");
  Serial.println(value3);
  return value3;
}
//****************************************************RETURN DHT VALUE
// Khai báo chân kết nối với cảm biến
#define DHTPIN1 13
#define DHTPIN2 12
#define DHTPIN3 14

// Loại cảm biến (DHT11)
#define DHTTYPE DHT11

// Khởi tạo ba đối tượng DHT
DHT dht1(DHTPIN1, DHTTYPE);
DHT dht2(DHTPIN2, DHTTYPE);
DHT dht3(DHTPIN3, DHTTYPE);

// Hàm đọc độ ẩm từ từng cảm biến
float hum1_getValue() { return dht1.readHumidity(); }
float hum2_getValue() { return dht2.readHumidity(); }
float hum3_getValue() { return dht3.readHumidity(); }

// Hàm đọc nhiệt độ từ từng cảm biến
float temp1_getValue() { return dht1.readTemperature(); }
float temp2_getValue() { return dht2.readTemperature(); }
float temp3_getValue() { return dht3.readTemperature(); }

//***************************************************************FLAME SENSOR
#define Flame1_PIN 5  // ESP32's pin GPIO13 connected to DO pin of the flame sensor
#define Flame2_PIN 18
#define Flame3_PIN 19

uint8_t flame1_getValue(){
  int flame_state = digitalRead(Flame1_PIN);

  if (flame_state == HIGH){
    return 0;
    Serial.println("No flame1 dected => The fire1 is NOT detected");
  }
  else{
    return 1;
    Serial.println("Flame1 dected => The fire1 is detected");
  }
}

uint8_t flame2_getValue(){
  int flame_state = digitalRead(Flame2_PIN);

  if (flame_state == HIGH){
    return 0;
    Serial.println("No flame1 dected => The fire1 is NOT detected");
  }
  else{
    return 1;
    Serial.println("Flame1 dected => The fire1 is detected");
  }
}

uint8_t flame3_getValue(){
  int flame_state = digitalRead(Flame3_PIN);

  if (flame_state == HIGH){
    return 0;
    Serial.println("No flame1 dected => The fire1 is NOT detected");
  }
  else{
    return 1;
    Serial.println("Flame1 dected => The fire1 is detected");
  }
}

//******************************************************************************
void setup() {
  Serial.begin(9600);
  Serial.println("DHTxx test!");
  dht1.begin();
  dht2.begin();
  dht3.begin();

  pinMode(Flame1_PIN, INPUT);
  pinMode(Flame2_PIN, INPUT);
  pinMode(Flame3_PIN, INPUT);


}


void loop() {

  // float h1 = hum1_getValue();
  // float h2 = hum2_getValue();
  // float h3 = hum3_getValue();
  
  // float t1 = temp1_getValue();
  // float t2 = temp2_getValue();
  // float t3 = temp3_getValue();

  // // Kiểm tra xem có lỗi đọc dữ liệu không
  // if (isnan(h1) || isnan(t1) || isnan(h2) || isnan(t2) || isnan(h3) || isnan(t3)) {
  //   Serial.println("Failed to read from one or more DHT sensors!");
  //   return;
  // }

  // // Hiển thị dữ liệu lên Serial Monitor
  // Serial.print("DHT1 - Humidity: "); Serial.print(h1); Serial.print(" %\t Temperature: "); Serial.print(t1); Serial.println(" *C");
  // Serial.print("DHT2 - Humidity: "); Serial.print(h2); Serial.print(" %\t Temperature: "); Serial.print(t2); Serial.println(" *C");
  // Serial.print("DHT3 - Humidity: "); Serial.print(h3); Serial.print(" %\t Temperature: "); Serial.print(t3); Serial.println(" *C");

  // Serial.println("-------------------------------");




  int check = flame1_getValue();
  if(check){
    Serial.println("Chay");
  }
  else{
    Serial.println("Khong chay");
  }
}
