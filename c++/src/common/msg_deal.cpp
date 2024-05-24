#include "msg_deal.h"

#include <iostream>
#include <mutex>
#include <thread>

#include "pybind11/pybind11.h"
#include "pybind11/embed.h"
#include "pybind11/pytypes.h"

#include "json.hpp"
// #include "utils_method.h"

namespace py = pybind11;
using json = ::nlohmann::json;

bool object_to_json(py::object obj, std::string &result)
{
    // 使用 Python 的 json.dumps 来将对象转换为 JSON 字符串
    py::object json_module = py::module::import("json");
    py::object dumps_func = json_module.attr("dumps");

    if (py::isinstance<py::set>(obj))
    {
        py::list lst;
        for (auto item : py::iter(obj.cast<py::set>()))
        {
            lst.append(item);
        }
        obj = lst;
    }

    try
    {
        py::object json_obj = dumps_func(obj);
        result = json_obj.cast<std::string>();
    }
    catch (...)
    {
        return false;
    }

    return true;
}

void object_to_json(py::object obj, json &parent)
{
    // 调用Python的dir()函数来获取对象的成员
    py::list members = py::module_::import("builtins").attr("dir")(obj);
    // 遍历所有成员, 并存入为json
    for (py::handle h : members)
    {
        std::string member_name = py::cast<std::string>(h);
        // 过滤python内部默认的私有成员
        if (member_name.size() >= 2 &&
            (member_name.front() == '_' && member_name.front() + 1 == '_') ||
            (member_name.back() == '_' && member_name.back() - 1 == '_') ||
            (member_name.front() == '_' && member_name.back() == '_'))
        {
            continue;
        }
        // 尝试获取成员，并检查它是否可调用
        if (!py::hasattr(obj, member_name.c_str()))
        {
            continue;
        }
        // 过滤python内部默认的私有属性
        if (!member_name.empty() && member_name.front() == '_')
        {
            continue;
        }
        // 获取属于对象的数据成员
        py::object member = obj.attr(member_name.c_str());
        if (!member.is_none() &&
            !py::isinstance<py::function>(member) &&
            !py::isinstance<py::staticmethod>(member) &&
            !py::hasattr(member, "__call__"))
        {
            std::string dataStr;
            if (object_to_json(member, dataStr))
            {
                parent[member_name] = dataStr;
            }
            else
            {
                object_to_json(member, parent[member_name]);
            }
        }
    }
}

std::atomic<MsgDeal::Status> MsgDeal::s_useStatus_ = MsgDeal::Status::FREE;
std::string MsgDeal::s_pyMoudlePath_ = "../python_moudle";
std::atomic_int MsgDeal::s_insNums_ = 0;
// std::unique_ptr<pybind11::gil_scoped_release> MsgDeal::s_gulRelease_ = nullptr;

MsgDeal::MsgDeal()
{
    m_useStatus_ = Status::FREE;
    if (s_insNums_ == 0)
    {
        py::initialize_interpreter();

        auto sys = py::module::import("sys");
        auto pathStr = s_pyMoudlePath_ + "/thirdparty/";
        sys.attr("path").attr("insert")(0, pathStr);

        // s_gulRelease_ = std::make_unique<pybind11::gil_scoped_release>();
    }
    ++s_insNums_;
}

MsgDeal::~MsgDeal()
{
    --s_insNums_;
    if (s_insNums_ == 0)
    {
        py::finalize_interpreter();
        // s_gulRelease_ = nullptr;
    }
}

void MsgDeal::setDependPyMoudleDir(const std::string &pathStr)
{
    // std::call_once(m_s_singleFlag, [&] {
        s_pyMoudlePath_ = pathStr;
    // });
}

// thread_local py::object t_rosbagModule_;
void MsgDeal::readRosBagContent(const std::string &bagPath, const std::vector<std::string> &vTopicNames,
                                unsigned int startStamp, unsigned int endStamp)
{
    while (s_useStatus_ != Status::FREE)
    { // wait
        m_useStatus_ = Status::FREE;
        sleep(1);
    }

    s_useStatus_ = Status::RUNNING;
    // py::scoped_interpreter guard{};
    // py::initialize_interpreter();
    m_useStatus_ = Status::RUNNING;

    try
    {
        pybind11::gil_scoped_acquire acquire;

        auto rosbag = py::module::import("rosbag");
        auto bag = rosbag.attr("Bag")(bagPath);

        py::list topics;
        for (const auto &name : vTopicNames)
        {
            topics.append(name);
        }
        auto bagData = startStamp == endStamp ? bag.attr("read_messages")(topics) :
                                                bag.attr("read_messages")(topics, startStamp, endStamp);
        for (auto &dataPair : bagData)
        {
            if (m_useStatus_ == Status::FREE)
            {
                break;
            }

            auto tt = dataPair.cast<py::tuple>();
            const std::string topicName = tt[0].cast<std::string>();

            json root;
            object_to_json(tt[1], root);

            if (m_funcsMap.find(topicName) != m_funcsMap.end())
            {
                auto dealFunc = m_funcsMap[topicName];
                (void) dealFunc(root.dump());
            }
            else
            {
                defaultDealFunc(topicName, root.dump());
            }
        }
    }
    catch (std::exception &e)
    {
        std::cout << e.what() << std::endl;
    }

    m_useStatus_ = Status::FREE;
    // py::finalize_interpreter();
    s_useStatus_ = Status::FREE;
}

void MsgDeal::defaultDealFunc(const std::string &topicName, const std::string &jsonContent)
{
    py::print(topicName, ": ", jsonContent);
}