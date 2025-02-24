#include <Arduino.h>


#include <Wire.h>
#include <LiquidCrystal_I2C.h>

// Khai báo địa chỉ LCD (0x27 hoặc 0x3F, tùy module của bạn)
LiquidCrystal_I2C lcd(0x27, 16, 2);

//****************************************************RETURN MQ2 VALUE
const int sensorPin1 = 25;
const int sensorPin2 = 26;
const int sensorPin3 = 27;
uint8_t MQ21_getValue() {
  uint8_t value1 = analogRead(sensorPin1);
  // Serial.print("Analog 25: ");
  // Serial.print(value1);
  return value1;
}
uint8_t MQ22_getValue() {
  uint8_t value2 = analogRead(sensorPin2);
  // Serial.print(" | Analog 26: ");
  // Serial.print(value2);
  return value2;
}
uint8_t MQ23_getValue() {
  uint8_t value3 = analogRead(sensorPin3);
  // Serial.print(" | Analog 27: ");
  // Serial.println(value3);
  return value3;
}
//****************************************************RETURN DHT VALUE
#include "DHT.h"

#define DHTTYPE DHT11  // there are multiple kinds of DHT sensors

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
uint8_t hum1_getValue() {
  return dht1.readHumidity();
}
uint8_t hum2_getValue() {
  return dht2.readHumidity();
}
uint8_t hum3_getValue() {
  return dht3.readHumidity();
}

// Hàm đọc nhiệt độ từ từng cảm biến
uint8_t temp1_getValue() {
  return dht1.readTemperature();
}
uint8_t temp2_getValue() {
  return dht2.readTemperature();
}
uint8_t temp3_getValue() {
  return dht3.readTemperature();
}

//***************************************************************FLAME SENSOR
#define Flame1_PIN 5  // ESP32's pin GPIO13 connected to DO pin of the flame sensor
#define Flame2_PIN 18
#define Flame3_PIN 19

uint8_t flame1_getValue() {
  int flame_state = digitalRead(Flame1_PIN);

  if (flame_state == HIGH) {
    return 0;
    //Serial.println("No flame1 dected => The fire1 is NOT detected");
  } else {
    return 1;
    //Serial.println("Flame1 dected => The fire1 is detected");
  }
}

uint8_t flame2_getValue() {
  int flame_state = digitalRead(Flame2_PIN);

  if (flame_state == HIGH) {
    return 0;
    //Serial.println("No flame1 dected => The fire1 is NOT detected");
  } else {
    return 1;
    //Serial.println("Flame1 dected => The fire1 is detected");
  }
}

uint8_t flame3_getValue() {
  int flame_state = digitalRead(Flame3_PIN);

  if (flame_state == HIGH) {
    return 0;
    //Serial.println("No flame1 dected => The fire1 is NOT detected");
  } else {
    return 1;
    //Serial.println("Flame1 dected => The fire1 is detected");
  }
}

//******************************************************************************
void setup() {
  Serial.begin(115200);
  //*********************************Khoi tao LCD
  lcd.init();        // Khởi tạo LCD
  lcd.backlight();   // Bật đèn nền

  // Hiển thị nội dung
  lcd.setCursor(0, 0);  // Di chuyển con trỏ về đầu dòng 1
  lcd.print("Smart Home......"); 

  lcd.setCursor(0, 1);  // Di chuyển con trỏ về đầu dòng 2
  lcd.print("Initializing");
  lcd.clear();
  Serial.println("DHTxx test!");
  dht1.begin();
  dht2.begin();
  dht3.begin();

  //Khoi tao cac chan cam bien nhan biet ngon lua
  pinMode(Flame1_PIN, INPUT);
  pinMode(Flame2_PIN, INPUT);
  pinMode(Flame3_PIN, INPUT);


  //Khoi tao cac chan BUZZ (2), Light(17)
  pinMode(2, OUTPUT);
  pinMode(17, OUTPUT);
  // Cấu hình chân là INPUT
  pinMode(33, INPUT); 



  //Khai bao cac Tasks
   // Tạo Task1, chạy trên core 0
    xTaskCreatePinnedToCore(Task1, "Task 1", 8192, NULL, 2, NULL, 0);
    xTaskCreatePinnedToCore(Task2, "Task 2", 2048, NULL, 1, NULL, 1);
    xTaskCreatePinnedToCore(Light_Control, "Light_Control", 2048, NULL, 1, NULL, 1);
    xTaskCreatePinnedToCore(Display, "Display", 2048, NULL, 1, NULL, 1);

}
// Declare global global variable
uint8_t Flame1_Value = 0;
uint8_t Flame2_Value = 0;
uint8_t Flame3_Value = 0;

uint8_t Temp1_Value = 0;
uint8_t Temp2_Value = 0;
uint8_t Temp3_Value = 0;

uint8_t Hum1_Value = 0;
uint8_t Hum2_Value = 0;
uint8_t Hum3_Value = 0;

uint8_t CO1_Value = 0;
uint8_t CO2_Value = 0;
uint8_t CO3_Value = 0;
// Output processing variable
// Task 1: Collect Data_______________________________________________
void Task1(void *pvParameters) {
    while (1) {
        Flame1_Value = flame1_getValue();
        Flame2_Value = flame2_getValue();
        Flame3_Value = flame3_getValue();

        Temp1_Value = temp1_getValue();
        Temp2_Value = temp2_getValue();
        Temp3_Value = temp3_getValue();

        Hum1_Value = hum1_getValue();
        Hum2_Value = hum2_getValue();
        Hum3_Value = hum3_getValue();

        CO1_Value = MQ21_getValue();
        CO2_Value = MQ22_getValue();
        CO3_Value = MQ23_getValue();

        vTaskDelay(100 / portTICK_PERIOD_MS); // Delay 1 giây
    }
} 
// Task 2: Processing Data
void Task2(void *pvParameters) {
    while (1) {
      // Serial.print(Flame1_Value);
      // Serial.print(Flame2_Value);
      // Serial.println(Flame3_Value);
        // Fire Detect
        if(Flame1_Value || Flame2_Value || Flame3_Value){
          digitalWrite(2, HIGH);
          Serial.print("RING");
        }
        vTaskDelay(100 / portTICK_PERIOD_MS);
    }
}
// Task 3: On/Off light handle
void Light_Control(void *pvParameters) {
    while (1) {
        uint8_t x = analogRead(33);
        // Serial.println(x);
        if(x > 3000){
          digitalWrite(17, HIGH);
        }
        vTaskDelay(10 / portTICK_PERIOD_MS);
    }
}
void Display(void *pxParameters) {
  while(1){
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("Tem(*C):");
    lcd.print(Temp1_Value);
    lcd.print("_");
    lcd.print(Temp2_Value);
    lcd.print("_");
    lcd.print(Temp3_Value);
    lcd.setCursor(0,1);
    lcd.print("Hum(%) :");
    lcd.print(Hum1_Value);
    lcd.print("_");
    lcd.print(Hum2_Value);
    lcd.print("_");
    lcd.print(Hum3_Value);
    vTaskDelay(1000 / portTICK_PERIOD_MS);
  }
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
}
