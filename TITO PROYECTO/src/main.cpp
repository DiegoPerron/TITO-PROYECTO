#include <Arduino.h>
#include <Q2HX711.h>

const byte hx711_data_pin = A2;
const byte hx711_clock_pin = A3;
char caso ; 
void loop()
{

  // Mandamos comandos para toma de temperatura a los sensores
  Q2HX711 hx711(hx711_data_pin, hx711_clock_pin);

  // Leemos y mostramos los datos de los sensores DS18B20
  String T1 = String(hx711.read()/100.0); // obtener t1
  String T2 = String(); // obtener t2P

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