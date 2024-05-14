#include "msg_deal.h"

#include <iostream>

#include "pybind11/pybind11.h"
#include "pybind11/embed.h"
#include "pybind11/pytypes.h"

#include "base/json.hpp"

namespace py = pybind11;
using json = ::nlohmann::json;

bool object_to_json(py::object obj, std::string &result)
{
    // py::scoped_interpreter guard{};

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
    catch(...)
    {
        return false;
    }
    
    return true;
}

void object_to_json(py::object obj, json &parent)
{
    // py::scoped_interpreter guard{};

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

void MsgDeal::readRosBagContent(const std::string &bagPath)
{
    py::scoped_interpreter guard{}; // 初始化 Python 解释器, 同lock_gurd，作用域结束自动回收

    try
    {
        auto sys = py::module::import("sys");
        sys.attr("path").attr("insert")(0, "../python_moudle/thirdparty/");

        auto rsBag = py::module::import("rosbag");
        auto bag = rsBag.attr("Bag")(bagPath);

        auto bagData = bag.attr("read_messages")();
        for (auto &dataPair : bagData)
        {
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
                // todo 先直接打印
                py::print(root.dump());
            }
        }
    }
    catch (std::exception &e)
    {
        std::cout << e.what() << std::endl;
    }
}

void MsgDeal::readRosBagContent(const std::string &bagPath, const std::string &topicName)
{

}