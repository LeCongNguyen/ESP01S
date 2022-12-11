#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include "zEEPROMtool.h"
#include "zConnectToWifi.h"

/* Put your SSID & Password */
const char *ssid = "NodeMCU";       // Enter SSID here
const char *password = "123456789"; // Enter Password here

/* Put IP Address details */
IPAddress local_ip(192, 168, 1, 113);
IPAddress gateway(192, 168, 1, 113);
IPAddress subnet(255, 255, 255, 0);

ESP8266WebServer server(80);

int i = 0;
int flashBtn = 0; // normal High
int flashStatus;

void setup()
{
    Serial.begin(115200);
    pinMode(flashBtn, INPUT);
    EEPROM.begin(512);
}

void loop()
{
work_mode:
    ledStatus();
    String wifiName = eepromReadString(0);
    String wifiPass = eepromReadString(wifiName.length() + 1);
    const char *WIFI = wifiName.c_str(); // chuyển String sang const char *
    const char *PASS = wifiPass.c_str();
    Serial.println();
    Serial.println("__work_mode__");
    Serial.println(WIFI);
    Serial.println(PASS);
    connectToWifi(WIFI, PASS);
    while (1)
    {
        yield();
        flashStatus = digitalRead(flashBtn);
        while (flashStatus == LOW)
        {
            flashStatus = digitalRead(flashBtn);
            delay(10);
            i++;
            if (i > 200)
            {
                i = 0;
                goto set_mode;
            }
        }
    }

set_mode:
    ledStatus();
    WiFi.softAPConfig(local_ip, gateway, subnet); // lệnh này trước rồi mới tới Wifi.softAp
    WiFi.softAP(ssid, password);
    delay(100);

    server.on("/", handle_OnConnect);
    server.on("/wifiSet", takeWifiInfo);
    server.onNotFound(handle_NotFound);

    server.begin();
    Serial.println();
    Serial.println("__set_mode__");
    Serial.println("HTTP server started");
    while (1)
    {
        server.handleClient();
        flashStatus = digitalRead(flashBtn);
        while (flashStatus == LOW)
        {
            flashStatus = digitalRead(flashBtn);
            delay(10);
            i++;
            if (i > 200)
            {
                i = 0;
                goto work_mode;
            }
        }
    }
}

void handle_OnConnect()
{
    server.send(200, "text/html", SendHTML());
}

void handle_NotFound()
{
    server.send(404, "text/plain", "Not found");
}

String SendHTML()
{
    String ptr = "<!DOCTYPE html>\n";
    ptr += "<html>\n";

    ptr += "<head>\n";
    ptr += "<title>Food Manager</title>\n";
    ptr += "<meta charset=\"utf-8\">\n";
    ptr += "<meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0\">\n";
    ptr += "</head>\n";

    ptr += "<body>\n";
    ptr += "<h1>WIFI SETTING</h1>\n";
    ptr += "<form action=\"wifiSet\" method=\"get\">\n";
    ptr += "<label for=\"wifiName\">wifiName: </label>\n";
    ptr += "<input type=\"text\" id=\"wifiName\" name=\"wifiName\"><br><br>\n";
    ptr += "<label for=\"pass\">password: </label>\n";
    ptr += "<input type=\"text\" id=\"pass\" name=\"wifiPass\"><br><br>\n";
    ptr += "<button type=\"submit\">OK</button>\n";
    ptr += "</form>\n";
    ptr += "</body>\n";

    ptr += "</html>\n";
    return ptr;
}

void takeWifiInfo()
{
    String wifiName = server.arg("wifiName");
    String wifiPass = server.arg("wifiPass");
    Serial.println();
    Serial.println("__takeWifiInfo()__");
    Serial.print("wifiName: ");
    Serial.println(wifiName);
    Serial.print("password: ");
    Serial.println(wifiPass);
    server.send(200, "text/plain", "Wifi Information is send!");

    eepromWriteString(0, wifiName);
    int x = wifiName.length() + 1;
    eepromWriteString(x, wifiPass);
}

void ledStatus()
{
    pinMode(LED_BUILTIN, OUTPUT);
    int i = 25;
    while (i > 0)
    {
        digitalWrite(LED_BUILTIN, HIGH);
        delay(100);
        digitalWrite(LED_BUILTIN, LOW);
        delay(100);
        i--;
    }
}