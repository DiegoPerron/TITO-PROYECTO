#include "string.h"


void setup() {
  // put your setup code here, to run once:
 Serial.begin(9600); // Start serial communication
}

String cadena = "1N9.9754657889";
String numeroEnString = "0.00";
char caracter;

void loop() {
  // put your main code here, to run repeatedly:

   caracter = cadena[3];

   numeroEnString[0] = cadena[2];
   numeroEnString[1] = cadena[3];
   numeroEnString[2] = cadena[4];
   numeroEnString[3] = cadena[5];
   numeroEnString[4] = cadena[6];
   numeroEnString[5] = cadena[7];
   numeroEnString[6] = cadena[8];
   numeroEnString[7] = cadena[9];


   Serial.println(numeroEnString);

   

}
