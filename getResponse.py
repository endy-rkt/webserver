#!/usr/bin/python3

import sys
import requests

uri = "http://localhost:8080"

if (len(sys.argv) == 4):
    uri = "http://" + sys.argv[1] + ":" + sys.argv[2] + "/" + sys.argv[3] 

response = requests.get(uri)
print(response.headers)