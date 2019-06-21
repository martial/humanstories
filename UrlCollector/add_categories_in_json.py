#!/usr/bin/env python3

import json
import os

with open('result_with_offline.json', 'r') as f:
    data = json.load(f)


BASE = 'categorised_images'
cat2id_set = {}

cat_names = [x for x in os.listdir(BASE) if '.' not in x]

def extract_id(fname):
    for i, ch in enumerate(fname):
        if ch not in '0123456789':
            return int(fname[:i])
    raise Exception()


for cat in cat_names:
    files = [x for x in os.listdir(f'{BASE}/{cat}') if '.jpg' in x.lower()]
    ids = [extract_id(x) for x in files]
    ids = [x for x in ids if data['cameras'][x]['status'] != 'offline']
    cat2id_set[cat] = set(ids)


output = []
for cat, ids in cat2id_set.items():
    output.append({'category': cat, 'ids': sorted(list(ids))})

#for i, cam in enumerate(data['cameras']):
#    cam['categories'] = []
#    for cat in cat_names:
#        if i in cat2id_set[cat]:
#            cam['categories'].append(cat)


with open('categories.json', 'w') as f:
    f.write(json.dumps(output))

