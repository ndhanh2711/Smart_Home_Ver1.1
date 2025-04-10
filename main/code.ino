//*************************************************************************Khai báo các thư viện ngoại vi
#include <Arduino.h>
#include <Wire.h>                    // Giao tiếp 1-Wire cho cảm biến DHT
#include <LiquidCrystal_I2C.h>       // Thư viện cho I2C
#include <ESP32Servo.h>              // Thư viện cho động cơ servo SG90
#include "DHT.h"                     // Thư viện cho cảm biến DHT
LiquidCrystal_I2C lcd(0x27, 16, 2);  // Địa chỉ I2C cho LCD
#include "freertos/task.h"

static const int servoPin = 15;      // Chân kết nối với động cơ servo
Servo servo1;
// Khai báo các chân kết nối với cảm biên khí gas
#define sensorPin1  25
#define sensorPin2  26
#define sensorPin3  35
// Khai báo chân kết nối với cảm biến DHT11
#define DHTPIN1 13
#define DHTPIN2 27
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

uint8_t CO1_Value = 0;
uint8_t CO2_Value = 0;
uint8_t CO3_Value = 0;

//*********************************************************************************APP
/* Select ERa host location (VN: Viet Nam, SG: Singapore) */
#define ERA_LOCATION_VN
// #define ERA_LOCATION_SG

// You should get Auth Token in the ERa App or ERa Dashboard
#define ERA_AUTH_TOKEN "ad49479e-bb2d-4a1e-9c5e-d98110b4b794"

#include <ERa.hpp>
#include <Automation/ERaSmart.hpp>
#include <Time/ERaEspTime.hpp>


const char ssid[] = "Quang Hai T3";
const char pass[] = "19741975";

ERaEspTime syncTime;
ERaSmart smart(ERa, syncTime);

/* This function is triggered whenever an SMS is sent */
ERA_WRITE_SMS() {
    ERA_LOG("ERa", "Write SMS to %s: %s", to, message);
    return true;
}

/* This function will run every time ERa is connected */
ERA_CONNECTED() {
    ERA_LOG("ERa", "ERa connected!");
}

/* This function will run every time ERa is disconnected */
ERA_DISCONNECTED() {
    ERA_LOG("ERa", "ERa disconnected!");
}

//*********************************************************************************RETURN MQ2 VALUE
uint8_t MQ21_getValue() {
  uint8_t value1 = digitalRead(sensorPin1);
  if(value1 == HIGH){
    return 0;
  }
  else{
    return 1;
  }
}
uint8_t MQ22_getValue() {
  uint8_t value2 = digitalRead(sensorPin2);
  if(value2 == HIGH){
    return 0;
  }
  else{
    return 1;
  }
}
uint8_t MQ23_getValue() {
  uint8_t value3 = digitalRead(sensorPin3);
  if(value3 == HIGH){
    return 0;
  }
  else{
    return 1;
  }
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

  pinMode(sensorPin1, INPUT);
  pinMode(sensorPin2, INPUT);
  pinMode(sensorPin3, INPUT);

  //Khoi tao cac chan BUZZ (2), Light(17)
  pinMode(2, OUTPUT);
  pinMode(17, OUTPUT);
  // Cấu hình chân là INPUT
  pinMode(33, INPUT); 

  ERa.setScanWiFi(true);
  ERa.begin(ssid, pass);
  digitalWrite(17, LOW); // Đảm bảo đèn tắt ban đầu

    // Lắng nghe sự kiện từ ERa Virtual Pin V12
  // Đăng ký callback sử dụng lambda function để chuyển đổi kiểu dữ liệu phù hợp
  //ERa.addInterval(2000L, readSensorData); // Gửi dữ liệu cảm biến mỗi 2 giây
  //Khai bao cac Tasks
   // Tạo Task1, chạy trên core 0
}
void updateSensorValues() {
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
}

// Hàm riêng cho từng virtual pin
void sendTemp1() { ERa.virtualWrite(V0, Temp1_Value); }
void sendTemp2() { ERa.virtualWrite(V1, Temp2_Value); }
void sendTemp3() { ERa.virtualWrite(V2, Temp3_Value); }

void sendHum1() { ERa.virtualWrite(V3, Hum1_Value); }
void sendHum2() { ERa.virtualWrite(V4, Hum2_Value); }
void sendHum3() { ERa.virtualWrite(V5, Hum3_Value); }

void sendFlame1() { ERa.virtualWrite(V6, Flame1_Value ? "WARNING" : "GOOD"); }
void sendFlame2() { ERa.virtualWrite(V7, Flame2_Value ? "WARNING" : "GOOD"); }
void sendFlame3() { ERa.virtualWrite(V8, Flame3_Value ? "WARNING" : "GOOD"); }

void sendCO1() { ERa.virtualWrite(V9, CO1_Value  ? "WARNING" : "GOOD"); }
void sendCO2() { ERa.virtualWrite(V10, CO2_Value  ? "WARNING" : "GOOD"); }
void sendCO3() { ERa.virtualWrite(V11, CO3_Value  ? "WARNING" : "GOOD"); }

