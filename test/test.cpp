/**
 * Author: sascha_lammers@gmx.de
 */

#include <Arduino.h>
#include "StreamString.h"
#include <MemoryFree.h>

size_t mLimit;

void setup()
{
    Serial.begin(115200);
    mLimit = freeMemory() - 128;
}

struct __freelist
{
  size_t sz;
  struct __freelist *nx;
};

extern struct __freelist *__flp;

int memoryFragmentation()
{
    struct __freelist* current;
    int total = 0;
    for (current = __flp; current; current = current->nx) {
        total++;
    }
    return total;
}

void printFreeMem()
{
    Serial.print(F("free memory = "));
    Serial.print(freeMemory());
    Serial.print(F(" fragmentation = "));
    Serial.println(memoryFragmentation());
}

StreamString ss;
const char *testStr = "0123456789";
long loopCounter = 0;

void loop()
{
    Serial.println(F("---------------------------------------------------------------------------"));
    Serial.print(F("loop = "));
    Serial.print(++loopCounter);
    Serial.print(F(" millis = "));
    Serial.print(millis());
    Serial.print(' ');
    printFreeMem();

    Serial.print(F("str = '"));
    Serial.print(testStr);
    auto len = strlen(testStr);
    Serial.print(F("' len = "));
    Serial.println(len);

    Serial.print(F("StreamString write("));
    Serial.print(len);
    Serial.print(F(") = "));
    Serial.println(ss.write(reinterpret_cast<const uint8_t *>(testStr), len));

    Serial.print(F("StreamString available() = "));
    Serial.println(ss.available());

    Serial.print(F("StreamString read() = '"));
    while(ss.available()) {
        Serial.print((char)ss.read());
    }
    Serial.println('\'');

    Serial.print(F("StreamString available() = "));
    Serial.println(ss.available());

    len = (rand() % mLimit) + 1;
    auto count = len;

    Serial.print(F("StreamString "));
    Serial.print(len);
    Serial.print(F("x write() = "));
    size_t wlen = 0;
    unsigned int seed = millis();
    srand(seed);
    while(count--) {
        wlen += ss.write((uint8_t)rand());
    }
    Serial.println(wlen);
    if (wlen != len) {
        Serial.println(F("StreamString write() failed"));
        for(;;) {
        }
    }

    Serial.print(F("StreamString available() = "));
    Serial.println(ss.available());

    printFreeMem();

    srand(seed);
    count = 0;
    while(ss.available()) {
        auto expected = (uint8_t)rand();
        int ch = ss.read();
        if (ch == -1) {
            Serial.print(F("read error at "));
            Serial.print(count);
            Serial.print(F(" expected = "));
            Serial.println(expected);
            break;
        }
        else if ((uint8_t)ch != expected) {
            Serial.print(F("read failure at "));
            Serial.print(count);
            Serial.print(F(" expected = "));
            Serial.print(expected);
            Serial.print(F(" got = "));
            Serial.println((uint8_t)ch);
            break;
        }
        count++;
    }
    printFreeMem();

    Serial.print(F("StreamString read() = "));
    Serial.print(count);
    Serial.print(F(" / "));
    Serial.println(len);
    Serial.print(F("StreamString read() "));
    auto success = (len == count);
    Serial.println(success ? F("successful") : F("failed"));
    if (!success) {
        for(;;) {
        }
    }

    printFreeMem();

    Serial.print(F("StreamString available() = "));
    Serial.println(ss.available());

    auto p = malloc(500);
    Serial.print(F("allocation 500 byte = "));
    Serial.println(p ? F("ok") : F("failure"));
    free(p);

    delay(1000);
}