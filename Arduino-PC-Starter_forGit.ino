#include <Adafruit_NeoPixel.h>
#include <SPI.h>
#include <MFRC522.h>
#define PIN 5

constexpr uint8_t RST_PIN = 9;     // MFRC522 RFID Module RST Pin9
constexpr uint8_t SS_PIN = 10;     // MFRC522 RFID Module SDA Pin10

MFRC522 mfrc(SS_PIN, RST_PIN);           
Adafruit_NeoPixel pixels = Adafruit_NeoPixel(1, PIN, NEO_GRB + NEO_KHZ800); //adafruit Neopixel setting

int Relay = 3;                            // relay pin 3

void setup() {
  Serial.begin(9600);                     // serial 9600
  while (!Serial);    
  SPI.begin();                             // reset SPI
  mfrc.PCD_Init();    // Init MFRC522
  pinMode (Relay, OUTPUT);                 
  pixels.begin();//neopixel starter
  pixels.setPixelColor(0, pixels.Color(0,0,0));//neopixel starter
  pixels.show();// neopixel apply
 }

void loop() {
  start:
   if ( ! mfrc.PICC_IsNewCardPresent() || ! mfrc.PICC_ReadCardSerial() ) {    //if not tagged or not read the Tag ID
    delay(500);        
    return;                                   
  }
    
  if(mfrc.uid.uidByte[0] == 48 && mfrc.uid.uidByte[1] == 60 
       && mfrc.uid.uidByte[2] == 249 && mfrc.uid.uidByte[3] == 25) {    // Correct tag
    digitalWrite (Relay, HIGH);                // pin3 realy On
    Serial.print("Right Card Green LED on \n:"); //for debugging
    pixels.setPixelColor(0, pixels.Color(0,255,0));//Neo pixel Green On
    pixels.show();//Neopixel Apply
    delay(300);
    digitalWrite (Relay, LOW);                // Relay off
    goto kill;
     }
  
  else 
  {
    Serial.print("Wrong Card! Who are You?: \n");  //for debugging                                  
    pixels.setPixelColor(0, pixels.Color(127,0,0));//Red LED On
    pixels.show();//Neopixel Apply
    Serial.print("RED LED ON \n"); //for debugging
    delay(3000);
    pixels.setPixelColor(0, pixels.Color(0,0,0));// LED Off
    pixels.show();//Neopixel Apply
    Serial.print("RED LED OFF \n"); //for debugging
    Serial.print("goto start \n");
    goto start;
  
  }  
  kill:
  delay(3000);
  pixels.setPixelColor(0, pixels.Color(0,0,0));// Neopixel off
  Serial.print("LED OFF: \n");  //for debugging
  pixels.show();//apply on the neopixel
  Serial.print("goto start: \n");  //for debugging
  goto start;
 }
