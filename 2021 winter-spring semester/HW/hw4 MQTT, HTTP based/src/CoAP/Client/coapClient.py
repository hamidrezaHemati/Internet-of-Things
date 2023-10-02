from coapthon.client.helperclient import HelperClient
import json
import time
import random


def __main__():
    host = "192.168.1.7"
    port = 5683

    client = HelperClient(server=(host, port))
    while True:
        response = client.get('tagID')

        if (response is not None) and (response.payload is not None):
            with open('clientsDataBase.json','r') as json_file:
                clientsDatas = json.load(json_file)
            
            if not(response.payload in clientsDatas):
                clientsDatas[response.payload] = {}
                clientsDatas[response.payload]['level'] = random.randint(1, 5)
                clientsDatas[response.payload]['temperature'] = random.randint(1, 100)
                with open('clientsDataBase.json','w') as json_file:
                    json.dump(clientsDatas,json_file)
            
            level = clientsDatas[response.payload]['level']
            temp = clientsDatas[response.payload]['temperature']
            
            client.put('bath', f'{level} {temp}')

        time.sleep(1)

__main__()
