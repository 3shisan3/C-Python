/***************************************************************
Copyright (c) 2022-2030, shisan233@sszc.live.
All rights reserved.
File:        readRosBag.h
Version:     1.0
Author:      cjx
start date: 2024-5-13
Description: 解析数据封装。
Version history

[序号]    |   [修改日期]  |   [修改者]   |   [修改内容]
1             2024-5-26      cjx        create

*****************************************************************/

#ifndef READ_ROS_BAG_H
#define READ_ROS_BAG_H

#include <functional>
#include <string>
#include <unordered_map>
#include <vector>

namespace c_py::rosbag
{


// 读取rosbag内容时限制指定的数据获取
struct LimitInfo
{
    std::vector<std::string> topicNames;    // 想要读取的topic集合
    unsigned int start = 0;                 // 截取开始时间戳，单位秒
    unsigned int end = 0;                   // 截取停止时间戳，单位秒
};

/**
 * @brief 设置python模块的路径
 * 
 * @param[in]  path                         python模块的路径地址
 * 
 */
void setPyModulePath(const std::string &path);

/**
 * @brief 以默认方式获取rosbag中的内容
 * 
 * @param[in]  path                         rosbag文件地址
 * @param[in]  limit                        获取rosbag文件中的指定内容
 * @param[in]  defFunc                      针对获取信息的默认处理方法
 * @param[in]  dealFuncs                    希望做特定处理的topic信息（当前不考虑生成多态化实例。使用右值传入，禁止dealfuncs在外部使用）
 *                                          仅需每次运行时首次read时传入
 * 
 */
void readRosBag(const std::string& path, const LimitInfo &limit = {},
                const std::function<void(const std::string &, const std::string &)> &defFunc = nullptr,
                std::unordered_map<std::string, std::function<void(const std::string &)>>&& dealFuncs = {});



} // namespace c_py::rosbag

#endif