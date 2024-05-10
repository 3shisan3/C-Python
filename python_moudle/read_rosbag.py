import os
import sys

dependlib_root = os.path.abspath(os.path.join(os.path.dirname(__file__), './thirdparty'))
sys.path.insert(0, dependlib_root)
print(sys.path)

import rosbag

# 获取ros包中指定topic的内容，二进制数据返回
def getRosMsgInfo(bag_path, topic_name):
    bag = rosbag.Bag(bag_path)
    index = 0
    for topic, msg, t in bag.read_messages(topics=topic_name):
        header = msg.header
        header_seq = header.seq 
        stamp_sec = header.stamp.secs
        stamp_nsec = header.stamp.nsecs
        data = msg.data #bytes
        print('{:0>5d} {} {} {}'.format(index, header_seq, stamp_sec, stamp_nsec))
        index += 1
        
