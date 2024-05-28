#include "readRosBag.h"

#include <iostream>
#include <filesystem>
#include <thread>
#include <unistd.h>

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

int main(int argc, char *argv[])
{
    auto runPath = getCurrentFolder(argc, argv);
    std::string dependDir = findSubdirectoryPath(runPath, "python_moudle");
    while (dependDir.empty())
    {
        runPath += "/..";
        dependDir = findSubdirectoryPath(runPath, "python_moudle");
    }
    std::cout << dependDir << std::endl;
    c_py::rosbag::setPyModulePath(dependDir);
    // 检查是否提供了足够的参数
    if (argc < 2)
    {
        std::cerr << "Usage: " << argv[0] << " <file_path>" << std::endl;
        std::thread t1([](){
            c_py::rosbag::readRosBag("./test.bag");
        });
        t1.detach();
        sleep(3);
        std::thread t2([](){
            c_py::rosbag::readRosBag("");
        });
        std::thread t3([&](){
            c_py::rosbag::readRosBag("/home/shisan/program_running/gelanqu_2023-10-14-15-44-09.bag");
        });
        t2.join();
        t3.join();

        return 1;
    }

    // 第一个参数是程序名称，所以文件路径从第二个参数开始
    std::string filePath = argv[1]; // 假设我们只关心第一个文件路径参数

    // 输出捕获到的文件路径
    std::cout << "File path: " << filePath << std::endl;



    return 0;
}