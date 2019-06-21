#!/usr/bin/env python3

import json
import os

with open('result.json', 'r') as f:
    data = json.load(f)

for i, cam in enumerate(data['cameras']):
    is_online = os.path.isfile(f'images/{i}.jpg')
    if not is_online:
        cam['status'] = 'offline'

with open('result_with_offline.json', 'w') as f:
    f.write(json.dumps(data))

