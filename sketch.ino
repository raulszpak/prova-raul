#include <WiFi.h>
#include <HTTPClient.h>

// Definições dos pinos
#define led_amarelo 9
#define led_verde 2
#define led_vermelho 40
#define ldrPin 4
#define botaopush 18

const int threshold = 600; // Limite para luz clara/escura
unsigned long lastDebounceTime = 0;
const int debounceDelay = 50; // 50ms para debounce

int buttonState = 0;
int lastButtonState = 0;
int buttonPressCount = 0;

void setup() {
  Serial.begin(9600);

  // Configuração dos LEDs como saída
  pinMode(led_amarelo, OUTPUT);
  pinMode(led_verde, OUTPUT);
  pinMode(led_vermelho, OUTPUT);
  digitalWrite(led_amarelo, LOW);
  digitalWrite(led_verde, LOW);
  digitalWrite(led_vermelho, LOW);

  // Configuração do botão e do LDR
  pinMode(botaopush, INPUT);
  pinMode(ldrPin, INPUT);

  // Conexão WiFi
  WiFi.begin("Wokwi-GUEST", "");
  while (WiFi.status() != WL_CONNECTED) {
    delay(100);
    Serial.print(".");
  }
  Serial.println("\nWiFi conectado!");
}

void loop() {
  int ldrStatus = analogRead(ldrPin);

  // Modo noturno
  if (ldrStatus <= threshold) {
    digitalWrite(led_amarelo, HIGH);
    delay(500);
    digitalWrite(led_amarelo, LOW);
    delay(500);
  } 
  // Modo convencional
  else {
    // Estado do semáforo
    digitalWrite(led_verde, HIGH);
    delay(3000);
    digitalWrite(led_verde, LOW);
    digitalWrite(led_amarelo, HIGH);
    delay(2000);
    digitalWrite(led_amarelo, LOW);
    digitalWrite(led_vermelho, HIGH);
    delay(5000);
    digitalWrite(led_vermelho, LOW);
  }

  // Leitura do botão com debounce
  int reading = digitalRead(botaopush);
  if (reading != lastButtonState) {
    lastDebounceTime = millis();
  }
  if ((millis() - lastDebounceTime) > debounceDelay) {
    if (reading != buttonState) {
      buttonState = reading;
      if (buttonState == HIGH) {
        buttonPressCount++;
        Serial.println("Botão pressionado!");
        if (buttonPressCount == 3) {
          sendHTTPRequest();
          buttonPressCount = 0;
        }
      }
    }
  }
  lastButtonState = reading;

  // Abrir semáforo se botão pressionado no vermelho
  if (digitalRead(led_vermelho) == HIGH && buttonState == HIGH) {
    delay(1000);
    digitalWrite(led_vermelho, LOW);
    digitalWrite(led_verde, HIGH);
    delay(3000);
    digitalWrite(led_verde, LOW);
  }
}

void sendHTTPRequest() {
  if (WiFi.status() == WL_CONNECTED) {
    HTTPClient http;
    http.begin("http://www.google.com.br/");
    int httpResponseCode = http.GET();
    if (httpResponseCode > 0) {
      Serial.print("HTTP Response code: ");
      Serial.println(httpResponseCode);
    } else {
      Serial.print("Error code: ");
      Serial.println(httpResponseCode);
    }
    http.end();
  } else {
    Serial.println("WiFi desconectado.");
  }
}
