#include <WiFi.h>
#include <FirebaseESP32.h>

#define LED_BUILTIN 2 //#
#define SENSOR 27 //#
#define FIREBASE_HOST "esp32-firebase-a75b1.firebaseio.com" //Do not include https:// in FIREBASE_HOST
#define FIREBASE_AUTH "D5zIzQjT5sHnsrMIh264CzzBqKXNHgMWUAk7OCmr"
#define WIFI_SSID "CyberGuns" //#
#define WIFI_PASSWORD "12345678"  //#

float vazao; //Variável para armazenar o valor em L/min
float media = 0; //Variável para fazer a média
int contaPulso; //Variável para a quantidade de pulsos
int i = 0; //Variável para segundos
int Min = 00; //Variável para minutos
float Litros = 0; //Variável para Quantidade de agua
float MiliLitros = 0; //Variavel para Conversão

FirebaseData firebaseData;  //Define FirebaseESP32 data object

// Root Path
String path = "/SmartIrrigationIoT";

void printResult(FirebaseData &data);

void setup()
{

  Serial.begin(115200);

 pinMode(LED_BUILTIN, OUTPUT);  //#
 pinMode(SENSOR, INPUT_PULLUP); //#

  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  Serial.print("Connecting to Wi-Fi");
  while (WiFi.status() != WL_CONNECTED)
  {
    Serial.print(".");
    delay(300);
  }
  Serial.println();
  Serial.print("Connected with IP: ");
  //Serial.println(WiFi.localIP());
  //Serial.println();

  Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH);
  Firebase.reconnectWiFi(true);

  //Set database read timeout to 1 minute (max 15 minutes)
  Firebase.setReadTimeout(firebaseData, 1000 * 60);
  //tiny, small, medium, large and unlimited.
  //Size and its write timeout e.g. tiny (1s), small (10s), medium (30s) and large (60s).
  Firebase.setwriteSizeLimit(firebaseData, "tiny");
  /*
  This option allows get and delete functions (PUT and DELETE HTTP requests) works for device connected behind the
  Firewall that allows only GET and POST requests.
  
  Firebase.enableClassicRequest(firebaseData, true);
  */
  String path = "/Test";
}

void loop() {
  void initWifi();

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
 Serial.print("Media por minuto = "); //Imprime a frase Media por minuto =
 Serial.print(media); //Imprime o valor da media
 Serial.println(" L/min - "); //Imprime L/min

   Firebase.setInt(firebaseData, path + "/Litros", media );
 
 media = 0; //Zera a variável media para uma nova contagem
 i = 0; //Zera a variável i para uma nova contagem
 } 
}

void incpulso ()  {
 contaPulso++; //Incrementa a variável de pulsos
}

void initWifi(){
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  Serial.print("Connecting to Wi-Fi");
  while (WiFi.status() != WL_CONNECTED)  {
    Serial.print(".");
    delay(300);
  }

  Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH);
  Firebase.reconnectWiFi(true);
  
  //Set database read timeout to 1 minute (max 15 minutes)
  Firebase.setReadTimeout(firebaseData, 1000 * 60);
  //tiny, small, medium, large and unlimited.
  //Size and its write timeout e.g. tiny (1s), small (10s), medium (30s) and large (60s).
  Firebase.setwriteSizeLimit(firebaseData, "tiny");
}
