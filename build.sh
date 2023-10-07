#!/usr/bin/bash

set -ex
arduino-cli compile -b arduino:avr:uno --warnings all --output-dir ./debug --no-color
# arduino-cli upload -b arduino:avr:uno -t -i ./debug/labvbackend.ino.hex -p /dev/ttyACM0 --verbose --no-color

