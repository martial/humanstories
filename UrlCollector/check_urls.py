#!/usr/bin/env python3

import magic
import requests
import sys
import os
import subprocess

with open(sys.argv[1], 'r') as f:
    input_urls = [x.strip() for x in f.readlines() if x.strip()]

for i in range(len(input_urls)):
    url = input_urls[i]
    subprocess.call(['timeout', '--signal=9', '4', 'python3', 'check_url.py', str(i), url, sys.argv[2]])
    print('')
