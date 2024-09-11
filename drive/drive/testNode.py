import rclpy
from rclpy.node import Node

from sensor_msgs.msg import Joy

import ctrl
driveObj = ctrl.control("/dev/ttyUSB0", 115200)

class test_node(Node):
    def __init__(self):
        super().__init__('test_node')
        self.subscription = self.create_subscription(
            Joy,
            'joy',
            self.listener_callback,
            10)
        self.subscription  # prevent unused variable warning

    def listener_callback(self, msg):
        #status = driveObj.driveCtrl(round(msg.axes[0]*255), round(msg.axes[1]*255))
        driveObj.armCtrl(round(msg.axes[0]*255), round(msg.axes[1]*255), round(msg.axes[4]*255), msg.buttons)
        #self.get_logger().info('%s' % status)


def main(args=None):
    rclpy.init(args=args)

    test = test_node()

    rclpy.spin(test)

    # Destroy the node explicitly
    # (optional - otherwise it will be done automatically
    # when the garbage collector destroys the node object)
    test.destroy_node()
    rclpy.shutdown()
