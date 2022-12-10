#include <ESP8266WiFi.h>
#include <WiFiClient.h>

void ledBlink(unsigned int timeDelay1, unsigned int timeDelay2)
{
    digitalWrite(LED_BUILTIN, HIGH);
    delay(timeDelay1);
    digitalWrite(LED_BUILTIN, LOW);
    delay(timeDelay2);
}

// data có dạng name1=value1&name2=value2&...
// port thường là 80
String HttpsGETRequest(String host, String path, unsigned int port, String data)
{
    int i = 0;
    String response;
    pinMode(LED_BUILTIN, OUTPUT);
    while (true)
    {
        if (WiFi.status() == WL_CONNECTED)
        {
            WiFiClient client;
            if (!client.connect(host, port))
            {
                Serial.println("Connect to server fail!");
                ledBlink(250, 1000);
                i++;
                if (i > 100)
                {
                    Serial.println("Waiting time is over!");
                    break;
                }
                else
                    continue;
            }
            // First request to get Cookie
            client.print(String("GET ") + path + "?" + data + " HTTP/1.1\r\n" +
                         "Host: " + host + "\r\n" +
                         "Connection: keep-alive\r\n\r\n");
            i = 0;
            while (client.available() == 0)
            {
                ledBlink(50, 50);
                i++;
                // Thoát nếu sau 2 phút mà vẫn chưa có phản hồi
                if (i > 1200)
                    break;
            }
            while (client.available())
                response = client.readString();
            int s = response.indexOf("document.cookie");
            int e = response.indexOf(" ;", s);
            String cookie = response.substring(s + 17, e);
            // Second request to get data
            client.print(String("GET ") + path + "?" + data + " HTTP/1.1\r\n" +
                         "Host: " + host + "\r\n" +
                         "Cookie: " + cookie + "\r\n" +
                         "Connection: keep-alive\r\n\r\n");
            i = 0;
            while (client.available() == 0)
            {
                ledBlink(50, 50);
                i++;
                // Thoát nếu sau 2 phút mà vẫn chưa có phản hồi
                if (i > 1200)
                    break;
            }
            while (client.available())
                response = client.readString();
        }
        else
        {
            Serial.println("WiFi Disconnected!");
            ledBlink(1000, 1000);
            i++;
            if (i > 100)
                break;
            else
                continue;
        }
        return response;
    }
}
