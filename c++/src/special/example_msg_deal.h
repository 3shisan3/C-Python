#ifndef MSG_DEAL_EXAMPLE_H
#define MSG_DEAL_EXAMPLE_H

#include "msg_deal.h"

#include <map>
class MsgDealExample : public MsgDeal
{
public:
    void readRosBagContent(const std::string &bagPath, const std::vector<std::string> &vTopicNames = {},
                           unsigned int startStamp = 0, unsigned int endStamp = 0);

protected:
    void defaultDealFunc(const std::string &topicName, const std::string &jsonContent) override;

private:
    std::string m_saveJsonFileDir_ = "./rosbag_content";
    std::map<std::string, FILE *> m_saveJsonFileMap_;
};

#endif