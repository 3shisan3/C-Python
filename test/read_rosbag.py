import os
import sys

dependlib_root = os.path.abspath(os.path.join(os.path.dirname(__file__), '../python_moudle/thirdparty'))
sys.path.insert(0, dependlib_root)
print(sys.path)

import rosbag

# 获取ros包中指定topic的内容，二进制数据返回
def getRosMsgInfo(bag_path, topic_name):
    bag = rosbag.Bag(bag_path)
    index = 0
    # msg = bag.read_messages(topics=topic_name)
    # for topic, msg, t in bag.read_messages(topics=topic_name):
    # for topic, msg, t in bag.read_messages(topics=['chatter', 'numbers']):
    for test in bag.read_messages(topics=topic_name):  
        print(type(test))
        topic, msg, t = test
        # print(msg.data)
        print(type(msg))
        print(topic)
        print(msg)
        # header = msg.header
        # header_seq = header.seq 
        # stamp_sec = header.stamp.secs
        # stamp_nsec = header.stamp.nsecs
        # print('{:0>5d} {} {} {}'.format(index, header_seq, stamp_sec, stamp_nsec))
        index += 1

# ----test code-----   
if __name__ == "__main__":  
    if len(sys.argv) != 3:  
        print("Usage: python script.py file_path topic_name")  
        sys.exit(1)  
      
    file_path = sys.argv[1]  
    match_name = sys.argv[2]  
      
    getRosMsgInfo(file_path, match_name)