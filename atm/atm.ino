#include "DHT.h"
#define DHTPIN A1
#define GasA A0

# define DHTTYPE DHT11

DHT dht(DHTPIN, DHTTYPE);

void setup() {
  Serial.begin(115200);

  dht.begin();

  delay(5000);
}

void loop() {
  int valor_ar = analogRead(GasA);
  float h = dht.readHumidity();
  float t = dht.readTemperature();
  float a = 10;

  float hic = dht.computeHeatIndex(t, h, false);

  Serial.println(String(t, 1) + "," + String(h, 1) + "," + String(valor_ar));

  delay(2000);
}