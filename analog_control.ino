#include <LiquidCrystal.h>
#include <Servo.h> //library for servo motor

LiquidCrystal lcd(25, 29, 39, 41, 43, 45);
Servo Servo1; //Servo Up/Down
Servo Servo2; //Servo Left/Right
int angServo1 = 90;  
int angServo2 = 90;
//int Analog_Out = 11;

char SerialinData[50];
char *SerialinParse[20];
int Serialindex = 0;
boolean SerialstringComplete = false;

int Analog1_Value = 0;
int Analog2_Value = 0;
int Old_Slider1 = 0;
int Old_Slider2 = 0;
int Slider1_Value = 0;
int Slider2_Value = 0;

int Input1 = A8;
int Input2 = A9;
int Input3 = A10;
int Input4 = A11;
int Slider1 = A0;
int Slider2 = A1; 

int aux1[4] = {1, 1, 1, 1};
int Old_aux1[4] = {1, 1, 1, 1};
int aux2 = 0;

byte LEDpins[] = {2, 3, 4, 5, 6, 7, 8, 9};
int state;

void setup() {
  Serial.begin(115200);

  Servo1.attach(10); 
  Servo2.attach(11); 
  Servo1.write(angServo1); 
  Servo2.write(angServo2);
  //pinMode(Analog_Out, OUTPUT);
  
  pinMode(Input1,INPUT_PULLUP);
  pinMode(Input2,INPUT_PULLUP);
  pinMode(Input3,INPUT_PULLUP);
  pinMode(Input4,INPUT_PULLUP);
  pinMode(Slider1,INPUT);
  pinMode(Slider2,INPUT);

  for(int i = 0; i < 8; i++) {
    pinMode(LEDpins[i], OUTPUT);
  }

  lcd.begin(16, 2);
  lcd.print("LCD test");
  lcd.setCursor(0,1);
  lcd.print("Analog Control");
}

void loop() {

  if (SerialstringComplete) {
    ParseSerialData();
    memset(SerialinData, 0, sizeof(SerialinData));//clear the SerialinData string
    SerialstringComplete = false; 
  }

  if (Serial.available() && SerialstringComplete == false){
    char inChar = Serial.read();    
    if (inChar == '\n') 
    {
      Serialindex = 0;
      SerialstringComplete = true;
    }
    else
    {
      SerialinData[Serialindex] = inChar; 
      Serialindex++;
    }
  }
  else{
  aux1[0] = digitalRead(Input1);
  aux1[1] = digitalRead(Input2);
  aux1[2] = digitalRead(Input3);
  aux1[3] = digitalRead(Input4);
  if (aux1[0] != Old_aux1[0] or aux1[1] != Old_aux1[1] or aux1[2] != Old_aux1[2] or aux1[3] != Old_aux1[3]){
    Serial.print("B,");
    Serial.print(aux1[0]);
    Serial.print(",");
    Serial.print(aux1[1]);
    Serial.print(",");
    Serial.print(aux1[2]);
    Serial.print(",");
    Serial.print(aux1[3]);
    Serial.print(",");
    Old_aux1[0] = aux1[0];
    Old_aux1[1] = aux1[1];
    Old_aux1[2] = aux1[2];
    Old_aux1[3] = aux1[3];
  }
  Analog1_Value = analogRead(Slider1);
  Analog2_Value = analogRead(Slider2);
  Slider1_Value = Analog1_Value * (100.0 / 1023.0);
  Slider2_Value = Analog2_Value * (100.0 / 1023.0);
  if (Slider1_Value != Old_Slider1 or Slider2_Value != Old_Slider2){
    Serial.print("S,");
    Serial.print(Slider1_Value);
    Serial.print(",");
    Serial.print(Slider2_Value);
    Serial.print(",");
    Old_Slider1 = Slider1_Value;
    Old_Slider2 = Slider2_Value;
  }
  delay(20);
  }
}

void ParseSerialData(){
    char *p = SerialinData;
    char *str;   
    int count = 0;
    
    while ((str = strtok_r(p, ",", &p)) )
    {   
      SerialinParse[count] = str;
      count++;      
    }
    if(count >= 3)//
    {
      char *func = SerialinParse[0];
      char *prop1 = SerialinParse[1];
      char *prop2 = SerialinParse[2];   
      switch(*func)
      {
        case 'V': Update_LEDs(prop1, prop2); break;//Serial.println("OK"); break;//
      }
      //Serial.println("OK");
    }
  }

void Update_LEDs(char *prop1, char *prop2){    
  int value1 = atoi(prop1); 
  int value2 = atoi(prop2);
  //int Servo1_val = map(value, 0, 100, 0, 180);
  Servo1.write(value1 * 1.8);//Servo1_val); 
  Servo2.write(value2 * 1.8);//Servo2_val);
  //analogWrite(Analog_Out, value);
  for (int i = 1; i < 9; i++){
    if (value1 > i * 11)
      digitalWrite(LEDpins[i-1], HIGH);
    else
      digitalWrite(LEDpins[i-1], LOW);
  }
}
