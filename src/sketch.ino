#include <SoftwareServo.h>
#include <LiquidCrystal.h>
//#include <OneWire.h>
//#include <DallasTemperature.h>
#include <math.h>
//#include <Thermistor4.h>

#define servoPin 6
SoftwareServo myservo;
#define servo_max 90
#define servo_min 0

// initialize the library with the numbers of the interface pins
// LiquidCrystal lcd(12, 11, 5, 4, 3, 2);
LiquidCrystal lcd(7, 8, 9, 10, 11, 12);

float Vout;
float R2;
float buffer;
//float dstemp=0;
float Ttemp;
float TK;
int set_temp;

float error;
float Kp;
float Ki;
float terror=0;
#define dt 1
int pid=0;
int pid_last=0;
#define pid_delay 60000
//#define pid_delay 60000

unsigned long time=0;
unsigned long pid_time;
int seconds=0;
int minutes=-1;
unsigned int hours=0;
#define buttonPin 4
int buttonState=0;

#define Vin 5
#define R1 10000

#define ThermistorPin 0
#define DsPin 5

#define A1 -5.592569324221352e-04
#define A2 5.396561643682833e-04
#define A3 -2.816959457164149e-05
#define A4 8.351502163577249e-07
//OneWire onewire(DsPin);
//DallasTemperature ds(&onewire);

void setup() {
  myservo.attach(6);
  lcd.begin(20, 4);
  pid_time=millis()+60000;
  pinMode(buttonPin, INPUT);
  //ds.begin();
  //lcd.print("Set servo Max");
  //while(true){
    pinMode(13,OUTPUT);
  //}
}

void loop() {
  // set the cursor to column 0, line 1
  // (note: line 1 is the second row, since counting begins with 0):
  //ds.requestTemperatures();
  //dstemp=ds.getTempFByIndex(0);

  Vout=(5.0/1023.0)*analogRead(0);
  buffer = (Vin/Vout)-1;
  R2=R1/buffer;
  TK=1/(A1+A2*log(R2)+A3*pow(log(R2),2)+A4*pow(log(R2),3));//Thermistor equation
  Ttemp=(TK-273.15)*1.8+32;//Convert to F

  //set_temp=100*analogRead(1)/1023.0+100+0.5; // Percentage rounded to int and offset by 100 (100-200 pot)
  set_temp=map(analogRead(1), 0, 1023, 100, 200)+0.5;

  Kp=5*analogRead(2)/1023.0;//0-5 pot
  Ki=0.5*analogRead(3)/1023.0;//0-1 pot
  //Kp=map(analogRead(2),0,1023,0,5);
  //Ki=map(analogRead(3),0,1023,0,0.5);
  if (pid_delay<millis()-pid_time){
    error=set_temp-Ttemp;
    terror=terror+error*dt;
    pid=Kp*error+Ki*terror+0.5;

    if (pid > servo_max){
        pid=servo_max;
    }
    else if (pid < servo_min){
        pid=servo_min;
    }
    pid_time=millis();
    //myservo.write(pid);
  }
  buttonState=digitalRead(buttonPin);
  if (buttonState == HIGH){
    time=millis();
    seconds=0;
    minutes=-1;
    hours=0;
  }//else{
      //}
  
  seconds=((millis()-time)/(1000))%60;
  if (seconds==0){
    minutes++;
    if (minutes > 59){
        minutes=0;
        hours++;
    }
  }

  //Thermistor.ReadCalculate(3);
  //Ttemp=Thermistor.GetFarenheit();
  lcd.clear();
  lcd.print("BEVO BBQ!");
  //lcd.setCursor(18,0);
  //lcd.print(buttonState);

  lcd.setCursor(0,1);
  //lcd.print("dsT: ");
  //lcd.print(dstemp);
  lcd.print("T: ");
  lcd.print((int)(Ttemp+0.5));
  lcd.print((char)223);
  lcd.print("F");

  lcd.setCursor(10,1);
  lcd.print("Set: ");
  lcd.print(set_temp);
  lcd.print((char)223);
  lcd.print("F");

  lcd.setCursor(0,2);
  lcd.print("PI:");
  lcd.print((int)(pid+0.5));
  lcd.print("Kp:");
  lcd.print(Kp);
  lcd.print("Ki:");
  lcd.print(Ki);

  lcd.setCursor(0, 3);
  lcd.print("Timer: ");
  if (hours<10){
      lcd.print("0");
  }
  lcd.print((int)hours);
  lcd.print(":");
  if (minutes<10){
      lcd.print("0");
  }
  lcd.print(minutes);
  lcd.print(":");
  if (seconds<10){
      lcd.print("0");
  }
  lcd.print(seconds);
  //for(int i=0; i<100; i=+10){
    if(pid!=pid_last){
    myservo.write(pid);
    SoftwareServo::refresh();
    pid_last=pid;
    }
    delay(1000);
  //}
}
