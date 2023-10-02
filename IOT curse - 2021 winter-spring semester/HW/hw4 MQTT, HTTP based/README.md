# MQTT-CoAPP-HTTP

## Table of Contents
- Project Decsription
- Deployment Instructions

## Project Decsription
The ultimate goal of this exercise is to implement a part of a smart home using the mentioned protocols. In the implementation of this smart bath, personal information for each individual (including desired water level and temperature) is customized and stored in personal databases. This customization not only serves the convenience of the residents but also results in water and time savings for these individuals.
To implement this scenario, you will be using two protocols, MQTT and CoAP. These protocols will facilitate communication and data exchange between the different nodes and components in your smart home system. MQTT (Message Queuing Telemetry Transport) and CoAP (Constrained Application Protocol) are both lightweight and suitable for IoT (Internet of Things) applications.

By using both MQTT and CoAP, you can create a robust and efficient communication system within your smart home scenario, allowing the components to interact seamlessly and provide the desired functionality.

## Deployment Instructions
1. First, the person in question presents their card or switch to Node 1. (In total, there are two individuals in this scenario.)
2. Then, the ID of this person is sent to the broker and forwarded from there to Node 2.
3. Node 2 subsequently retrieves the person's information (water level and desired temperature) from its database based on the received ID and sends it to Node 1 via the broker.
4. Finally, Node 1, which is the NodeMcu module, adjusts the requested water level and temperature in the smart bath based on this information. (Node 1, or the NodeMcu module, is equipped with three LEDs, the precise functions of which will be explained further.)

For example, imagine that two people live in a house, each with their unique ID (one with an RFID card and the other with a switch). Now, suppose Person A presents their card to Node 1 for identity verification. Their ID is obtained by the broker and sent to Node 2. Node 2, equipped with a database storing individuals' information, extracts the relevant information based on the received ID and sends it back to Node 1. This information includes the required water volume and the desired temperature, both scaled from 1 to 100.
