#include <WiFi.h>
#include <PubSubClient.h>
#include <FloatToString.h>

#define LED_BUILTIN 2
#define SENSOR  27

const char* ssid = "CyberGuns"; 
const char* password =  "12345678";
const char* mqttServer = "tailor.cloudmqtt.com";
const int mqttPort = 12519; 
const char* mqttUser = "qzzuvacg"; 
const char* mqttPassword = "hbhVYxRLlRBc";

float vazao; //Variável para armazenar o valor em L/min
float media = 0; //Variável para fazer a média
int contaPulso; //Variável para a quantidade de pulsos
int i = 0; //Variável para segundos
int Min = 00; //Variável para minutos
float Litros = 0; //Variável para Quantidade de agua
float MiliLitros = 0; //Variavel para Conversão

WiFiClient espClient;
PubSubClient client(espClient);

void mqtt_callback(char* topic, byte* dados_tcp, unsigned int length);

void setup()  {
 Serial.begin(115200);
 pinMode(LED_BUILTIN, OUTPUT);
 pinMode(SENSOR, INPUT_PULLUP);
 attachInterrupt(digitalPinToInterrupt(SENSOR), incpulso, FALLING);

WiFi.begin(ssid, password);
 
  while (WiFi.status() != WL_CONNECTED) {   
     delay(100);
    Serial.println("Conectando a WiFi..");
  }
  Serial.println("Conectado!"); 
  client.setServer(mqttServer, mqttPort); 
  
  while (!client.connected()) {
    Serial.println("Conectando ao servidor MQTT...");
      if (client.connect("Projeto", mqttUser, mqttPassword )) {
      Serial.println("Conectado ao servidor MQTT!");  
      } else {
        Serial.print("Falha ao conectar ");
        Serial.print(client.state());
        delay(2000);
        }
  }
 
  client.publish("Status ","Reiniciado!");
  client.publish("Placa","Em funcionamento!");
  client.subscribe("Temperatura"); 
}

void re_connected(){

  if (WiFi.status() != WL_CONNECTED) {
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {   
    delay(100);
    Serial.println("Conectando a WiFi..");
  }
  Serial.println("Conectado!"); 
  client.setServer(mqttServer, mqttPort); 
  
  while (!client.connected()) {
    Serial.println("Conectando ao servidor MQTT...");
      if (client.connect("Projeto", mqttUser, mqttPassword )) {
        Serial.println("Conectado ao servidor MQTT!");  
      } else {
        Serial.print("Falha ao conectar ");
        Serial.print(client.state());
        delay(2000);
      }
  }
  client.publish("Status ","Reiniciado!");
  client.publish("Placa","Em funcionamento!");
  client.subscribe("Temperatura"); 
  }
}
void loop ()  {
   void re_connected();
   client.loop();
   
 contaPulso = 0;//Zera a variável
 sei(); //Habilita interrupção
 delay (1000); //Aguarda 1 segundo
 cli(); //Desabilita interrupção
 
 vazao = contaPulso / 5.5; //Converte para L/min
 media = media + vazao; //Soma a vazão para o calculo da media
 i++;
 MiliLitros = vazao / 60;
 Litros = Litros + MiliLitros;
 
 // Neste conjunto de linhas fizemos a média das leituras obtidas a cada 1 minuto
 if (i == 60)  {
  Min++;
 
 if (Min >= 60)  {
  Min = 0;
 }
 media = media / 60; //faz a média
 Serial.print("nMedia por minuto = "); //Imprime a frase Media por minuto =
 Serial.print(media); //Imprime o valor da media
 Serial.println(" L/min - "); //Imprime L/min
 
 String msg;
 msg = media;
 // if (msg != "-999,00"){
    msg = msg + " litros";
    char message[58];
    msg.toCharArray(message, 58);
    client.publish("Temperatura", message);
  
 
 media = 0; //Zera a variável media para uma nova contagem
 i = 0; //Zera a variável i para uma nova contagem
 } 
}
 
void incpulso ()  {
 contaPulso++; //Incrementa a variável de pulsos
}
