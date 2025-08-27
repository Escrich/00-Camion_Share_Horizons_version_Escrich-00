//***************************************************************************************************
//
//   Versión del programa para mis dos camiones, modificados desde el diseño original de
//   Share Horizons
//   Jose M. Escrich 20250322
//
//   En esta versión se ha cambiado, comprobado y corregido, la nomenclatura de todas
//   las variables de dirección y control de motor.
//   He recalculado los angulos de la dirección, haciendolos simetricos
//   Tambien se han añadido rutinas para el cambio de color de los leds en función de la dirección
//   Añadidos nuevos leds Neopixel en el canal 5
//   De momento he deshechado la idea de poner algún tipo de elemento acustico
//   He traducido la mayoría de los comentarios, para hacerlo mas didactico para los que están empezando
//
//   Gracias Share horizons, Gracias Creality
//
//***************************************************************************************************


// ----------------------------------------------------------------------------
// Incluye las librerías que vamos a necesitar utilizar en el proyecto
// ----------------------------------------------------------------------------

#include <esp_now.h>
#include <WiFi.h>
#include <ESP32Servo.h>
#include <Adafruit_NeoPixel.h>

#define LED_PIN 2  // led en Wemos D1 Mini ESP32, mismo pin en DOIT ESP32 Devkit V1
bool led = LOW;    // variable para encendido del led

#define SIGNAL_TIMEOUT 1000  // This is signal timeout in milli seconds. We will reset the data if no signal
unsigned long lastRecvTime = 0;

// ----------------------------------------------------------------------------
// SERVOS, crea como objetos los servos existentes, para poder ser controlados como servos
// ----------------------------------------------------------------------------
// En un ESP32 puedes definir hasta 16 servos

Servo BrazoDerecho;
Servo BrazoIzquierdo;

//Posiciones de los servos
int PosBrazoDerecho = 0;
int PosBrazoIzquierdo = 0;
#define tiempobrazos 300  // Tiempo de espera para los servos de los brazos

// ----------------------------------------------------------------------------
// Definición del canal Neopixel que controla la tira led de debajo del camión
// ----------------------------------------------------------------------------

#define PIN_WS2812B 21  // El pin GPIO21 del ESP32 conecta con tira de leds del tipo WS2812B, a la que llamaremos ws2812b
#define NUM_PIXELS 8    // 12 // Numero de LEDs (pixels) en una tira led del tipo WS2812B
//#define TIMERPIXELS 500

Adafruit_NeoPixel ws2812b(NUM_PIXELS, PIN_WS2812B, NEO_GRB + NEO_KHZ800);


// ----------------------------------------------------------------------------
// Nuevo canal Neopixel el equivalente a una tira de cuatro leds
// ----------------------------------------------------------------------------

#define ledsBomberosPin 5  // El pin GPIO5 del ESP32 conecta con tira deleds del tipo WS2812B, a la que llamaremos ledBomberos
#define numLeds 4          // Numero de LEDs (pixels) en una tira led del tipo WS2812B, en este caso son leds sueltos, conectados como una tira de 4 leds

Adafruit_NeoPixel ledBomberos(numLeds, ledsBomberosPin, NEO_GRB + NEO_KHZ800);

//  Variables para la nueva tira de cuatro leds
int contador = 0;       // Contador para hacer los avances de paso
int longitudPaso = 15;  // numero de pasos de contador para avanzar un paso
int pasoActual = 0;     // paso actual
bool parado = LOW;      // variable que muestra si el camión está parado o en marcha

// ----------------------------------------------------------------------------
// Definicion de pines de conexión
// ----------------------------------------------------------------------------
#define BrazoDerechoPin 32
#define BrazoIzquierdoPin 33

//MOTORES IZQUIERDA
#define MotorENAPinL 23   // Permiso de marcha y llegada señal PWM
#define MotorIN1PinLB 27  // Marcha atras
#define MotorIN2PinLF 12  // Marcha adelante
//MOTORES DERECHA
#define MotorENBPinR 22   // Permiso de marcha y llegada señal PWM
#define MotorIN3PinRF 18  // Marcha adelante
#define MotorIN4PinRB 19  // Marcha atras

// ============================================================================================

