#include <ESP8266WiFi.h>

// tRetry tính theo phút
void connectToWifi(const char *ssid, const char *password)
{
    int i = 0;
    int flashBtn = 0; // normal High
    int flashStatus;
    pinMode(flashBtn, INPUT);

    WiFi.begin(ssid, password);
    Serial.print("Connecting...");
    delay(250);
    while (WiFi.status() != WL_CONNECTED)
    {
        Serial.print(".");
        delay(250);
        flashStatus = digitalRead(flashBtn);
        while (flashStatus == LOW)
        {
            flashStatus = digitalRead(flashBtn);
            delay(10);
            i++;
            if (i > 200)
            {
                goto xxx;
            }
        }
    }
    Serial.println("");
    Serial.println("__connectToWifi(ssid, password)__");
    Serial.print("Connected to WiFi network with IP Address: ");
    Serial.println(WiFi.localIP());
    xxx:
    yield();
}