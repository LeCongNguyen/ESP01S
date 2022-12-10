/*
Serial.print("AB");

char data[65, 66, '\0']; //65, 66 là mã ASCII của ký tự A, B
Serial.print(data) 
*/

#include <EEPROM.h>

// void setup()
// {
//     Serial.begin(115200);
//     EEPROM.begin(512);
//     eepromWriteString(5, "C0");
//     delay(1000);
//     EEPROMreadAll();
//     // Serial.println(eepromReadString(5));
// }

void EEPROMreadAll()
{
    int address = 0;
    byte value;
    int i;
    for (i = 0; i < 512; i++)
    {
        value = EEPROM.read(address);
        Serial.print(address);
        Serial.print("\t");
        Serial.print(value);
        Serial.println();
        address = address + 1;
    }
}

void eepromWriteString(char add, String data)
{
    int _size = data.length();
    int i;
    for (i = 0; i < _size; i++)
    {
        EEPROM.write(add + i, data[i]);
    }
    EEPROM.write(add + _size, '\0'); // Add termination null character for String Data
    EEPROM.commit();
}

String eepromReadString(char add)
{
    int i;
    char data[100]; // Max 100 Bytes
    int len = 0;
    unsigned char k;
    k = EEPROM.read(add);
    while (k != '\0' && len < 500) // Read until null character
    {
        k = EEPROM.read(add + len);
        data[len] = k;
        len++;
    }
    data[len] = '\0';
    return String(data);
}
