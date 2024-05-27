#include "example_msg_deal.h"

#include <iostream>
#include <filesystem>
#include <unistd.h>

#include "base/json.hpp"

using json = nlohmann::json;
// const std::string rosInfoSaveDir = "./rosbag_content";

void MsgDealExample::dataWriteToJsonFile(const std::string &topicName, const std::string &jsonContent)
{
    // 写入到文件中
    std::string fileName = m_saveJsonFileDir_ + "/f_" + topicName + ".json";
    std::filesystem::path p = static_cast<std::filesystem::path>(fileName).parent_path();
    if (!std::filesystem::exists(p))
    {
        std::filesystem::create_directories(p);
    }

    FILE *fp = nullptr;
    auto res = m_saveJsonFileMap_.find(topicName);
    if (res != m_saveJsonFileMap_.end())
    {
        fp = res->second;
    }
    else
    {
        fp = fopen(fileName.c_str(), "w+");
        m_saveJsonFileMap_.insert({topicName, fp});
    }
    if (!fp)
    {
        return;
    }

    long pos = ftell(fp);
    if (pos == 0)
    {   // 将所有消息作为一个json数组,当文件无消息时,插入数组的做中括号
        fprintf(fp, "[\n");
    }
    else
    {   // 如果要新增消息,则需要覆盖最后三个字符 \n]\n
        fseek(fp, -3, SEEK_CUR);
        // 增加消息的分隔符
        fprintf(fp, ",\n");
    }

    // 写消息
    auto formatJsonStr = json::parse(jsonContent).dump(4);
    fwrite(formatJsonStr.c_str(), 1, formatJsonStr.size(), fp);

    // 在结尾插入右中括号封闭json
    fprintf(fp, "\n]\n");

    fflush(fp);
}

void MsgDealExample::defaultDealFunc(const std::string &topicName, const std::string &jsonContent)
{
    dataWriteToJsonFile(topicName, jsonContent);
}

void MsgDealExample::readRosBagContent(const std::string &bagPath, const std::vector<std::string> &vTopicNames,
                                       unsigned int startStamp, unsigned int endStamp)
{
    MsgDeal::readRosBagContent(bagPath, vTopicNames, startStamp, endStamp);

    for (auto &v : m_saveJsonFileMap_)
    {
        fclose(v.second);
    }
    m_saveJsonFileMap_.clear();
}