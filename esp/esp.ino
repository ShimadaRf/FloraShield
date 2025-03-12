#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <WiFiClientSecure.h>

const char* ssid = "FloraShield";
const char* password = "Filho321.";
const String FIREBASE_HOST = "https://florashield-5b753-default-rtdb.firebaseio.com/";
const String FIREBASE_AUTH = "nqXj2z8leVoM7N7JSGPNF2d6EBpvHDWfFEuzFGq0";

String incomingString;

void setup() {
  pinMode(12, OUTPUT);  // Inicializa o pino 12 como saída
  Serial.begin(115200);  // Comunicação serial com o Arduino
  WiFi.begin(ssid, password);  // Conecta ao WiFi
  
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("WiFi connected.");

  delay(5000);
}

void loop() {
  if (Serial.available() > 0) {
    // Lê a string recebida do Arduino
    incomingString = Serial.readStringUntil('\n');
    
    // Separa os números usando a vírgula como delimitador
    int firstCommaIndex = incomingString.indexOf(',');
    int secondCommaIndex = incomingString.indexOf(',', firstCommaIndex + 1);
    
    if (firstCommaIndex != -1 && secondCommaIndex != -1) {
      String number1String = incomingString.substring(0, firstCommaIndex);
      String number2String = incomingString.substring(firstCommaIndex + 1, secondCommaIndex);
      String number3String = incomingString.substring(secondCommaIndex + 1);
      
      float number1 = number1String.toFloat();
      float number2 = number2String.toFloat();
      float number3 = number3String.toFloat();

      if (WiFi.status() == WL_CONNECTED) {
        WiFiClientSecure client;
        client.setInsecure();  // Desabilita a verificação de certificado SSL
        
        HTTPClient http;
        
        String urlTemp = FIREBASE_HOST + "FloraShield/FloraShield/Temp.json?auth=" + FIREBASE_AUTH;
        http.begin(client, urlTemp);
        http.addHeader("Content-Type", "application/json");
        String jsonTempData = "{\"Temp\": " + String(number1) + "}";
        int httpResponseCode = http.PUT(jsonTempData);
        if (httpResponseCode > 0) {
          Serial.print("HTTP Response code (Temp): ");
          Serial.println(httpResponseCode);
        } else {
          Serial.print("Error code (Temp): ");
          Serial.println(httpResponseCode);
        }
        http.end();
        
        String urlHumi = FIREBASE_HOST + "FloraShield/FloraShield/Humi.json?auth=" + FIREBASE_AUTH;
        http.begin(client, urlHumi);
        http.addHeader("Content-Type", "application/json");
        String jsonHumiData = "{\"Humi\": " + String(number2) + "}";
        httpResponseCode = http.PUT(jsonHumiData);
        if (httpResponseCode > 0) {
          Serial.print("HTTP Response code (Humi): ");
          Serial.println(httpResponseCode);
        } else {
          Serial.print("Error code (Humi): ");
          Serial.println(httpResponseCode);
        }
        http.end();

        String urlAr = FIREBASE_HOST + "FloraShield/FloraShield/Ar.json?auth=" + FIREBASE_AUTH;
        http.begin(client, urlAr);
        http.addHeader("Content-Type", "application/json");
        String jsonArData = "{\"Ar\": " + String(number3) + "}";
        httpResponseCode = http.PUT(jsonArData);
        if (httpResponseCode > 0) {
          Serial.print("HTTP Response code (Ar): ");
          Serial.println(httpResponseCode);
        } else {
          Serial.print("Error code (Ar): ");
          Serial.println(httpResponseCode);
        }
        http.end();
      }
    }
  }
  
  delay(2000); // Espera antes de verificar novamente
}
