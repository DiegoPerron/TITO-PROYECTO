#include <Arduino.h>
#include <HX711.h>

const byte hx711_data_pin = A1;  // DOUT
const byte hx711_clock_pin = A0; // CLK
const byte bomba1 = 2;     //bomba que mete agua
const byte bomba2 = 3;  //bomba que saca agua
const byte bombpwm1 = 5;         // PWM1
const byte bombpwm2 = 6;         // PWM2
HX711 scale;                     // Create an instance of the HX711 class
char caso;                       // Variable para el switch
// Configuramos los pines del sensor Trigger y Echo
const int PinTrig = 12;
const int PinEcho = 11;
// Constante velocidad sonido en cm/s
const float VelSon = 34000.0;
// Número de muestras
const int numLecturas = 20;
// Distancia a los 100 ml y vacío
const float distanciaVacio = 30;
float lecturas[numLecturas]; // Array para almacenar lecturas
int lecturaActual = 0;       // Lectura por la que vamos
float total = 0;             // Total de las que llevamos
float media = 0;             // Media de las medidas
bool primeraMedia = false;   // Para saber que ya hemos calculado por lo menos una
float distanciaLleno = 0;    // Distancia lleno

void setup()
{
  Serial.begin(9600); // Start serial communication
  // Mandamos comandos para toma de temperatura a los sensores
  scale.begin(hx711_data_pin, hx711_clock_pin); // Initialize the HX711 library
  scale.set_scale(439430.25);                   // Set the scale factor
  Serial.println(scale.read());                 // Read the current value
  scale.tare(0);                                // Tare the scale
  Serial.println(scale.get_units(20), 2);       // Read the current value in grams
  pinMode(bombpwm1, OUTPUT);                    // Set the PWM1 pin as an output
  pinMode(bombpwm2, OUTPUT);                    // Set the PWM2 pin as an output
  analogWrite(bombpwm1, 63);                    // Set the PWM1 pin to 63
  analogWrite(bombpwm2, 63);                    // Set the PWM2 pin to 63
  // Ponemos el pin Trig en modo salida
  pinMode(PinTrig, OUTPUT);
  // Ponemos el pin Echo en modo entrada
  pinMode(PinEcho, INPUT);
  // Inicializamos el array
  for (int i = 0; i < numLecturas; i++)
  {
    lecturas[i] = 0;
  }
}

void iniciarTrigger()
{
  // Ponemos el Triiger en estado bajo y esperamos 2 ms
  digitalWrite(PinTrig, LOW);
  delayMicroseconds(2);

  // Ponemos el pin Trigger a estado alto y esperamos 10 ms
  digitalWrite(PinTrig, HIGH);
  delayMicroseconds(10);

  // Comenzamos poniendo el pin Trigger en estado bajo
  digitalWrite(PinTrig, LOW);
}





void loop()
{

  // Eliminamos la última medida
  total = total - lecturas[lecturaActual];
 
  iniciarTrigger();
 
  // La función pulseIn obtiene el tiempo que tarda en cambiar entre estados, en este caso a HIGH
  unsigned long tiempo = pulseIn(PinEcho, HIGH);
 
  // Obtenemos la distancia en cm, hay que convertir el tiempo en segudos ya que está en microsegundos
  // por eso se multiplica por 0.000001
  float distancia = tiempo * 0.000001 * VelSon / 2.0;
 
  // Almacenamos la distancia en el array
  lecturas[lecturaActual] = distancia;
 
  // Añadimos la lectura al total
  total = total + lecturas[lecturaActual];
 
  // Avanzamos a la siguiente posición del array
  lecturaActual = lecturaActual + 1;
 
  // Comprobamos si hemos llegado al final del array
  if (lecturaActual >= numLecturas)
  {
    primeraMedia = true;
    lecturaActual = 0;
  }
 
  // Calculamos la media
  media = total / numLecturas;
 
  // Solo mostramos si hemos calculado por lo menos una media
  if (primeraMedia)
  {
    float distanciaLleno = distanciaVacio - media;
    
    Serial.println(media);
    Serial.print(" cm");
    
    
  }

  String T1 = String(scale.get_units(20), 2); // obtener t1P
  String T2 = String(distanciaLleno);                  // obtener t2P

  Serial.println(T1 + "A" + T2);
  delay(1000);

  if (Serial.available())
  {
    caso = Serial.read();
    if (caso == 'A')
    {
      digitalWrite(bomba1, HIGH);
      digitalWrite(bomba2, LOW);
    } // sasoa
    if (caso == 'B')
    {
      digitalWrite(bomba1, LOW);
      digitalWrite(bomba2, LOW);
    }  // casob
    if (caso == 'C')
    {
      digitalWrite(bomba1, LOW);
      digitalWrite(bomba2, HIGH);
    } // casoc

    if (caso == 'D')
    {
      digitalWrite(bomba1, LOW);
      digitalWrite(bomba2, LOW);
    } // casoD
  }   // casos

} // void
