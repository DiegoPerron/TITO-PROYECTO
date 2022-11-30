#include <Arduino.h>
#include <HX711.h>

const byte hx711_data_pin = A1;
const byte hx711_clock_pin = A0;
const byte bombpwm1 = 3;
const byte bombpwm2 = 5;
char caso;

void setup()
{
  Serial.begin(9600);
  // Mandamos comandos para toma de temperatura a los sensores
  scale.begin(hx711_data_pin, hx711_clock_pin);
  pinMode(bombpwm1, OUTPUT);
  pinMode(bombpwm2, OUTPUT);
}

void loop()
{

  // Leemos y mostramos los datos de los sensores DS18B20
  Serial.println(scale.read()); // obtener t1
  String T2 = String("23");                     // obtener t2P
  String T2 = String("23");  

  Serial.print(T1 + "A" + T2);
  delay(100);

  if (Serial.available())
  {
    caso = Serial.read();
    if (caso == 'A')
    {
      digitalWrite(7, LOW);
      digitalWrite(6, LOW);
      digitalWrite(4, HIGH);
      digitalWrite(5, HIGH);
    } // sasoa
    if (caso == 'B')
    {
      digitalWrite(7, LOW);
      digitalWrite(6, LOW);
      digitalWrite(4, LOW);
      digitalWrite(5, LOW);
    } // casob
    if (caso == 'C')
    {
      digitalWrite(4, LOW);
      digitalWrite(5, LOW);
      digitalWrite(6, HIGH);
      digitalWrite(7, HIGH);
    } // casoc

    if (caso == 'D')
    {
      digitalWrite(7, HIGH);
      digitalWrite(6, HIGH);
      digitalWrite(4, HIGH);
      digitalWrite(5, HIGH);
    } // casob
  }   // casos

} // void