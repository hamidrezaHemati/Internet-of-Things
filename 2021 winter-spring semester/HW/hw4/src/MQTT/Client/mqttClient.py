import paho.mqtt.client as mqtt
import json
import random
import os


def on_message(client, userdata, message):
    print("message received " ,str(message.payload.decode("utf-8")))
    rfid = str(message.payload.decode("utf-8"))


    with open('clientsDataBase.json','r') as json_file:
        clientsDatas = json.load(json_file)


    if not(rfid in clientsDatas):
        clientsDatas[rfid] = {}
        clientsDatas[rfid]['level'] = random.randint(1,5)
        clientsDatas[rfid]['temperature'] = random.randint(1,100)
        with open('clientsDataBase.json','w') as json_file:
            json.dump(clientsDatas,json_file)
    
    level = clientsDatas[rfid]['level']
    temp = clientsDatas[rfid]['temperature']
    client.publish('house/bath', f"{level} {temp}")


def on_connect(client, userdata, flags, rc):
    if rc == 0:
        print("Connected successfully")
    else:
        print("Connect returned result code: " + str(rc))


def __main__():
    client = mqtt.Client()
    client.on_connect = on_connect
    client.on_message = on_message
    
    # connect to HiveMQ Cloud on port 1883
    client.connect("192.168.1.7", 1883)

    client.subscribe("house/hardwareBath")

    client.loop_forever()


__main__()