#include <ESP8266WiFi.h>
#include <WiFiClientSecure.h>
#include <DHT.h>
#include <TelegramBot.h>


//Configurando a rede Wi-Fi
const char* ssid = "******";     //Substituir os * com a sua rede
const char* password = "*****";  //Substituir os * com a senha da rede

//Configurando Telegram BOT
WiFiClientSecure client;
const char* BotToken = "640620942:AAGfmC9MRD5ca13LTBsIQGARW8nLiBn9-_Q"; //Token do bot criado para dar o aviso
TelegramBot bot(BotToken, client);

#define DHTTYPE DHT22   //Definindo o sensor
#define DHTPIN 2        //Pino do DHT, D4
#define BUZZER 4        //Pino de entrada do buzzer, D2
#define BUTTON 14       //Pino de entrada do botão, D5


DHT dht(DHTPIN, DHTTYPE, 15); //Configurando o sensor

int t;

void semRede(){
  byte valor = digitalRead(BUTTON); //Leitura do botão 
  while ( valor != 1){
    valor = digitalRead(BUTTON);
  }
  t = dht.readTemperature();
  if (t >65){
    digitalWrite(BUZZER, HIGH);
    Serial.println("BUZZER LIGADO. PRONTO. SEM WIF");
    while ( valor != 0){
    valor = digitalRead(BUTTON);
    }
    if (valor == HIGH){  
      digitalWrite(BUZZER, LOW);
    }
  }
}

void avaliandoMensagem(message men)
{
  bot.sendMessage(men.chat_id, "Só um segundo...");
  Serial.print("Mensagem recebida = ");
  Serial.println(men.text);
 
  String user = men.sender;        // Recebe o nome do usuario que enviou a mensagem
  if (user == ""){ user = "Usuario";}
 
  if (men.text.equals("/on"))                              
  {
     bot.sendMessage(men.chat_id, "Sensor iniciado.\n Avisaremos quando seu café ficar pronto.");  
     t = dht.readTemperature();  //Lendo a temperatura em graus Celsius
     if (t >65){
       Serial.println("PRONTO. COM WIFI");
       bot.sendMessage(men.chat_id, "Seu café está pronto!!");
       return;    
    }
  }
  if (men.text.equals("/temp"))        // Envia temperatua atual                         
   {
     int tmp = dht.readTemperature();
     String graus = String(tmp);
     bot.sendMessage(men.chat_id,graus+"ºC é a temperatura atual");
     return; 
   }
     
   if (men.text.equals("/help")||men.text.equals("/start"))    // Envia mensagem de instruções                              
   {
     String welcome = "Bem-vindo ao SmartCoffee Bot, " + user + "!!\n";
     welcome += "Esse Bot te avisa quando seu café está pronto.\n";
     welcome += "/on : Inicia o sensor de temperatura.\n";
     welcome += "/temp : Para saber a temperatura atual.\n";
     welcome += "/help : Mostra esta mensagem.\n";
     bot.sendMessage(men.chat_id, welcome);
     return;
    }
    else{
      bot.sendMessage(men.chat_id, "Comando inválido");
      return;
    }
}

void setup() {

  Serial.begin(9600);
  dht.begin(); // Iniciando o DHT 

  WiFi.mode(WIFI_STA);
  delay(10);

  //Configurando buzzer e botão
  pinMode(BUZZER, OUTPUT);
  pinMode(BUTTON, INPUT);
  delay(8000);  // tempo para eu abrir o monitor serial
  
  WiFi.begin(ssid, password); // Conectando na rede wifi
  int wificount = 0; // Contador para limitar as tentativas de conexão
  while(wificount <30){
    
    if (WiFi.status() != WL_CONNECTED) {
      Serial.println("NAO conectado");
      delay(500);
    }
    wificount ++;
    if (WiFi.status() == WL_CONNECTED){
      Serial.println("Conectado!!");
      wificount = 30; 
    }
  }
  
  bot.begin(); // Iniciando o TelegramBot
  
}

void loop() {

  Serial.println("Entrei no LOOP");
  delay(4000);
  message m = bot.getUpdates(); // Lendo novas mensagens
 
  if (WiFi.status() != WL_CONNECTED){ //Chamando a função que funciona offline
    void semRede();  
  }
  else if (WiFi.status() == WL_CONNECTED){
    bot.sendMessage(m.chat_id, "Conectado!");
    message m = bot.getUpdates();      // Lendo novas mensagens
    bot.sendMessage(m.chat_id, "Comando escolhido: "+ m.text);
    avaliandoMensagem(m);     // Envia mensagem recebida para função 
  }
  
}
