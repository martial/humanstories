#!/bin/bash

cd "$(dirname "$0")"

git pull origin master && ./rpi_startup.sh
