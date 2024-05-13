/***************************************************************
Copyright (c) 2022-2030, shisan233@sszc.live.
All rights reserved.
File:        msg_deal.h
Version:     1.0
Author:      cjx
start date: 2024-5-13
Description: ros消息解析的父类
Version history

[序号]    |   [修改日期]  |   [修改者]   |   [修改内容]
1             2024-5-13      cjx        create

*****************************************************************/

#ifndef MSG_DEAL_BASE_H
#define MSG_DEAL_BASE_H

#include <string>
#include <unordered_map>


class MsgDeal
{
public:
    // std::unordered_map<std::string, std::function<void()>> s_funcsMap;

protected:
    void readRosBagContent(const std::string &bagPath);

    void readRosBagContent(const std::string &bagPath, const std::string &topicName);
};



#endif