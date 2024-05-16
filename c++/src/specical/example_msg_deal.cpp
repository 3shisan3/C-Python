#include "example_msg_deal.h"

#include <iostream>
#include <unistd.h>


void MsgDealExample::defaultDealFunc(const std::string &topicName,const std::string &jsonContent)
{
     std::cout << topicName << ": " << jsonContent << std::endl;

     while (true)
     {
        sleep(1);
        std::cout << "wait" << std::endl;
     }
}