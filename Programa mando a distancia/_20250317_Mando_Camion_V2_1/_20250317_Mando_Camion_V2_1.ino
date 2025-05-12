// -------------------------------------------------------------------------------
//
//  Programa para el mando a distancia de los camiones de Sharehorizons
//
//  emite usando el protocolo ESPNOW al receptor que es el camión
//
//  Totalmente operacional en ambos modelos de mandos a distancia, 
//  posiblemente necesite retocar algunos parametros, 
//  en función de la posición fisica del Joystick, respecto al propio mando en si.
//
//  Ligeramente reformado por Jose M. Escrich - 20250512
//
// -------------------------------------------------------------------------------


// Bibliotecas a incluir
#include <esp_now.h>
#include <WiFi.h>
#include <math.h>

// Definición de pines
#define X_AXIS_PIN 32 // Los numeros de pin se mantienen, aunque las funciones de X e Y se intercambian
#define Y_AXIS_PIN 33 // Los numeros de pin se mantienen, aunque las funciones de X e Y se intercambian
#define SWITCH_PIN 25 // Interruptor del Joystick, se activa al pulsarlo
#define LED_PIN 2     // led en Wemos D1 Mini ESP32, mismo pin en DOIT ESP32 Devkit V1

// MAC DEL RECEPTOR
// Comentar la linea que no se use
//uint8_t receiverMacAddress[] = { 0xAA,0xAA,0xAA,0xAA,0xAA,0xAA };  // AA:AA:AA:AA:AA:AA // Escrich // 20250317
// AA:AA:AA:AA:AA:AA Camión de bomberos

uint8_t receiverMacAddress[] = { 0xAA,0xAA,0xAA,0xAA,0xAA,0xAA };  // AA:AA:AA:AA:AA:AA // Escrich // 20250317
// AA:AA:AA:AA:AA:AA Camión con radar


// Definición de variables
double angulo;
double opuesto;
double continuo;
double modulo;

double ejeX;  // Usados solo para debug
double ejeY;  // Usados solo para debug

bool led = LOW;  // variable para encendido del led que va en la placa

// Definición de la estructura de datos, que envía los valores del Joystick al camión

struct PacketData {
  int AnguloValue;
  int ModuloValue;
  int switchPressed;
};
PacketData data;

//This function is used to map 0-4095 joystick value to 0-254. hence 127 is the center value which we send.
//It also adjust the deadband in joystick.
//Jotstick values range from 0-4095. But its center value is not always 2047. It is little different.
//So we need to add some deadband to center value. in our case 1800-2200. Any value in this deadband range is mapped to center 127.

double MapeoJoystick(int valor) {

  if (valor >= 2200) {
    valor = map(valor, 2200, 4095, 0, 255);
  } else if (valor <= 1800) {
    valor = map(valor, 1800, 0, 0, -255);
  } else {
    valor = 0;
  }

  return valor;
}

/*
// Esta parte es el original ## 20250321 # Escrich
// He cambiado de lugar el signo "-" para invertir ambos ejes, ya que me viene mejor
// la posición fisica del joystick, con los cables mirando hacia el centro del mando
double MapeoJoystick(int valor) {

  if (valor >= 2200) {
    valor = map(valor, 2200, 4095, 0, -255);
  } else if (valor <= 1800) {
    valor = map(valor, 1800, 0, 0, 255);
  } else {
    valor = 0;
  }

  return valor;
}
*/

// callback when data is sent

void OnDataSent(const uint8_t *mac_addr, esp_now_send_status_t status) {
  /*
  Serial.print("\r\nLast Packet Send Status:\t ");
  Serial.println(status);
  Serial.println(status == ESP_NOW_SEND_SUCCESS ? "Message sent" : "Message failed");
  */
}

void setup() {
  Serial.begin(115200);
  WiFi.mode(WIFI_STA);
  delay(1000);

  // Init ESP-NOW
  if (esp_now_init() != ESP_OK) {
    Serial.println("Error initializing ESP-NOW");
    return;
  } else {
    Serial.println("Succes: Initialized ESP-NOW");
  }

  esp_now_register_send_cb(OnDataSent);

  // Register peer
  //esp_now_peer_info_t peerInfo; // Linea original
  esp_now_peer_info_t peerInfo = {}; // Añadido en la ultima versión de Shere horizons para evitar bloqueos
  memcpy(peerInfo.peer_addr, receiverMacAddress, 6);
  peerInfo.channel = 0;
  peerInfo.encrypt = false;

  // Add peer
  if (esp_now_add_peer(&peerInfo) != ESP_OK) {
    Serial.println("Failed to add peer");
    return;
  } else {
    Serial.println("Succes: Added peer");
  }

  pinMode(SWITCH_PIN, INPUT_PULLUP);
  pinMode(LED_PIN, OUTPUT);

} // Fin de setup

void loop() {
  data.switchPressed = false;

  if (digitalRead(SWITCH_PIN) == LOW) {
    data.switchPressed = true;
  }

  opuesto = MapeoJoystick(analogRead(X_AXIS_PIN));
  continuo = MapeoJoystick(analogRead(Y_AXIS_PIN));
  angulo = atan2(opuesto, continuo);
  angulo = 180 * (double)angulo / PI;
  if (angulo < 0) {
    angulo = angulo + 360;
  }
  modulo = sq(opuesto) + sq(continuo);
  modulo = sqrt(modulo);

  if (modulo <= 30) {
    modulo = 0;
  }
  if (modulo > 255) {
    modulo = 255;
  }

  data.AnguloValue = int(trunc(angulo));
  data.ModuloValue = int(trunc(modulo));


  // Comentar tras la puesta en marcha // Escrich
  /*
  // Esta parte es el original
  Serial.print("Ángulo: ");
  Serial.println(data.AnguloValue);
  Serial.print("Módulo: ");
  Serial.println(data.ModuloValue);
  Serial.print("Botón: ");
  Serial.println(data.switchPressed);
*/

  ejeX = analogRead(X_AXIS_PIN);  // Usados solo para debug
  ejeY = analogRead(Y_AXIS_PIN);  // Usados solo para debug

  // /*
  // estos son mis datos en pantalla
  //Serial.println("");
  Serial.print("Ángulo: ");
  Serial.print(data.AnguloValue);
  Serial.print("  Módulo: ");
  Serial.print(data.ModuloValue);
  Serial.print("  Botón: ");
  Serial.print(data.switchPressed);
  Serial.print("  Opuesto X mapeado: ");
  Serial.print(opuesto);
  Serial.print("  Continuo Y mapeado: ");
  Serial.print(continuo);
  Serial.print("  Eje X: ");
  Serial.print(ejeX);
  Serial.print("  Eje Y: ");
  Serial.print(ejeY);

  Serial.println("");
  // */


  esp_err_t result = esp_now_send(receiverMacAddress, (uint8_t *)&data, sizeof(data));
  if (result == ESP_OK) {
    //Serial.println("Sent with success"); // Texto en el terminal eliminado por claridad
  } else {
    // Serial.println("Error sending the data"); // Texto en el terminal eliminado por claridad
  }
  delay(150);  // # 200 # Reducir este valor hace que los datos de el mando se envían mas veces por segundo

  // Encendido del led azul de la placa
  // ===================================
  if (led == LOW) {
    led = HIGH;
  } else {
    led = LOW;
  }
  digitalWrite(LED_PIN, led);
  // ===================================


}  // Fin de loop
