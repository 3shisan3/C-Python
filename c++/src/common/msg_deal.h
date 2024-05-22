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

#include <atomic>
#include <functional>
#include <string>
#include <unordered_map>
#include <vector>


class MsgDeal
{
public:
    MsgDeal();

    static void setDependPyMoudleDir(const std::string &pathStr);

    /**
     * @brief 解析指定rosbag包中数据
     * 
     * @param[in] bagPath               解析rogbag的文件路径
     * @param[in] vTopicNames           指定想要获取的topic集合
     * @param[in] startStamp            过滤想要的时间段内信息，开始时间戳  （秒）
     * @param[in] endStamp              过滤想要的时间段内信息，结束时间戳  （秒）
     * 
     */
    void readRosBagContent(const std::string &bagPath, const std::vector<std::string> &vTopicNames = {},
                           unsigned int startStamp = 0, unsigned int endStamp = 0);

protected:
    virtual void defaultDealFunc(const std::string &topicName,const std::string &jsonContent);

    // topic -> dealfunc
    std::unordered_map<std::string, std::function<void(const std::string &)>> m_funcsMap;

private:
    enum class Status
    {
        RUNNING,
        FREE,
    };

    static std::atomic<MsgDeal::Status> s_useStatus_;
    std::atomic<MsgDeal::Status> m_useStatus_;
    static std::string s_pyMoudlePath_;

};



#endif