// Siguiendo mi costumbre, he usado las iniciales de las palabras
// adelante, atras, izquierda y derecha, en ingles, en cada simbolo
// Su significado es el siguiente:
// L : Left; Izquierda
// R : Right; Derecha
// F : Forward; Adelante
// B : Backwards; Atras

// La tabla de la verdad para las entradas,
// uno y dos para el grupo de motores de la Izquierda,
// es la siguiente:

//  IN 1  IN 2  MOTOR
//  LOW   LOW   PARADO
//  HIGH  LOW   ATRAS
//  LOW   HIGH  ADELANTE
//  HIGH  HIGH  PARADO

// o tres y cuatro para el grupo de motores de la derecha,
// es la siguiente:

//  IN 4  IN 3  MOTOR
//  LOW   LOW   PARADO
//  HIGH  LOW   ATRAS
//  LOW   HIGH  ADELANTE
//  HIGH  HIGH  PARADO

// ============================================================================================

// ----------------------------------------------------------------------------
// Definición de variables
// ----------------------------------------------------------------------------

int Conexionperdida = 0;

int angulo = 0;
int modulo = 0;
int Boton = 0;
int velmin = 80;     //Velocidad minima a la que se mueven los motores //  OJO al dato, reducir cuanto se pueda
int Potencia = 0;    //Potencia Joystick
int PotenciaMR = 0;  //Potencia motores derecha
int PotenciaML = 0;  //Potencia motores izquierda

// Variables usadas para generar las diferentes intermitencias
int intermitencia = LOW;           // señal que cambia de estado para usar como intermitente
unsigned long anteriorMillis = 0;  // guarda el valor del tiempo del ultimo cambio
const long intervalo = 200;        // intervalo para intermitencia en milisegundos

//Paquete de datos recibido del mando
struct PacketData {
  int AnguloValue;
  int ModuloValue;
  int switchPressed;
};
PacketData receiverData;

// ----------------------------------------------------------------------------
// Función de rellamada que se ejecuta cuando se reciben datos
// ----------------------------------------------------------------------------

void OnDataRecv(const esp_now_recv_info_t *info, const uint8_t *incomingData, int len) {
  if (len == 0) {
    return;
  }
  memcpy(&receiverData, incomingData, sizeof(receiverData));
  String inputData;
  inputData = inputData + "values " + receiverData.AnguloValue + "  " + receiverData.ModuloValue + "  " + receiverData.switchPressed;
  Serial.println(inputData);
  lastRecvTime = millis();
}

// ----------------------------------------------------------------------------
// Escalado de lso datos del Joystick
// ----------------------------------------------------------------------------

void MapeoJoystick() {
  angulo = receiverData.AnguloValue;
  modulo = receiverData.ModuloValue;
  if (modulo > 255) {
    modulo = 255;
  }
  Boton = receiverData.switchPressed;
}


// ----------------------------------------------------------------------------
//  Rutina para el control de los leds llamados de bomberos
// ----------------------------------------------------------------------------

