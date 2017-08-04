#!/usr/bin/python
import rospy

import smbus
import time
import struct
bus = smbus.SMBus(1)
address = 0x50
voltage = (0x60, 0x61)
current = (0x62, 0x63)
from std_msgs.msg import Float32MultiArray


def readValues(reg):
	try:
                Register = bus.read_i2c_block_data(address, 0x60,4)
        	b1 = 0
        	b2 = Register[1]
		#b1 = bus.read_byte_data(address,reg[0])
        	#b2 = bus.read_byte_data(address,reg[1])
		data = [0,0,b2,b1]
        	b = ''.join(chr(i) for i in data)
        	value = struct.unpack('>i', b)[0]
                rospy.loginfo([Register[0],Register[1], Register[2],Register[3]])
        	return value/100.0

	except IOError as e:
		return -1





pub = rospy.Publisher('powerValues', Float32MultiArray, queue_size=1)
power_values = Float32MultiArray()
rospy.init_node('powerDetails')
rate = rospy.Rate(1) # 10hz

while not rospy.is_shutdown():
        voltageOut = readValues(voltage)
	#time.sleep(1)
        #currentOut = readValues(current)
	#time.sleep(1)
	currentOut = 0
	power_values.data= [voltageOut,currentOut]
	pub.publish(power_values)
	rate.sleep()

