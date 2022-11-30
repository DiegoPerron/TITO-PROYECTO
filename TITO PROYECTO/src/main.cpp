#include <Arduino.h>
#include <HX711.h>

const byte hx711_data_pin = A1;  // DOUT
const byte hx711_clock_pin = A0;    // CLK
const byte bombpwm1 = 3;  // PWM1
const byte bombpwm2 = 5;  // PWM2
HX711 scale;  // Create an instance of the HX711 class
char caso;  // Variable para el switch

void setup()
{
  Serial.begin(9600);  // Start serial communication
  // Mandamos comandos para toma de temperatura a los sensores
  scale.begin(hx711_data_pin, hx711_clock_pin);  // Initialize the HX711 library
  scale.set_scale(439430.25);  // Set the scale factor
  Serial.println(scale.read());   // Read the current value
  scale.tare(0);  // Tare the scale
  Serial.println(scale.get_units(20),2);  // Read the current value in grams
  pinMode(bombpwm1, OUTPUT);  // Set the PWM1 pin as an output
  pinMode(bombpwm2, OUTPUT);  // Set the PWM2 pin as an output
  analogWrite(bombpwm1, 63);  // Set the PWM1 pin to 63
  analogWrite(bombpwm2, 63);    // Set the PWM2 pin to 63
}

void loop()
{
  String T1 = String(scale.get_units(20),2);    // obtener t1P
  Serial.println(scale.read()); // obtener t1
  String T2 = String("23");                     // obtener t2P

  Serial.print(T1 + "A" + T2);
  delay(1000);

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