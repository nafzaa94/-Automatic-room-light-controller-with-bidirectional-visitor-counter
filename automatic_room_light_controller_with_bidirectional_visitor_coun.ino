#include <SPI.h>
#include <MFRC522.h>
#include <Wire.h> 
#include <LiquidCrystal_I2C.h>

constexpr uint8_t RST_PIN = 9;     // Configurable, see typical pin layout above
constexpr uint8_t SS_PIN = 10;     // Configurable, see typical pin layout above
MFRC522 rfid(SS_PIN, RST_PIN); // Instance of the class
MFRC522::MIFARE_Key key;
String tag;

LiquidCrystal_I2C lcd(0x27, 16, 2);

int Var = 0;
int Var2 = 0;
int state = 0;
int statelcd = 0;
int statelcd2 = 0;
int statelcd3 = 0;

int counter = 0;

// ir sensor
int irsensor1 = 2;
int irsensor2 = 3;

int valueirsensor1 = 0;
int valueirsensor2 = 0;

// relay lampu soket and lock

int relaylamp = 4;
int relaysocket = 5;
int relaylock = 6;


void setup() {
  Serial.begin(9600);
  pinMode(irsensor1, INPUT);
  pinMode(irsensor2, INPUT);
  pinMode(relaylamp, OUTPUT);
  pinMode(relaysocket, OUTPUT);
  pinMode(relaylock, OUTPUT);
  SPI.begin(); // Init SPI bus
  rfid.PCD_Init(); // Init MFRC522
  lcd.begin(); // kalu error tukar begin() ke init()
  lcd.backlight();

  digitalWrite(relaylamp, HIGH);
  digitalWrite(relaysocket, HIGH);
  digitalWrite(relaylock, HIGH);

}

void loop() {
  valueirsensor1 = digitalRead(irsensor1);
  valueirsensor2 = digitalRead(valueirsensor2);

  switch (Var) {
  case 0:
    lcd.setCursor(0, 0);
    lcd.print("SCAN YOUR RFID");
    lcd.setCursor(1, 2);
    lcd.print("DOOR LOCKED");
    
    if ( ! rfid.PICC_IsNewCardPresent())
    return;
    if (rfid.PICC_ReadCardSerial()) {
      for (byte i = 0; i < 4; i++) {
        tag += rfid.uid.uidByte[i];
      }
      Serial.println(tag);
    }

    if (tag == "3512763512"){
      digitalWrite(relaylock, LOW);
      tag = "";
      lcd.setCursor(1, 0);
      lcd.print("               ");
      lcd.setCursor(1, 2);
      lcd.print("Access Success");
      delay(2000); // 2 sec
      Var = 1;
      statelcd = 1;
      statelcd3 = 0;
      }
    else {
      lcd.setCursor(1, 2);
      lcd.print("               ");
      lcd.setCursor(1, 2);
      lcd.print("Error.....");
      delay(3000);
      }
    
    break;
  case 1:
    if ( ! rfid.PICC_IsNewCardPresent())
    return;
    if (rfid.PICC_ReadCardSerial()) {
      for (byte i = 0; i < 4; i++) {
        tag += rfid.uid.uidByte[i];
      }
      Serial.println(tag);
    }

    if (tag == "3512763512"){
      digitalWrite(relaylock, HIGH);
      tag = "";
      delay(2000); // 2 sec
      }
    Var = 0;
    break;
}

if (valueirsensor1 == LOW){
   statelcd2 = 1;
      if(state == 3){
        Serial.println("out");
        state = 4;
        }
      else {
        state = 1;
        }
    }

if (valueirsensor2 == LOW){
  statelcd2 = 1;
  if (state == 1){
    Serial.println("in");
    state = 2;
    }
  else {
    state = 3;
    }
  }

if (state == 2){
  counter++;
  state = 0;
  }

if (state == 4){
  counter--;
  state = 0;

  if (counter < 0){
    counter = 0;
    }
  }

if (counter > 0){
  digitalWrite(relaylamp, LOW);
  digitalWrite(relaysocket, LOW);
  }

if (counter == 0){
  digitalWrite(relaylamp, HIGH);
  digitalWrite(relaysocket, HIGH);
  }

if (statelcd == 1)
  if (statelcd3 == 0){
    lcd.setCursor(0, 0);
    lcd.print("                ");
    lcd.setCursor(1, 0);
    lcd.print("                ");
    lcd.setCursor(0, 0);
    lcd.print("Counter Visitor");
    lcd.setCursor(1, 0);
    lcd.print("IN : ");
    lcd.setCursor(1, 5);
    lcd.print(counter);
    statelcd3 = 1;
    }
  if (statelcd2 == 1){
    lcd.setCursor(1, 5);
    lcd.print("   ");
    lcd.setCursor(1, 5);
    lcd.print(counter);
    statelcd2 = 0;
    }
}
