#include <SoftwareSerial.h>
#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define OLED_RESET          A4
#define pin 7

#define SSD1306_WHITE 1
#define SSD1306_BLACK 0
#define SSD1306_INVERSE 2

Adafruit_SSD1306 display(OLED_RESET);

int analogInput = A1;

//Alarm reciever's phone number with country code
const String PHONE = "+91 9836686360";

//GSM Module RX pin to Arduino 3
//GSM Module TX pin to Arduino 2
#define rxPin 2
#define txPin 3
SoftwareSerial sim800(rxPin,txPin);

//the pin that the pir sensor is atteched to
#define trigPin 8
#define echoPin 9

void setup() {

  pinMode(pin,INPUT);
  digitalWrite(pin,LOW);

  pinMode(analogInput, INPUT);

  Serial.begin(9600);
  Serial.println("DC VOLTMETER");

  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);

  display.display();
  display.clearDisplay();

  display.display();

  display.drawRect(0, 0,  128 , 16, SSD1306_WHITE);
  display.setTextColor(SSD1306_WHITE, SSD1306_BLACK);
  display.setTextSize(1);
  display.setCursor(18, 4);
  display.println("- DC VOLTMETER -");
  display.display();
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  
//  Serial.begin(115200);          // initialize serial

  sim800.begin(9600);
  Serial.println("SIM800L software serial initialize");

  sim800.println("AT");
  delay(1000);
}

float vout = 0.0;
float vin = 0.0;

float R1 = 30000.0;
float R2 = 7500.0;

int value = 0;

void loop(){

//////////////////////////////////////////////////
value = analogRead(analogInput);
  vout = (value * 5.0) / 1024.0;
  vin = vout / (R2 / (R1 + R2));

  Serial.print("INPUT Voltage= ");
  Serial.println(vin, 2);

  display.setTextSize(1);
  display.setCursor(0, 25);
//  display.println("INPUT Voltage");

//  display.setTextSize(2);
//  display.setCursor(0, 35);
  display.println(vin, 2);
//  display.setCursor(1, 25);
//  display.println("V");
  display.display();

  delay(2000);
while(sim800.available()){
  Serial.println(sim800.readString());
}
//////////////////////////////////////////////////
while(Serial.available())  {
  sim800.println(Serial.readString());
}
//////////////////////////////////////////////////
  long time_duration, distance_in_cm;
  digitalWrite(trigPin, LOW); 
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  time_duration = pulseIn(echoPin, HIGH);
  distance_in_cm = time_duration / 29 / 2;

  Serial.print(distance_in_cm);
  Serial.println(" cm");
delay(1000);
  //Ranging Distance − 2cm – 400 cm
  //if the distance is equal or less then 10 cm
  if (distance_in_cm <= 5) {
    Serial.println("Motion detected!");
    Serial.println("calling....");
    Serial.println("sending sms....");
    sim800.println("ATD"+PHONE+";");
    sim800.println("AT+CMGF=1"); // Configuring TEXT mode
    delay(1000);
    sim800.println("AT+CMGS=\"+919836686360\"\r");//change ZZ with country code and xxxxxxxxxxx with phone number to sms
    delay(1000);
    sim800.print("Dustbim filled"); //text content
    delay(1000);
    sim800.write((char)26);
    delay(1000); //20 sec delay
  }

  delay(500);

  if(pin==HIGH){
    Serial.println("sending sms....");
    Serial.println("complain sent");
    sim800.println("ATD"+PHONE+";");
    sim800.println("AT+CMGF=1"); // Configuring TEXT mode
    delay(1000);
    sim800.println("AT+CMGS=\"+919836686360\"\r");//change ZZ with country code and xxxxxxxxxxx with phone number to sms
    delay(1000);
    sim800.print("Complain register"); //text content
    delay(1000);
    sim800.write((char)26);
  }
}
