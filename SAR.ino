//#include <IRremote.h>
#include <Servo.h>
#include <BitVoicer11.h>

//Variáveis
BitVoicerSerial bvSerial = BitVoicerSerial();
byte dataType = 0;
int valorComando = 0;
int ValLedB = 0;
int pinLedB = 2;
int ValLedS = 0;
int pinLedS = 3;
int ValLedQ = 0;
int pinLedQ = 4;
int pinMotor = 9;
Servo motor;
boolean ativarMotor = false;

boolean reverse = false;

int estado = 1;

void doMotor()
{
  float angle = motor.read();
  if(angle > 90)
  {
    for (; angle > 0; angle--)  
    {                                  
      motor.write(angle);               
      delay(15);                   
    }
  }
  else if (angle < 90)
  {
    for(; angle < 180; angle++)
    {
      motor.write(angle);
      delay(15);
    }
  }
  ativarMotor = false;
} 

//inicialização
void setup() {
  Serial.begin(9600);
  pinMode(pinLedB, OUTPUT);
  pinMode(pinLedQ, OUTPUT);
  pinMode(pinLedS, OUTPUT);
  motor.attach(pinMotor);
}
void loop() {
  digitalWrite(pinLedB, ValLedB);
  digitalWrite(pinLedQ, ValLedQ);
  digitalWrite(pinLedS, ValLedS);
  if(ativarMotor)
    doMotor();
}  

//identificação do comando
void serialEvent()
{
  dataType = bvSerial.getData();
  if (dataType == BV_INT)
  {
    valorComando = bvSerial.intData;
    switch(valorComando)
    {
    case 1:
      ValLedQ = !ValLedQ;
      break;
    case 2:
      ValLedS = !ValLedS;
      break;
    case 3: 
      ValLedB = !ValLedB;
      break;
    case 9:
      ativarMotor = true;
      break;
    }
   }  
}