void handleBuzzer() {
    if (Flame1_Value || Flame2_Value || Flame3_Value) {
        for (int i = 0; i < 10; i++) {
            digitalWrite(2, HIGH);
            vTaskDelay(300 / portTICK_PERIOD_MS);
            digitalWrite(2, LOW);
            vTaskDelay(200 / portTICK_PERIOD_MS);
        }
    }
    else if (CO1_Value || CO2_Value  || CO3_Value ) {
      for (int i = 0; i < 10; i++) {
        digitalWrite(2, HIGH);
        vTaskDelay(300 / portTICK_PERIOD_MS);
        digitalWrite(2, LOW);
        vTaskDelay(200 / portTICK_PERIOD_MS);
      }
    }
    Serial.print(CO1_Value);
    Serial.print(" ");
    Serial.print(CO2_Value);
    Serial.print(" ");
    Serial.println(CO3_Value);
}



int door_made = 0; // Trạng thái cửa mở/đóng theo thủ công
int manual_mode = 0; // 0: tự động, 1: thủ công

void controlServo() {
    int state = digitalRead(23); 

    // Chỉ chạy chế độ tự động khi không ở chế độ thủ công
    if (state == LOW && manual_mode == 0) {
        door_made = 0;  // Đảm bảo chế độ tự động hoạt động
        for (int posDegrees = 0; posDegrees <= 90; posDegrees++) {
            servo1.write(posDegrees);
            vTaskDelay(20 / portTICK_PERIOD_MS);
        }
        vTaskDelay(2500 / portTICK_PERIOD_MS);
        for (int posDegrees = 90; posDegrees >= 0; posDegrees--) {
            servo1.write(posDegrees);
            vTaskDelay(20 / portTICK_PERIOD_MS);
        }
    }
}

// Điều khiển bằng app (ERA_WRITE)
ERA_WRITE(V13) {
    uint8_t led1 = param.getInt();

    manual_mode = 1; // Chuyển sang chế độ thủ công khi nhận lệnh từ app

    if (led1 == 1) {
        door_made = 1;
        for (int posDegrees = 0; posDegrees <= 90; posDegrees++) {
            servo1.write(posDegrees);
            vTaskDelay(20 / portTICK_PERIOD_MS);
        }
    } 
    else {
        door_made = 0;
        for (int posDegrees = 90; posDegrees >= 0; posDegrees--) {
            servo1.write(posDegrees);
            vTaskDelay(20 / portTICK_PERIOD_MS);
        }
        manual_mode = 0; // Cho phép chế độ tự động hoạt động lại sau khi cửa đóng
    }
    ERa.virtualWrite(V13, led1);
}

void updateLCD() {
    lcd.setCursor(0, 0);
    lcd.print("Tem(*C):");
    lcd.print(Temp1_Value);
    lcd.print("_");
    lcd.print(Temp2_Value);
    lcd.print("_");
    lcd.print(Temp3_Value);

    lcd.setCursor(0, 1);
    lcd.print("Hum(%) :");
    lcd.print(Hum1_Value);
    lcd.print("_");
    lcd.print(Hum2_Value);
    lcd.print("_");
    lcd.print(Hum3_Value);
}

uint8_t led_state = 0;  // Trạng thái điều khiển LED
const int THRESHOLD = 4000;  // Ngưỡng quang trở

void controlRelay() {
    if (led_state == 0) {  // Chỉ hoạt động khi app không điều khiển đèn
        int sensorValue = analogRead(33);
        if(sensorValue > 4094){
          digitalWrite(17, HIGH);
        }
        else if(sensorValue < 3200){
          digitalWrite(17, LOW);
        }
    }
}

ERA_WRITE(V12) {
    uint8_t value = param.getInt();  // Nhận giá trị từ app

    if (value) {  
        led_state = 1;  // Chặn quang trở
        digitalWrite(17, HIGH);
    } else {  
        led_state = 0;  // Cho phép quang trở điều khiển
        digitalWrite(17, LOW);  // Tắt đèn ngay lập tức
    }

    // Cập nhật trạng thái đèn về app
    ERa.virtualWrite(V12, digitalRead(17));
}

void loop() {
    ERa.run();  // Gọi ERa để xử lý các sự kiện

    updateSensorValues();  // Cập nhật giá trị cảm biến

    // Gửi dữ liệu lên ERa qua các hàm riêng
    sendTemp1();
    sendTemp2();
    sendTemp3();

    sendHum1();
    sendHum2();
    sendHum3();

    sendFlame1();
    sendFlame2();
    sendFlame3();

    sendCO1();
    sendCO2();
    sendCO3();

    handleBuzzer();  // Điều khiển còi
    controlRelay();  // Điều khiển relay
    controlServo();  // Điều khiển servo
    updateLCD();     // Cập nhật màn hình LCD
     // Đăng ký callback một lần duy nhất
}
