#include<ESP8266WiFi.h>
#include<WiFiClient.h>
#include<ESP8266WebServer.h>

const char* ssid = "Universitas Mulawarman";
const char* password = "";

ESP8266WebServer server(80);

const int BUZZER = D8;
const int BUTTON_BUZZER = D7;
const int BUTTON_BUZZ_PLUS = D5;
const int BUTTON_BUZZ_MIN = D6;

int ON_OFF_BUZZ ;
int PLUS_BUZZ = 0;
int MIN_BUZZ = 0;
int BUZZ_SUARA = 100;


const int LED = D4;
const int BUTTON_LED = D3;
const int BUTTON_LED_PLUS = D1;
const int BUTTON_LED_MIN = D2;

int ON_OFF_LED ;
int PLUS_LED = 0;
int MIN_LED = 0;
int LED_KECERAHAN = 0;


void setup() {
  Serial.begin(9600);    
  // Initialize pins
  pinMode(BUTTON_BUZZER, INPUT_PULLUP);
  pinMode(BUTTON_BUZZ_PLUS, INPUT_PULLUP);
  pinMode(BUTTON_BUZZ_MIN, INPUT_PULLUP);


  pinMode(BUTTON_LED, INPUT_PULLUP);
  pinMode(BUTTON_LED_PLUS, INPUT_PULLUP);
  pinMode(BUTTON_LED_MIN, INPUT_PULLUP);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED){
    delay(1000);
    Serial.println("Sedang Menghubungkan Ke WiFi...");
  }
  Serial.println("Terhubung Ke WiFi");
  Serial.println(WiFi.localIP());

  server.on("/", mainMenu);
  server.on("/kecerahan",statusLed);
  server.on("/frekuensi",statusBuzzer);
  server.begin();
}

void loop() {
  server.handleClient();
  delay(150);
  // Check button state
  ON_OFF_BUZZ = digitalRead(BUTTON_BUZZER);
  PLUS_BUZZ = digitalRead(BUTTON_BUZZ_PLUS);
  MIN_BUZZ = digitalRead(BUTTON_BUZZ_MIN);

  ON_OFF_LED = digitalRead(BUTTON_LED);
  PLUS_LED = digitalRead(BUTTON_LED_PLUS);
  MIN_LED = digitalRead(BUTTON_LED_MIN);

  


  // If button is pressed, turn on LED and buzzer
  if (ON_OFF_LED == LOW ) {
    if (PLUS_LED == LOW ) {
      plus_kecerahan();
      analogWrite(LED, LED_KECERAHAN);
      delay(100);
  }if (MIN_LED == LOW ) {
      min_kecerahan();
      analogWrite(LED, LED_KECERAHAN);
      delay(100);
  }
  }else{
    LED_KECERAHAN = 0;
    analogWrite(LED, LED_KECERAHAN);
  }
  

  //BUZZER

  if(ON_OFF_BUZZ == LOW ){
    if(PLUS_BUZZ == LOW ){
      plus_buzzer();
      tone(BUZZER,BUZZ_SUARA);
      delay(80);
    }if(MIN_BUZZ == LOW  ){
      min_buzzer();
      tone(BUZZER,BUZZ_SUARA);
      delay(80);
    }
  }
  else{
    BUZZ_SUARA = 100;
    noTone(BUZZER);
  }

  
}

void plus_kecerahan() {
  if (LED_KECERAHAN <= 250) {
    LED_KECERAHAN += 5;
  }
}
void min_kecerahan() {
  if (LED_KECERAHAN >= 0){
    LED_KECERAHAN -= 5;
  }
}

void plus_buzzer() {
  if (BUZZ_SUARA <= 2500) {
    BUZZ_SUARA += 5;
  }
}

void min_buzzer() {
  if (BUZZ_SUARA >= 100) {
    BUZZ_SUARA -= 5;
  }
}

void mainMenu() {
  server.send(200, "text/html", "<html><body><h1>Monitoring Kecerahan LED dan Frekuensi Buzzer </h1><p><a href=\"/kecerahan\">Lihat kecerahan</a></p><p><a href=\"/frekuensi\">Lihat Frekuensi</a></p></body></html>"); // Menampilkan halaman utama
}

//Monitoring Kecerahan
void statusLed() {
  String content = "<html><body><h1>Kecerahan Led</h1><p>" + String(LED_KECERAHAN) + " </p><p><a href=\"/\">Kembali ke halaman utama</a></p></body></html>"; 
  server.send(200, "text/html", content);
}

void statusBuzzer() {
 // Monitoring Frekuensi suara
  String content = "<html><body><h1>Frekuensi Buzzer</h1><p>" + String(BUZZ_SUARA) + " Hz</p><p><a href=\"/\">Kembali ke halaman utama</a></p></body></html>"; 
  server.send(200, "text/html", content);
}