void BucleBomberos() {

  if (Conexionperdida == 1) {
    if (intermitencia == HIGH) {
      ledBomberos.setPixelColor(0, ledBomberos.Color(255, 0, 0));  // rojo
      ledBomberos.setPixelColor(1, ledBomberos.Color(0, 0, 0));    // apagado
      ledBomberos.setPixelColor(2, ledBomberos.Color(0, 0, 0));    // apagado
      ledBomberos.setPixelColor(3, ledBomberos.Color(255, 0, 0));  // rojo
      ledBomberos.show();

    } else {
      ledBomberos.setPixelColor(0, ledBomberos.Color(0, 0, 0));    // apagado
      ledBomberos.setPixelColor(1, ledBomberos.Color(255, 0, 0));  // rojo
      ledBomberos.setPixelColor(2, ledBomberos.Color(255, 0, 0));  // rojo
      ledBomberos.setPixelColor(3, ledBomberos.Color(0, 0, 0));    // apagado
      ledBomberos.show();
    }
  } else {
    if (parado == HIGH) {
      if (intermitencia == HIGH) {
        ledBomberos.setPixelColor(0, ledBomberos.Color(0, 255, 0));  // verde
        ledBomberos.setPixelColor(1, ledBomberos.Color(0, 0, 0));    // apagado
        ledBomberos.setPixelColor(2, ledBomberos.Color(0, 0, 0));    // apagado
        ledBomberos.setPixelColor(3, ledBomberos.Color(0, 255, 0));  // verde
        ledBomberos.show();

      } else {
        ledBomberos.setPixelColor(0, ledBomberos.Color(0, 0, 0));    // apagado
        ledBomberos.setPixelColor(1, ledBomberos.Color(0, 255, 0));  // verde
        ledBomberos.setPixelColor(2, ledBomberos.Color(0, 255, 0));  // verde
        ledBomberos.setPixelColor(3, ledBomberos.Color(0, 0, 0));    // apagado
        ledBomberos.show();
      }

    } else {
      contador++;                      // Incrementamos el contador a cada ciclo de programa
      if (contador >= longitudPaso) {  // Comprobamos si ha contado los suficientes ciclos como para dar un paso
        pasoActual++;                  // Si ha contado suficiente, avanzamos un paso y
        contador = 0;                  // ponemos el contador de nuevo a cero
      }

      // Encendemos los leds en función del paso, (despues de haber definido mas de 10 pasos, lo dejo solo en dos, ya que el efecto luminoso es muy bueno)
      switch (pasoActual) {
        case 0:
          ledBomberos.setPixelColor(0, ledBomberos.Color(0, 0, 255));
          ledBomberos.setPixelColor(1, ledBomberos.Color(255, 0, 0));
          ledBomberos.setPixelColor(2, ledBomberos.Color(255, 0, 0));
          ledBomberos.setPixelColor(3, ledBomberos.Color(0, 0, 255));
          ledBomberos.show();
          break;
        case 1:
          ledBomberos.setPixelColor(0, ledBomberos.Color(255, 0, 0));
          ledBomberos.setPixelColor(1, ledBomberos.Color(0, 0, 255));
          ledBomberos.setPixelColor(2, ledBomberos.Color(0, 0, 255));
          ledBomberos.setPixelColor(3, ledBomberos.Color(255, 0, 0));
          ledBomberos.show();
          break;

        default: pasoActual = 0;  // Si el paso actual no coincide con lo que esperamos, porque sea mayor, lo volvemos a poner a cero
      }
    }
  }
}


// ----------------------------------------------------------------------------
// Rutina que genera todas las señales intermitentes para ser usadas donde se necesiten
// ----------------------------------------------------------------------------

void Intermitencias() {
  // Generación de señal intermitente

  unsigned long actualMillis = millis();

  if (actualMillis - anteriorMillis >= intervalo) {
    // guarda el momento en que se produjo el ultimo cambio
    anteriorMillis = actualMillis;
    // Si la señal está a uno la pone a cero, si está a cero la pone a uno
    if (intermitencia == LOW)
      intermitencia = HIGH;
    else
      intermitencia = LOW;
  }

}  // Fin de Intermitencias()

// ----------------------------------------------------------------------------

//GRAFCET NEOPIXEL

int PixelEstado = 0;

void PixelInicializar() {
  PixelEstado = 0;
}

void BucleNeopixel() {
  switch (PixelEstado) {
    case 0:  //Inicialización
             // ws2812b.clear();  // set all pixel colors to 'off'. It only takes effect if pixels.show() is called
             // ws2812b.show();   // update to the WS2812B Led Strip
             // PixelTimer = millis();  // No parece que se use
      PixelEstado = 1;
      break;

    case 1:
      if (Conexionperdida == 1) {
        for (int pixel = 0; pixel < NUM_PIXELS; pixel++) {  //red
          ws2812b.setPixelColor(pixel, ws2812b.Color(255, 0, 0));
          ws2812b.show();
        }
        PixelEstado = 2;
      } else {
        for (int pixel = 0; pixel < NUM_PIXELS; pixel++) {  //green
          ws2812b.setPixelColor(pixel, ws2812b.Color(0, 255, 0));
          ws2812b.show();
        }
        PixelEstado = 3;
      }
      break;

    case 2:
      if (Conexionperdida == 0) {
        PixelEstado = 1;
      }
      break;

    case 3:
      if (Conexionperdida == 1) {
        PixelEstado = 1;
      }
      break;
  }
}

// -------------------------------------------------------------------
// Rutinas para manejo de leds en función de la dirección, para 8 leds
// -------------------------------------------------------------------

