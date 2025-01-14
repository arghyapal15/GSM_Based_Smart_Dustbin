#include <SoftwareSerial.h>
#include<Servo.h>
Servo s1;
int soilSensor = A5;
//int servoMotor = 5;
//int irSensor = 4;


int servoMotor = 11;
int irSensor = 12;
int valIr;
int  valSoil;

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
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  
  Serial.begin(115200);          // initialize serial

  sim800.begin(9600);
  Serial.println("SIM800L software serial initialize");

  sim800.println("AT");
  delay(1000);
  pinMode(soilSensor, INPUT);
pinMode(irSensor, INPUT);
s1.attach(servoMotor);
Serial.begin(9600);
}

void loop(){

   valSoil = analogRead(soilSensor);
 valIr = digitalRead(irSensor);
    s1.write(90);
    delay(1000);
if(valIr == 0){

    if(valSoil > 580){
          s1.write(0);
           delay(3000);
      Serial.println(valSoil);
 }
 
 else if(valSoil > 450 && valSoil < 580){
   s1.write(130);
     delay(3000);
      
     Serial.println(valSoil);
 }
}
if(valIr == 1){
  s1.write(90);
    delay(1000);

}

//////////////////////////////////////////////////
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
//    sim800.println("ATD"+PHONE+";");
    sim800.println("AT+CMGF=1"); // Configuring TEXT mode
    delay(1000);
    sim800.println("AT+CMGS=\"+919836686360\"");//change ZZ with country code and xxxxxxxxxxx with phone number to sms
    delay(1000);
    sim800.print("2154"); //text content
    delay(1000);
    sim800.write(26);
    delay(1000); //20 sec delay
  }

  delay(500);
}
