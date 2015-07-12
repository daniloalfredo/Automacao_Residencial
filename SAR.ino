#include <IRremote.h>
#include <Servo.h>
#include <BitVoicer11.h>

// O LED IR deve estar conectado ao pino PWM 3

#define VOL_UP    4
#define VOL_DOWN  5
#define CH_UP     6
#define CH_DOWN   7
#define POWER     8
#define TV        10

//Variáveis
BitVoicerSerial bvSerial = BitVoicerSerial();
byte dataType = 0;
int valorComando = 0;
int ValLedB = 0;
int pinLedB = 2;
int ValLedS = 0;
int pinLedS = 3;  //TODO: Mudar, o IR usa o 3
int ValLedQ = 0;
int pinLedQ = 4;
int pinMotor = 9;
Servo motor;
IRsend irsend;
boolean ativarMotor = false;
int estado = 0;

boolean reverse = false;

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

void sendPhilips(int comando){

}

//inicialização
void setup() {
  Serial.begin(9600);
  pinMode(pinLedB, OUTPUT);
  pinMode(pinLedQ, OUTPUT);
  pinMode(pinLedS, OUTPUT);
  pinMode(3, OUTPUT);       //Para o IR
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
    if(estado == 0){
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
         case TV:
          Serial.println("Comecou TV");
          estado = 1;
          break;
        }
      }else{
        switch(valorComando)
        {
        case VOL_UP:
          Serial.println("Volume UP");
          break;
        case VOL_DOWN:
          Serial.println("Volume DOWN");      
          break;
        case CH_UP: 
          Serial.println("Channel UP");    
          break;
        case CH_DOWN:
          Serial.println("Channel Down");
          break;
        case POWER:
          /*for (int i = 0; i < 100; i++){
              irsend.sendSony(0xa90, 12); // Sony TV power code
              delay(40);
          }*/
          Serial.println("Power");
          break;
        case TV:        
          Serial.println("Cabou TV");
          estado = 0;
        break;
        }
      }
   }  
}