void ledsAdelante() {

  borraLeds();
  ws2812b.setPixelColor(3, ws2812b.Color(0, 0, 255));
  ws2812b.setPixelColor(4, ws2812b.Color(0, 0, 255));
  ws2812b.show();
}

void ledsAtras() {

  borraLeds();
  ws2812b.setPixelColor(0, ws2812b.Color(0, 0, 255));
  ws2812b.setPixelColor(7, ws2812b.Color(0, 0, 255));
  ws2812b.show();
}

void ledsDerecha() {

  borraLeds();
  ws2812b.setPixelColor(1, ws2812b.Color(0, 0, 255));
  ws2812b.setPixelColor(2, ws2812b.Color(0, 0, 255));
  ws2812b.show();
}

void ledsIzquierda() {

  borraLeds();
  ws2812b.setPixelColor(5, ws2812b.Color(0, 0, 255));
  ws2812b.setPixelColor(6, ws2812b.Color(0, 0, 255));
  ws2812b.show();
}

// Poner leds en su estado inicial cuando paran los motores
void borraLeds() {
  for (int pixel = 0; pixel < NUM_PIXELS; pixel++) {  //verde
    ws2812b.setPixelColor(pixel, ws2812b.Color(0, 255, 0));
    ws2812b.show();
  }
}

// -------------------------------------------------------------------
//  Rutina de manejo de los brazos, un buen trabajo de Sharehorizons
// -------------------------------------------------------------------

//Grafcet Empujar

int EstadoEmpujar = 0;
int TimerEmpujar = 0;
bool empujando = false;

void InicializarEmpujar() {
  EstadoEmpujar = 0;
  TimerEmpujar = 0;
  empujando = false;
  PosBrazoDerecho = 70;
  PosBrazoIzquierdo = 180 - PosBrazoDerecho;
}

void MoverBrazos() {
  BrazoDerecho.write(PosBrazoDerecho);
  BrazoIzquierdo.write(PosBrazoIzquierdo);
}

void CambiarPosicionBrazos() {
  if (empujando == false) {
    empujando = true;
    PosBrazoDerecho = 170;
    PosBrazoIzquierdo = 180 - PosBrazoDerecho;
  } else {
    empujando = false;
    PosBrazoDerecho = 70;
    PosBrazoIzquierdo = 180 - PosBrazoDerecho;
  }
}

void Empujar() {

  switch (EstadoEmpujar) {

    case 0:  //ESPERAMOS A QUE PULSEN EL SWITCH
      MoverBrazos();
      if (receiverData.switchPressed == true) {
        EstadoEmpujar = 1;
      }

      break;

    case 1:  //HAN PULSADO EL SWITCH
      CambiarPosicionBrazos();
      MoverBrazos();
      TimerEmpujar = millis();
      EstadoEmpujar = 2;

      break;

    case 2:  //ESPERAMOS QUE LOS SERVOS SE MUEVAN

      MoverBrazos();

      if (millis() >= TimerEmpujar + tiempobrazos) {
        TimerEmpujar = 0;
        EstadoEmpujar = 0;
      }

      break;
  }
}


//GRAFCET MOVIMIENTO
int EstadoMovimiento = 0;
int TimerMovimiento = 0;

void IniMovimiento() {
  EstadoMovimiento = 0;
  TimerMovimiento = 0;
}

// ----------------------------------------------------------------------------
// Rutinas de movimiento en función del angulo que les llega desde el Joystick
// ----------------------------------------------------------------------------

