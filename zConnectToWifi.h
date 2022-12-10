#include <ESP8266WiFi.h>

// tRetry tính theo phút
void connectToWifi(const char *ssid, const char *password, unsigned int tRetry)
{
    int i = 0;
    pinMode(LED_BUILTIN, OUTPUT);
    WiFi.begin(ssid, password);
    Serial.print("Connecting...");
    delay(250);
    while (WiFi.status() != WL_CONNECTED)
    {
        Serial.print(".");
        digitalWrite(LED_BUILTIN, LOW);
        delay(250);
        digitalWrite(LED_BUILTIN, HIGH);
        delay(250);
        i++;
        if (i > 100)
        {
            ESP.deepSleep(tRetry * 60000000);
        }
    }
    digitalWrite(LED_BUILTIN, LOW);
    Serial.println("");
    Serial.print("Connected to WiFi network with IP Address: ");
    Serial.println(WiFi.localIP());
}