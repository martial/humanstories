import sys
import numpy as np
import cv2

i = sys.argv[1]
url = sys.argv[2]

f_good_urls = open(sys.argv[3], 'a')


def main():
    try:
        print(f'Getting {url}...', end='', flush=True)
        cap = cv2.VideoCapture(url)
        ret, frame = cap.read()
        if frame is None:
            raise Exception()
        cv2.imwrite(f'images/{i}.jpg', frame)
        cap.release()
        print('OK')
        f_good_urls.write(f'{url}\n')
    except Exception as ex:
        print('FAILED')

main()