void Movimiento() {
  // He cambiado el reparto de angulos haciendolos simetricos, que por alguna razón no lo eran
  parado = LOW;  // reseteamos el flag que indica si el camión está parado o en marcha

  switch (EstadoMovimiento) {

    case 0:  //Inicializamos las variables
      EstadoMovimiento = 1;
      break;

    case 1:  //Esperemos a que muevan el joystick
      if (modulo > 0) {
        Potencia = map(modulo, 0, 255, velmin, 255);
        if (angulo >= 65 && angulo <= 115) {  //Z1: ADELANTE
          PotenciaML = Potencia;
          PotenciaMR = Potencia;
          adelante();
          ledsAdelante();                          // Enciende en azul los leds de marcha adelante
        } else if (angulo >= 25 && angulo < 65) {  //Z2: DIAGONAL DELANTE DERECHA
          PotenciaML = Potencia;
          PotenciaMR = velmin;
          adelante();
          ledsDerecha();                          // Enciende en azul los leds de girar a derecha
        } else if (angulo >= 0 && angulo < 25) {  //Z3: DERECHA
          PotenciaML = Potencia;
          PotenciaMR = 0;
          adelante();
          ledsDerecha();             // Enciende en azul los leds de girar a derecha
        } else if (angulo >= 335) {  //Z3: DERECHA
          PotenciaML = Potencia;
          PotenciaMR = 0;
          adelante();
          ledsDerecha();                             // Enciende en azul los leds de girar a derecha
        } else if (angulo > 115 && angulo <= 155) {  //Z8: DIAGONAL DELANTE IZQUIERDA
          PotenciaML = velmin;
          PotenciaMR = Potencia;
          adelante();
          ledsIzquierda();                           // Enciende en azul los leds de girar a izquierda
        } else if (angulo > 155 && angulo <= 215) {  //Z7: IZQUIERDA
          PotenciaML = 0;
          PotenciaMR = Potencia;
          adelante();
          ledsIzquierda();                           // Enciende en azul los leds de girar a izquierda
        } else if (angulo > 215 && angulo <= 245) {  //Z6: DIAGONAL DETRAS IZQUIERDA
          PotenciaML = velmin;
          PotenciaMR = Potencia;
          atras();
          ledsIzquierda();                           // Enciende en azul los leds de girar a izquierda
        } else if (angulo > 245 && angulo <= 295) {  //Z5: ATRAS
          PotenciaML = Potencia;
          PotenciaMR = Potencia;
          atras();
          ledsAtras();                              // Enciende en azul los leds de marcha atras
        } else if (angulo > 295 && angulo < 335) {  //Z4: DIAGONAL DETRAS DERECHA
          PotenciaML = Potencia;
          PotenciaMR = velmin;
          atras();
          ledsDerecha();  // Enciende en azul los leds de girar a derecha
        }
      } else {  //PARADO
        parar();
        borraLeds();
        parado = HIGH;  // Activamos el flag que nos dice que estamos parados
      }
      break;
  }
}

// ----------------------------------------------------------------------------
// Rutinas de marcha y dirección de los motores
// ----------------------------------------------------------------------------

void MLparado() {  //Motores Izquierda Parados
  analogWrite(MotorENAPinL, 0);
  digitalWrite(MotorIN1PinLB, LOW);
  digitalWrite(MotorIN2PinLF, LOW);
}
void MRparado() {  //Motores Derecha Parados
  analogWrite(MotorENBPinR, 0);
  digitalWrite(MotorIN3PinRF, LOW);
  digitalWrite(MotorIN4PinRB, LOW);
}
void MLadelante() {  //Motores Izquierda adelante
  analogWrite(MotorENAPinL, PotenciaML);
  digitalWrite(MotorIN1PinLB, LOW);
  digitalWrite(MotorIN2PinLF, HIGH);
}
void MRadelante() {  //Motores Derecha adelante
  analogWrite(MotorENBPinR, PotenciaMR);
  digitalWrite(MotorIN3PinRF, HIGH);
  digitalWrite(MotorIN4PinRB, LOW);
}
void MLatras() {  //Motores Izquierda atras
  analogWrite(MotorENAPinL, PotenciaML);
  digitalWrite(MotorIN1PinLB, HIGH);
  digitalWrite(MotorIN2PinLF, LOW);
}
void MRatras() {  //Motores Derecha atras
  analogWrite(MotorENBPinR, PotenciaMR);
  digitalWrite(MotorIN3PinRF, LOW);
  digitalWrite(MotorIN4PinRB, HIGH);
}
void parar() {  //Parar camión
  MRparado();
  MLparado();
}
void adelante() {  //Camión adelante
  MRadelante();
  MLadelante();
}
void atras() {  //Camión atras
  MRatras();
  MLatras();
}

