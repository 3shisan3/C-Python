#ifndef MSG_DEAL_EXAMPLE_H
#define MSG_DEAL_EXAMPLE_H

#include "msg_deal.h"


class MsgDealExample : public MsgDeal
{

protected:    
    void defaultDealFunc(const std::string &topicName,const std::string &jsonContent) override;

};


#endif