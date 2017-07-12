#!/usr/bin/env python
# Description: If you need enable debug mode use this script to allow that the
# Adafruit HUZZAH ESP8266 prints messages in the serial port of the activity 
# that it is executed
# Author: Edward U. Benitez Rendon
# Date: 11-07-17

import paho.mqtt.client as mqtt
import time
import argparse

parser = argparse.ArgumentParser(description='Enable Debug Mode in Modules.')
parser.add_argument('board', type=int,
                    help='Specify the board to enable debug mode')
parser.add_argument('debug', type=int,
                    help='Enable debug mode')

args = parser.parse_args()
print(args.debug)
print(args.board)

mqttc = mqtt.Client("debugMode")
mqttc.connect("192.168.1.136", 1883)

if (args.board == 1):
	mqttc.publish("myHome/board/room/debug", str(args.debug))
