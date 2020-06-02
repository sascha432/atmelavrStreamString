# atmelavrStreamString

ESP8266 StreamString class for atmelavr

- Fixed a bug in read() returning int instead of unsigned char
- Option to invalidate/free the string buffer if the stream is empty or when another stream is assinged (= operator)
