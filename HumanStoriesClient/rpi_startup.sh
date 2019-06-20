#!/bin/bash

echo Maintenance mode > /dev/tty1
ifconfig wlan0 > /dev/tty1
echo Got latest version from git, but didn\'t compile > /dev/tty1

make -j 2 && make RunRelease
