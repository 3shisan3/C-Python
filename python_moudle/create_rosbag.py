import os
import sys

dependlib_root = os.path.abspath(os.path.join(os.path.dirname(__file__), './thirdparty'))
sys.path.insert(0, dependlib_root)
print(sys.path)

import rosbag

from std_msgs.msg import Int32, String

bag = rosbag.Bag('test.bag', 'w')

try:
    s = String()
    s.data = 'foo'

    i = Int32()
    i.data = 42

    bag.write('chatter', s)
    bag.write('numbers', i)
finally:
    bag.close()