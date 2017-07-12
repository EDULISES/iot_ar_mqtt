#!/usr/bin/env python
# Description: It was developed with the objective of simulate 
# publications from Unity to control the led color in the slave
# device.
# Author: Edward U. Benitez Rendon
# Date: 11-07-17

import paho.mqtt.client as mqtt
import time

mqttc = mqtt.Client("roomPublishing")
mqttc.connect("192.168.1.136", 1883)

while True:
	bgr = '{0:02x}'.format(0xff)
	mqttc.publish("myHome/lampRgb01/room", bgr)
	time.sleep(0.5)
	bgr = '{0:02x}'.format(0xff00)
	mqttc.publish("myHome/lampRgb01/room", bgr)
	time.sleep(0.5)
	bgr = '{0:02x}'.format(0xff0000)
	mqttc.publish("myHome/lampRgb01/room", bgr)
	time.sleep(0.5)
	bgr = '{0:02x}'.format(0)
	mqttc.publish("myHome/lampRgb01/room", bgr)