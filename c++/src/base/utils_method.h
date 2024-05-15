/***************************************************************
Copyright (c) 2022-2030, shisan233@sszc.live.
All rights reserved.
File:        utils_method.h
Version:     1.0
Author:      cjx
start date: 2023-8-28
Description: 常用方法
Version history

[序号]    |   [修改日期]  |   [修改者]   |   [修改内容]
1             2023-8-28      cjx        create

*****************************************************************/

#ifndef _UTILS_METHOD_H_
#define _UTILS_METHOD_H_

#include <cmath>
#include <ctime>
#include <fstream>
#include <filesystem>
#include <stdint.h>
#include <string>
#include <vector>


// 获取当前时间
inline void getDateStamp(char * buf, size_t n)
{
	auto tt = std::time(nullptr);
	std::tm cur{};
	// gmtime_r(&tt, &cur);
	localtime_r(&tt, &cur);
	strftime(buf, n, "%a, %d %b %Y %H:%M:%S %Z", &cur);
}

// 获取程序运行路径
std::string getCurrentFolder(int argc, char *argv[])
{
    std::string execPath = argv[0];

    size_t lastSLashPos = execPath.find_last_of("/");

    return execPath.substr(0, lastSLashPos);
}

// 获取文件夹下某文件夹的路径
inline std::string findSubdirectoryPath(const std::filesystem::path &directory, const std::string &subdir_name)  
{  
    for (const auto &entry : std::filesystem::recursive_directory_iterator(directory))  
    {  
        if (entry.path().filename() == subdir_name && std::filesystem::is_directory(entry.path()))  
        {  
            return entry.path().string();  
        }  
    }  
    return ""; // 如果没有找到子文件夹，返回一个空的路径  
} 

#endif