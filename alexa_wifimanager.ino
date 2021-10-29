/**********************************************************************************
* Controle Alexa - REV. 0.1
* By Fernando Belíssimo - 09agosto2021
* 
* Utilizando a biblioteca "WIFIMANAGER" para controlar o acesso a rede wifi e a 
* biblioteca "ESPALEXA" para a interface entre o micro controlador e a Alexa
* * 
* REVISÕES:
* REV0.1: versão inicial com um comando apenas  
* REV0.2: Melhorado métodos de conexão. Alterado legendas. Nome do AP= Alexa_ap (18agosto2021)
* REV0.3: Otimização da rotina de conexão, visualização da senha de acesso do Roteador, eliminação de senha para acesso ao Access Point
*         Inclusão de circuito para "limpar" credenciais gravadas.Acrescentado indicador de conexão. (23agosto2021)
*   
*********************************************************************************/

#ifdef ARDUINO_ARCH_ESP32
//#include <WiFi.h>
#else
#include <ESP8266WiFi.h>
#endif
//#define ESPALEXA_DEBUG
#define ESPALEXA_MAXDEVICES 2
#include <Espalexa.h>
#include <HTTPClient.h>
#include <DNSServer.h> 
#include <WebServer.h>
#include <EEPROM.h>
#include "WiFiManager.h"
#include "Arduino.h" 

WiFiManager wifiManager;

#define LED_BUILTIN 2
int i = 0;
int statusCode;
const char* ssid = "Fernando_1";
const char* passphrase = "Fernando";
String st;
String content;

// define the GPIO connected with Relays

#define RelayPin1 13  //D1
#define led_conn 12 // 
//#define chave_res 2  
const int chave_res = 4;

//direct callback functions
void device1Changed(uint8_t brightness);

//acrescentar devices aqui

//special callback functions

// device names
String Device_1_Name = "massageador";

// device_function names

Espalexa espalexa;

// prototypes

//Establishing Local server at port 80 whenever required
WebServer server(80);
//ATÉ AQUI NOVO WIFI - FERNANDO

void setup()
{
  Serial.begin(115200);
  Serial2.begin(9600);
//NOVO WIFI - FERNANDO

WiFiManager wifiManager;




 if(!wifiManager.autoConnect())

  
 {
        Serial.println("Falha ao conectar");
        delay(2000);
        ESP.restart();
 }
  
 Serial.println("IP:");
 Serial.println(WiFi.localIP());

  //callback para quando entra em modo de configuração AP
  wifiManager.setAPCallback(configModeCallback); 
  //callback para quando se conecta em uma rede, ou seja, quando passa a trabalhar em modo estação
  wifiManager.setSaveConfigCallback(saveConfigCallback); 

  pinMode(RelayPin1, OUTPUT);
  pinMode(led_conn, OUTPUT);
   pinMode(chave_res, INPUT);

  // Initialise wifi connection
//  wifiConnected = connectWifi();

//---------------------------------------- Read eeprom for ssid and pass
  Serial.println("Reading EEPROM ssid");

    // Define your devices here.
    espalexa.addDevice(Device_1_Name, device1Changed);

    //acrescentar devices

    // Define your devices functions here.// FERNANDO RETIROU AQUI

  
    espalexa.begin();

    Serial.println("Ativado Espalexa");
    Serial.println("ZEN Vox Rev.0.3");

      
    digitalWrite(led_conn, HIGH);

    //teste aqui






   //wifiManager.resetSettings();
    


}

void loop()
{

  WiFiManager wifiManager;

 
  
  espalexa.loop();
  delay(100);
  Serial1.println ("1");
  //Serial.println("passei por aqui");

   if(digitalRead(chave_res) == LOW) // fernando teste aqui
  {
   // wifiManager.resetSettings();
  //  esp_wifi_set_auto_connect (false);
    esp_wifi_restore ();
   // wifiManager.startConfigPortal();
   delay(50);
   digitalWrite(led_conn, LOW);
   ESP.restart();
  }

}

//our callback functions
void device1Changed(uint8_t brightness){
Serial.println("===>agora aqui<===");
  //Control the device
  if (brightness == 255)
    {
      digitalWrite(RelayPin1, HIGH);
      Serial.println("Device1 ON");
      delay(100);
      Serial2.print("XM1");
      delay(1000);
     // Serial2.print("B10");
    }
  else if (brightness == 0)
  {
    digitalWrite(RelayPin1, LOW);
    delay(100);
    Serial2.print("XM0");
    Serial.println("Device1 OFF");
   
  }
  else
  {
    int brigh_perc = (brightness/255.)*100;
//   analogWrite(RelayPin1, brightness);
    Serial.print("Device1 Brightness: ");
    Serial.print(brigh_perc);
    Serial.println("%");
     Serial.println(brightness);
    
  }
}

//acrescentar devices aqui

// retirar o codigo a partir daqui --- Fernando


/* FUNÇÕES DO NOVO WIFI - FERNANDO
 *  
 *  
 */

// até aqui wifi
// até aqui web server

void configModeCallback (WiFiManager *myWiFiManager) {
 
 Serial.println("Entrou no modo de configuração");
 Serial.println(WiFi.softAPIP()); //imprime o IP do AP 
 Serial.println(myWiFiManager->getConfigPortalSSID()); //imprime o SSID criado da rede

 }
 
 //callback que indica que salvamos uma nova rede para se conectar (modo estação)
 
 void saveConfigCallback () {

 Serial.println("Configuração salva");
 Serial.println(WiFi.softAPIP()); //imprime o IP do AP 
 
 }
