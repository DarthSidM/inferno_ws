#!/usr/bin/env python
import rospy
from sensor_msgs.msg import Joy
from serial_package import ctrl
from serial.tools.list_ports import comports

baud_rate = 115200

port = ctrl.SerialPortChecker(baud_rate, 2).find_port("arm")
armObj = ctrl.arm(port(), baud_rate)
armObj.connect()

def callback(data):
    status = armObj.setState(round(data.axes[0]*255), round(data.axes[1]*255), round(data.axes[4]*255), round(data.axes[3]*255), data.buttons)
    armObj.serialWrite()
    rospy.loginfo("%s", status)

def listener():
    rospy.init_node('listener', anonymous=True)
    rospy.Subscriber('joy', Joy, callback)

    # spin() simply keeps python from exiting until this node is stopped
    rospy.spin()

if __name__ == '__main__':
    listener()