void setup() {
  // En esta sección el código que pongas, solo se va a ejecutar una vez, al arranque:

  // ----------------------------------------------------------------------------
  // Definición de entradas y salidas
  // ----------------------------------------------------------------------------
  pinMode(MotorENAPinL, OUTPUT);
  pinMode(MotorIN1PinLB, OUTPUT);
  pinMode(MotorIN2PinLF, OUTPUT);
  pinMode(MotorENBPinR, OUTPUT);
  pinMode(MotorIN3PinRF, OUTPUT);
  pinMode(MotorIN4PinRB, OUTPUT);

  pinMode(LED_PIN, OUTPUT);

  // ----------------------------------------------------------------------------
  // Arranque del canal serie para comunicación por USB
  // ----------------------------------------------------------------------------
  Serial.begin(115200);

  // ----------------------------------------------------------------------------
  // Arranque del canal wifi y definición del modo de trabajo
  // ----------------------------------------------------------------------------
  WiFi.mode(WIFI_STA);

  delay(1000);



  // ----------------------------------------------------------------------------
  // Inicializa la comunicacion ESP-NOW,
  // ----------------------------------------------------------------------------

  // El ESP-NOW es un protocolo, que puede transportar poca información, tan solo 250 bytes, pero consigue un alcance del orden de 200 metros.
  if (esp_now_init() != ESP_OK) {
    Serial.print("Error initializing ESP-NOW\n");
    return;
  }
  esp_now_register_recv_cb(OnDataRecv);

  // standard 50 hz servo
  BrazoDerecho.setPeriodHertz(50);
  BrazoIzquierdo.setPeriodHertz(50);
  // Conecta los servos
  BrazoDerecho.attach(BrazoDerechoPin, 500, 2500);
  BrazoIzquierdo.attach(BrazoIzquierdoPin, 500, 2500);

  IniMovimiento();
  InicializarEmpujar();
  ws2812b.begin();      // initialize WS2812B strip object (REQUIRED)
  ledBomberos.begin();  // Inicializa el objeto ledBomberos, imprescindible inicializarlo para que funcione
  PixelInicializar();
}

void loop() {
  // Aquí se pone el código que ha de ejecutarse, repetidamente, a cada ciclo de programa:

  // ----------------------------------------------------------------------------
  // Comprobación de pérdida de señal.
  // ----------------------------------------------------------------------------
  unsigned long now = millis();

  if (now - lastRecvTime > SIGNAL_TIMEOUT)  // SE HA PERDIDO LA CONEXION CON EL MANDO
  {
    Conexionperdida = 1;
    parar();  //PARAR MOTORES
    IniMovimiento();
    Serial.print("MANDO OUT\n");
  } else {
    Conexionperdida = 0;
    MapeoJoystick();
    Movimiento();
    Empujar();
  }

  // ----------------------------------------------------------------------------
  //  Llamadas a diferentes sub-rutinas
  // ----------------------------------------------------------------------------

  BucleNeopixel();
  BucleBomberos();
  Intermitencias();

  // ----------------------------------------------------------------------------
  // Salida de información de proceso por pantalla
  // ----------------------------------------------------------------------------

  //  /*
  // estos son mis datos en pantalla
  // Serial.println("");
  Serial.print("Ángulo: ");
  Serial.print(angulo);
  Serial.print("  Módulo: ");
  Serial.print(modulo);
  Serial.print("  Botón: ");
  Serial.print(Boton);
  Serial.print("  PotenciaML: ");
  Serial.print(PotenciaML);
  Serial.print("  PotenciaMR: ");
  Serial.print(PotenciaMR);
  Serial.print("  Potencia: ");
  Serial.print(Potencia);
  Serial.print("  velmin: ");
  Serial.print(velmin);
  Serial.print("  Paso actual: ");
  Serial.print(pasoActual);
  Serial.print("  contador: ");
  Serial.print(contador);
  Serial.print("  parado: ");
  Serial.print(parado);


  Serial.println("");

  // ----------------------------------------------------------------------------
  // Muestra la Mac address
  // ----------------------------------------------------------------------------

  //Quita la linea justo debajo si quieres ver la mac address en el monitor serie, recuerda ponerlas despues para no perder tiempo de proceso
  /*
  Serial.println("Mac address del dispositivo:");
  Serial.println(WiFi.macAddress());
  */
  //Quita la linea justo encima si quieres ver la mac address en el monitor serie, recuerda ponerlas despues para no perder tiempo de proceso

  // */

  // ----------------------------------------------------------------------------
  // Encendido del led azul de la placa
  // ----------------------------------------------------------------------------
  if (led == LOW) {
    led = HIGH;
  } else {
    led = LOW;
  }
  digitalWrite(LED_PIN, led);
  // ----------------------------------------------------------------------------
}
