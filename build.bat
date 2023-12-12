@ECHO ON

arduino-cli compile -b arduino:avr:uno --warnings all --output-dir ./debug --no-color ^
    && avrdude -v -patmega328p -carduino -PCOM4 -b115200 -D -Uflash:w:./debug/labvbackend.ino.hex:i 

