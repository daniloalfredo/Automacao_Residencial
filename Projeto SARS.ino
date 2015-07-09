#include <BitVoicer11.h>

//Variáveis
BitVoicerSerial bvSerial = BitVoicerSerial();
byte dataType = 0;
int valorComando = 0;
byte ValLed = 0;
byte pinLed = 13;
int estado = 1;

//inicialização
void setup() {
  Serial.begin(9600);
  pinMode(pinLed, OUTPUT);
}

void loop() {
  digitalWrite(13, ValLed);
}  

//identificação do comando
void serialEvent()
{
  dataType = bvSerial.getData();
  if (dataType == BV_INT)
  {
    valorComando = bvSerial.intData;
    switch(valorComando)
    case 1: 
      ValLed = !ValLed;
   }  
}
