#include <WiFi.h>
#include <HTTPClient.h>
#include <DHT.h>

#define DHTPIN 4     
#define DHTTYPE DHT11  

DHT dht(DHTPIN, DHTTYPE);

const char* ssid = "INFINITUM8SH5_2.4";
const char* password = "";
const char* serverAddress = "192.168.1.84";
const int serverPort = 80; 

void setup() {
    Serial.begin(115200);
    dht.begin();
    WiFi.begin(ssid, password);
    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.print(".");
    }
    Serial.println("Conectado a WiFi");
}

void loop() {
    if (WiFi.status() == WL_CONNECTED) {
        HTTPClient http;

        http.begin("http://" + String(serverAddress) + "/recibir.php");

        http.addHeader("Content-Type", "application/x-www-form-urlencoded");

        float h = dht.readHumidity();
        float t = dht.readTemperature();

        if (isnan(h) || isnan(t)) {
            Serial.println("Failed to read from DHT sensor!");
            return;
        }

        int httpResponseCode = http.POST("sensor=temperatura&valor=" + String(t));

        if (httpResponseCode > 0) {
            Serial.print("HTTP Response code: ");
            Serial.println(httpResponseCode);

            String response = http.getString();
            Serial.print("HTTP Response: ");
            Serial.println(response);
        } else {
            Serial.print("Error en la solicitud: ");
            Serial.println(httpResponseCode);
        }

        http.end();
    }

    delay(10000); 
}
