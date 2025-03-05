//*************************************************************************Khai báo các thư viện ngoại vi
#include <Arduino.h>
#include <Wire.h>                    // Giao tiếp 1-Wire cho cảm biến DHT
#include <LiquidCrystal_I2C.h>       // Thư viện cho I2C
#include <ESP32Servo.h>              // Thư viện cho động cơ servo SG90
#include "DHT.h"                     // Thư viện cho cảm biến DHT
LiquidCrystal_I2C lcd(0x27, 16, 2);  // Địa chỉ I2C cho LCD


static const int servoPin = 15;      // Chân kết nối với động cơ servo
Servo servo1;
// Khai báo các chân kết nối với cảm biên khí gas
const int sensorPin1 = 25;
const int sensorPin2 = 26;
const int sensorPin3 = 27;
// Khai báo chân kết nối với cảm biến DHT11
#define DHTPIN1 13
#define DHTPIN2 12
#define DHTPIN3 14
// Loại cảm biến (DHT11)
#define DHTTYPE DHT11
// Khởi tạo ba đối tượng DHT
DHT dht1(DHTPIN1, DHTTYPE);
DHT dht2(DHTPIN2, DHTTYPE);
DHT dht3(DHTPIN3, DHTTYPE);
// Khai báo, gán chân gpio cho cảm biến phát hiện ngọn lửa
#define Flame1_PIN 5  
#define Flame2_PIN 18
#define Flame3_PIN 19
// Khởi tạo các giá trị toàn cục cho từng loại thông số
uint8_t Flame1_Value = 0;
uint8_t Flame2_Value = 0;
uint8_t Flame3_Value = 0;

uint8_t Temp1_Value = 0;
uint8_t Temp2_Value = 0;
uint8_t Temp3_Value = 0;

uint8_t Hum1_Value = 0;
uint8_t Hum2_Value = 0;
uint8_t Hum3_Value = 0;

int CO1_Value = 0;
int CO2_Value = 0;
int CO3_Value = 0;
//*********************************************************************************RETURN MQ2 VALUE
int MQ21_getValue() {
  int value1 = analogRead(sensorPin1);
  // Serial.print("Analog 25: ");
  // Serial.print(value1);
  return value1;
}
int MQ22_getValue() {
  int value2 = analogRead(sensorPin2);
  // Serial.print(" | Analog 26: ");
  // Serial.print(value2);
  return value2;
}
int MQ23_getValue() {
  int value3 = analogRead(sensorPin3);
  // Serial.print(" | Analog 27: ");
  // Serial.println(value3);
  return value3;
}
//*********************************************************************************RETURN DHT VALUE
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

//***********************************************************************************FLAME SENSOR
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

//****************************************************************************************Hàm SET_UP
void setup() {
  Serial.begin(115200);
  servo1.attach(servoPin);
  pinMode(23, INPUT);
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
    xTaskCreatePinnedToCore(Task1, "Task 1",                   8192, NULL, 2, NULL, 0);
    xTaskCreatePinnedToCore(Flame_Ring, "Flame_Ring",          4096, NULL, 1, NULL, 0);
    xTaskCreatePinnedToCore(Light_Control, "Light_Control",    4096, NULL, 1, NULL, 0);
    xTaskCreatePinnedToCore(Display, "Display",                4096, NULL, 1, NULL, 1);
    xTaskCreatePinnedToCore(Smoke_Ring, "Smoke_Ring",          4096, NULL, 1, NULL, 0);
    xTaskCreatePinnedToCore(Door_Automation, "Door_Automation",4096, NULL, 1, NULL, 1);
}
// Output processing variable
// Task 1: Collect Data_______________________________________________
void Task1(void *pvParameters) {
    while (1) {
        Temp1_Value = temp1_getValue();
        Temp2_Value = temp2_getValue();
        Temp3_Value = temp3_getValue();

        Hum1_Value = hum1_getValue();
        Hum2_Value = hum2_getValue();
        Hum3_Value = hum3_getValue();

        vTaskDelay(100 / portTICK_PERIOD_MS); // Delay 1 giây
    }
}
// Task 2: Processing Data
void Flame_Ring(void *pvParameters) {
    while (1) {
        // Đọc giá trị từ cảm biến
        Flame1_Value = flame1_getValue();
        Flame2_Value = flame2_getValue();
        Flame3_Value = flame3_getValue();

        // Nếu phát hiện lửa từ bất kỳ cảm biến nào
        if (Flame1_Value || Flame2_Value || Flame3_Value) {
            for (int i = 0; i < 10; i++) { // Kêu 6 lần trong 3 giây
                digitalWrite(2, HIGH); // Bật còi
                vTaskDelay(300 / portTICK_PERIOD_MS); // Kêu 250ms
                digitalWrite(2, LOW); // Tắt còi
                vTaskDelay(200 / portTICK_PERIOD_MS); // Ngắt quãng 250ms
            }
        }
        // else{
        //   digitalWrite(2, LOW);
        // }
        vTaskDelay(100 / portTICK_PERIOD_MS); // Chờ 100ms trước khi kiểm tra lại
    }
}

void Smoke_Ring(void *pvParameters){
  while(1){
        CO1_Value = MQ21_getValue();
        CO2_Value = MQ22_getValue();
        CO3_Value = MQ23_getValue();
        Serial.print(CO1_Value);
        Serial.print(" ");
        Serial.print(CO2_Value);
        Serial.print(" ");
        Serial.println(CO3_Value);
        if(CO1_Value >= 2000 || CO2_Value >= 2000 || CO3_Value >= 2000){
          digitalWrite(2, HIGH); // Bật còi
          vTaskDelay(3000 / portTICK_PERIOD_MS); // Kêu 250ms
          digitalWrite(2, LOW); // Tắt còi
        }
        // else{
        //   digitalWrite(2, LOW);
        // }
        vTaskDelay( 100 / portTICK_PERIOD_MS);
  }
}

// Task 3: On/Off light handle
void Light_Control(void *pvParameters) {
    while (1) {
        int x = analogRead(33);
        //Serial.println(x);
        if(x > 4000){
          digitalWrite(17, HIGH);
        }
        else if(x < 3000)
        {
          digitalWrite(17, LOW);
        }
        vTaskDelay(10 / portTICK_PERIOD_MS);
    }
}

void Door_Automation(void *pvParameters){
  while(1){
     int state = digitalRead(23);

  if (state == LOW)
  {
    for(int posDegrees = 0; posDegrees <= 90; posDegrees++) {
    servo1.write(posDegrees);
    delay(20);
  }
  delay(2000);
  for(int posDegrees = 90; posDegrees >= 0; posDegrees--) {
    servo1.write(posDegrees);
    delay(20);
  }
  }
  else
  {
    servo1.write(0);
  }
    vTaskDelay(100 / portTICK_PERIOD_MS);
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
 //Don't use
}
