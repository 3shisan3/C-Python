#include "readRosBag.h"

#include <iostream>
#include <memory>
#include "pybind11/pybind11.h"

#include "special/example_msg_deal.h"

class CustomMsgDeal : public MsgDealExample, public std::enable_shared_from_this<CustomMsgDeal>
{
public:
    /**
     * @brief 设置默认的处理函数
     *
     * @param[in] model                 使用自带模式（0 直接打印，1 输出到文件，2 使用自定义函数（传空即不做处理））
     * @param[in] defFunc               自定义的处理函数集合(右值传入 请使用std::move)
     *
     */
    void setDefDealFunc(int model, const std::function<void(const std::string &, const std::string &)> &func = nullptr);

    /**
     * @brief 设置自定义的处理函数
     *
     * @param[in] dealmap               自定义的处理函数集合(右值传入 请使用std::move)
     *
     */
    void setTopicDealFuncMap(std::unordered_map<std::string, std::function<void(const std::string &)>>&& dealmap);

protected:
    void defaultDealFunc(const std::string &topicName, const std::string &jsonContent);

private:
    std::function<void(const std::string &, const std::string &)> m_defFunc_/*  = [] (const std::string &topicName, const std::string &jsonContent){
        std::cout << topicName << ": " << jsonContent << std::endl;
    } */;
};

void CustomMsgDeal::setDefDealFunc(int model, const std::function<void(const std::string &, const std::string &)> &func)
{
    switch (model)
    {
    case 0:
        {   // 主要调试时使用
            m_defFunc_ = [](const std::string &topicName, const std::string &jsonContent) {
                std::cout << topicName << ": " << jsonContent << std::endl;
            };
        }
        break;
    case 1:
        {
            // 实际会绑定到派生类重写的defaultDealFunc上
            // m_defFunc_ = std::bind(&MsgDealExample::defaultDealFunc, shared_from_this(), std::placeholders::_1, std::placeholders::_2);
            m_defFunc_ = std::bind(&MsgDealExample::dataWriteToJsonFile, shared_from_this(), std::placeholders::_1, std::placeholders::_2);
        }
        break;
    case 2:
        {
            if (func == nullptr)
            {
                m_defFunc_ = [](const std::string &topicName, const std::string &jsonContent) {};
            }
            else
            {
                m_defFunc_ = func;
            }
        }
        break;
    default:
        break;
    }
}

void CustomMsgDeal::setTopicDealFuncMap(std::unordered_map<std::string, std::function<void(const std::string &)>>&& dealmap)
{
    // m_funcsMap.swap(dealmap);
    m_funcsMap = std::move(dealmap);
}

void CustomMsgDeal::defaultDealFunc(const std::string &topicName, const std::string &jsonContent)
{
    (void)m_defFunc_(topicName, jsonContent);
}


namespace c_py::rosbag
{

void setPyModulePath(const std::string &path)
{
    MsgDeal::setDependPyMoudleDir(path);
}

void readRosBag(const std::string& path, const LimitInfo &limit,
                const std::function<void(const std::string &, const std::string &)> &defFunc,
                std::unordered_map<std::string, std::function<void(const std::string &)>>&& dealFuncs)
{
    static auto onlyIns = std::make_shared<CustomMsgDeal>();

    static std::once_flag flag;  
    if (defFunc != nullptr)
    {
        std::call_once(flag, []() {});      // 修改flag状态
        onlyIns->setDefDealFunc(2, defFunc);
    }
    else
    {
        std::call_once(flag, []() {
            onlyIns->setDefDealFunc(1);     // 默认处理为文件
        });
    }

    if (!dealFuncs.empty())
    {
        onlyIns->setTopicDealFuncMap(std::move(dealFuncs));
    }

    if (PyGILState_Check())
    {
        pybind11::gil_scoped_release release{};
    }
    onlyIns->readRosBagContent(path, limit.topicNames, limit.start, limit.end);
}


} // namespace c_py::rosbag