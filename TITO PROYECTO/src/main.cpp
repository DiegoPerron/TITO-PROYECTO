#include <Arduino.h>
#include <HX711.h>

const byte hx711_data_pin = A1;  // DOUT
const byte hx711_clock_pin = A0; // CLK
const byte bomba1 = 2;           // bomba que mete agua
const byte bomba2 = 3;           // bomba que saca agua
const byte bombpwm1 = 9;         // PWM1
const byte bombpwm2 = 6;         // PWM2
HX711 scale;                     // Create an instance of the HX711 class
char caso;                       // Variable para el switch
String cadena = "";              // String para el peso
String numeroEnString = "0.00";  // String para el peso
float stp = 0.00;                // Setpoint de peso
float T2F = 0.00;
float T1F = 0.00;
char caracter;
String enable = "";
String magnitud = "";
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
float mediareal = 0;         // Media real
bool state = true;          // Estado de la medicion
String stptemp = "";         // Setpoint de temperatura temp

void setup()
{
  Serial.begin(9600); // Start serial communication
  // Mandamos comandos para toma de temperatura a los sensores
  scale.begin(hx711_data_pin, hx711_clock_pin); // Initialize the HX711 library
  scale.set_scale(271253.24);                   // Set the scale factor
  Serial.println(scale.read());                 // Read the current value
  scale.tare(0);                                // Tare the scale
  Serial.println(scale.get_units(20), 2);       // Read the current value in grams
  pinMode(bombpwm1, OUTPUT);                    // Set the PWM1 pin as an output
  pinMode(bombpwm2, OUTPUT);                    // Set the PWM2 pin as an output
  analogWrite(bombpwm1, 120);                   // Set the PWM1 pin to 63
  analogWrite(bombpwm2, 200);                   // Set the PWM2 pin to 63
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

float dist()
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
    distanciaLleno = distanciaVacio - media;
  }
  delay(10);
  return distanciaLleno;
}

float lectura()
{
  for (int i = 0; i < 10; i++)
  {
    mediareal = mediareal + dist();
  }
  mediareal = mediareal / 10;
  return mediareal;
}

float recort_string(String stptemp)
{
  stptemp = cadena[3];

  numeroEnString[0] = cadena[2];
  numeroEnString[1] = cadena[3];
  numeroEnString[2] = cadena[4];
  numeroEnString[3] = cadena[5];
  numeroEnString[4] = cadena[6];
  numeroEnString[5] = cadena[7];
  numeroEnString[6] = cadena[8];
  numeroEnString[7] = cadena[9];

  return numeroEnString.toFloat();
}

String mag_recort(String stptemp)
{
  stptemp = cadena[1];

  numeroEnString[0] = cadena[0];
  numeroEnString[1] = cadena[1];

  return numeroEnString;
}

String enable_recort(String stptemp)
{
  stptemp = cadena[0];

  numeroEnString[0] = cadena[0];
  numeroEnString[1] = cadena[1];

  return numeroEnString;
}

void loop()
{
  if (state == false)
  {
    String T2 = String(lectura()); // obtener t2P
    T2F = T2.toFloat();
    String T1 = String(0); // obtener t1P
    Serial.println(T1 + "A" + T2);
  }
  if (state == true)
  {
    String T2 = String(0);                      // obtener t2P
    String T1 = String(scale.get_units(20), 2); // obtener t1P
    T1F = T1.toFloat();
    Serial.println(T1 + "A" + T2);
  }

  if (Serial.available() > 0)
  {
    stp = recort_string(Serial.readString());
    magnitud = mag_recort(Serial.readString());
    enable = enable_recort(Serial.readString());

    if (enable == "1"){
      //---Habilita control
      if (magnitud == "N")
      {
        //---Control de nivel
        state = false;
        if (T2F > (stp + 0.25))
        {
          digitalWrite(bomba1, LOW);
          digitalWrite(bomba2, HIGH);
        }
        else if (T2F > (stp - 0.25) && T2F < (stp + 0.25))
        {
          digitalWrite(bomba1, LOW);
          digitalWrite(bomba2, LOW);
        }
        else if (T2F < (stp - 0.25))
        {
          digitalWrite(bomba1, HIGH);
          digitalWrite(bomba2, LOW);
        }

      }
      else if (magnitud == "P")
      {
        //---Control de peso
        state = true;
        if (T1F > (stp + 0.025)){
          digitalWrite(bomba1, LOW);
          digitalWrite(bomba2, HIGH);
        }
        else if (T1F > (stp - 0.025) && T1F < (stp + 0.025))
        {
          digitalWrite(bomba1, LOW);
          digitalWrite(bomba2, LOW);
        }
        else if (T1F < (stp - 0.025))
        {
          digitalWrite(bomba1, HIGH);
          digitalWrite(bomba2, LOW);
        }
      }
    }
    else if (enable == "0"){
      //---Deshabilita control
      digitalWrite(bomba1, LOW);
      digitalWrite(bomba2, LOW);
    }
} // void