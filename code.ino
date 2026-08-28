#include <Arduino.h>
#include <LiquidCrystal.h>

LiquidCrystal lcd(21, 22, 13, 23, 18, 19);
// ================= GPIO =================
const int SENSOR1 = 27;
const int SENSOR2 = 26;
const int LED_PIN = 25;
const int BUTTON_PIN = 33;
// ================= SETTINGS =================
const float DISTANCE = 0.20;       
const float SPEED_LIMIT = 1.00;    
const unsigned long TIMEOUT_US = 5000000UL; 
// ================= VARIABLES =================
unsigned long startTime = 0;
unsigned long stopTime = 0;
bool s1_triggered = false; 
bool s2_triggered = false; 
bool measuring = false;
bool measured = false;
bool isOverSpeed = false;  
bool ledState = false;
unsigned long lastBlink = 0;
// ==================================================
void resetSystem() {
  measuring = false;
  measured = false;
  isOverSpeed = false;
  ledState = false;
  
  s1_triggered = false;
  s2_triggered = false;
  
  digitalWrite(LED_PIN, LOW);
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("ESP32 SPEED");
  lcd.setCursor(0, 1);
  lcd.print("READY");
}

void showWaitingScreen() {
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("MEASURING");
  lcd.setCursor(0, 1);
  lcd.print("WAIT SENSOR2");
}

void showTimeoutScreen() {
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("TIMEOUT");
  lcd.setCursor(0, 1);
  lcd.print("RESET SYSTEM");
}

void blinkLed() {
  if (millis() - lastBlink >= 250) {
    lastBlink = millis();
    ledState = !ledState;
    digitalWrite(LED_PIN, ledState);
    
    lcd.setCursor(15, 1);
    if (ledState) {
      lcd.print("*");
    } else {
      lcd.print(" ");
    }
  }
}

// ==================================================
void setup() {
  pinMode(SENSOR1, INPUT_PULLUP);
  pinMode(SENSOR2, INPUT_PULLUP);
  pinMode(BUTTON_PIN, INPUT_PULLUP);
  pinMode(LED_PIN, OUTPUT);
  
  digitalWrite(LED_PIN, LOW);
  lcd.begin(16, 2);
  
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("SYSTEM START");
  lcd.setCursor(0, 1);
  lcd.print("PLEASE WAIT");
  delay(1500);
  
  resetSystem();
}
// ==================================================
void loop() {
  // ================= RESET BUTTON =================
  if (digitalRead(BUTTON_PIN) == LOW) {
    delay(20); 
    if (digitalRead(BUTTON_PIN) == LOW) {
      resetSystem();
      while (digitalRead(BUTTON_PIN) == LOW); 
    }
  }

  // ================= SENSOR 1 =================
  if (!measuring && !measured && !s1_triggered && digitalRead(SENSOR1) == LOW) {
    startTime = micros();
    s1_triggered = true;// Khóa cảm biến 1 lại, không cho đè dữ liệu
    measuring = true;
    showWaitingScreen();
  }
  // ================= TIMEOUT =================
  if (measuring && !measured && (micros() - startTime > TIMEOUT_US)) {
  measuring = false;
  measured = false;

  s1_triggered = false;
  s2_triggered = false;

  digitalWrite(LED_PIN, LOW);

  showTimeoutScreen();
}
  // ================= SENSOR 2 =================
  if (measuring && !measured && !s2_triggered && digitalRead(SENSOR2) == LOW) {
    stopTime = micros();
    s2_triggered = true; 
    unsigned long elapsed = stopTime - startTime;
    float timeSec = elapsed / 1000000.0;
    float speed = DISTANCE / timeSec;

    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("V=");
    lcd.print(speed, 2);
    lcd.print(" m/s");

    lcd.setCursor(0, 1);
    if (speed > SPEED_LIMIT) {
      lcd.print("OVER SPEED");
      isOverSpeed = true; 
    } else {
      lcd.print("NORMAL");
      isOverSpeed = false;
      digitalWrite(LED_PIN, LOW);
    }

    measuring = false;
    measured = true;
  }
  // ================= BLINK LED PROCESS =================
  if (measured && isOverSpeed) {
    blinkLed();
  }

  delay(1); 
}
