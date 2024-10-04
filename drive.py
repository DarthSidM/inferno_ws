#!/usr/bin/env python3

import rospy
from sensor_msgs.msg import Joy
import ctrl

# Initialize the drive control object
driveObj = ctrl.control("/dev/ttyUSB0", 115200)
driveObj.connect()

class ControlNode:
    """
    A ROS1 node for controlling a device based on joystick input.
    Subscribes to the 'joy' topic and processes joystick messages.
    """
    def __init__(self):
        rospy.init_node('control_node', anonymous=True)
        self.subscription = rospy.Subscriber('/joy', Joy, self.listener_callback)

    def listener_callback(self, msg):
        """
        Callback function that is called whenever a new joystick message is received.
        
        :param msg: The joystick message containing axes and button states.
        """
        status = driveObj.driveCtrl(round(msg.axes[0]*255), round(msg.axes[1]*255))
        print(status)
        driveObj.serialWrite()

def main():
    node = ControlNode()

    # Keep the node running until it's interrupted
    try:
        rospy.spin()
    except rospy.ROSInterruptException:
        pass
    finally:
        # Ensure the drive object is cleaned up properly
        if hasattr(driveObj, 'disconnect'):
            driveObj.disconnect()

if __name__ == '__main__':
    main()
