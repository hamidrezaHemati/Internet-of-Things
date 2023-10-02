# WIFI
The NodeMCU board used in this project has a wifi chip that can be in two modes: Station and Access Point.
In station mode it is able to connect to other devices and networks and in access point it can create a local network of its own. In these projects we aimed to utilize the wifi ability of this board in different real-word problems.

## Table of Contents
- Station Mode
- Access Point_Mode
- Create an HTML Page on NodeMCU
- RFID intro
- RFID Identity Verification System

## Station Mode
In this simple implementation of the station mode the module is able to scan all the wifi networks nearby and show them on a serial monitor. It can also connect to each of them if the password is provided. 
## Access Point_Mode
In this project the NodeMCU module works as a network provider and creates a local wifi network with a static IP address.
## Create an HTML Page on NodeMCU
Create an HTML page on NodeMCU that contains three buttons positioned in the center of the page. Upon selecting the first button, the output of the LDR sensor (as done in question four of the first exercise set) should be displayed to the user. By selecting the second button, the LED connected to the NodeMCU board should turn on, and by selecting the third button, the LED should turn off. You can use either Point Access or Station mode for this project.
## RFID intro
explaination of what is RFID and how it works
## RFID Identity Verification System
One of the applications of RFID technology is in identity verification systems. To achieve this, it is necessary for a person to bring their card close to the RFID Reader module so that their entry into the system can be recognized as authorized. Write a program that, when a card or tag is brought close to the module, first reads its information and displays it in the Serial Monitor. Then, using the NTP protocol, display the time of entry for the individual in the Serial Monitor. If this time falls within a predefined time interval (e.g., between 9 AM and 3 PM), turn on an LED for 3 seconds; otherwise, activate a Buzzer for 3 seconds.