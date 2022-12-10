#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include "zEEPROMtool.h"

/* Put your SSID & Password */
const char *ssid = "NodeMCU";       // Enter SSID here
const char *password = "123456789"; // Enter Password here

/* Put IP Address details */
IPAddress local_ip(192, 168, 1, 1);
IPAddress gateway(192, 168, 1, 1);
IPAddress subnet(255, 255, 255, 0);

ESP8266WebServer server(80);

void setup()
{
    Serial.begin(115200);
    pinMode(0, INPUT);
    pinMode(LED_BUILTIN, OUTPUT);

    WiFi.softAPConfig(local_ip, gateway, subnet); // lệnh này trước rồi mới tới Wifi.softAp
    WiFi.softAP(ssid, password);
    delay(100);

    server.on("/", handle_OnConnect);
    server.on("/wifiSet", takeWifiInfo);
    server.onNotFound(handle_NotFound);

    server.begin();
    Serial.println();
    Serial.println("__setup()__");
    Serial.println("HTTP server started");

    EEPROM.begin(512);
}

void loop()
{
    server.handleClient();
    int flashBtn = digitalRead(0);
    if (flashBtn == LOW)
    {
        digitalWrite(LED_BUILTIN, LOW);
    }
    else
    {
        digitalWrite(LED_BUILTIN, HIGH);
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
    ptr += "<meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0, user-scalable=no\">\n";
    ptr += "</head>\n";

    ptr += "<body>\n";
    ptr += "<h1>WIFI SETTING</h1>\n";
    ptr += "<form action=\"wifiSet\" method=\"get\">\n";
    ptr += "<label for=\"wifiName\">wifiName: </label>\n";
    ptr += "<input type=\"text\" id=\"wifiName\" name=\"wifiName\"><br><br>\n";
    ptr += "<label for=\"pass\">password: </label>\n";
    ptr += "<input type=\"text\" id=\"pass\" name=\"wifiPass\"><br><br>\n";
    ptr += "<input type=\"submit\" value=\"OK\">\n";
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
    server.send(200, "text/plain", "Wifi is connected!");

    eepromWriteString(0, wifiName);
    int x = wifiName.length() + 1;
    eepromWriteString(x, wifiPass);
    Serial.println(eepromReadString(0));
    Serial.println(eepromReadString(x));
    EEPROMreadAll();
}