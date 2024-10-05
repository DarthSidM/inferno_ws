#!/usr/bin/env python
import rospy
from sensor_msgs.msg import Joy
from serial_package import ctrl

driveObj = ctrl.drive("/dev/ttyUSB0", 115200)


def callback(data):
    status = driveObj.ctrl(round(data.axes[0]*255), round(data.axes[1]*255))
    rospy.loginfo("%s", status)


def listener():

    # In ROS, nodes are uniquely named. If two nodes with the same
    # name are launched, the previous one is kicked off. The
    # anonymous=True flag means that rospy will choose a unique
    # name for our 'listener' node so that multiple listeners can
    # run simultaneously.
    rospy.init_node('listener', anonymous=True)

    rospy.Subscriber('joy', Joy, callback)

    # spin() simply keeps python from exiting until this node is stopped
    rospy.spin()

if __name__ == '__main__':
    listener()
