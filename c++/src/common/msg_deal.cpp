#include "msg_deal.h"

#include <iostream>

#include "pybind11/pybind11.h"
#include "pybind11/embed.h"
#include "pybind11/pytypes.h"

namespace py = pybind11;

void MsgDeal::readRosBagContent(const std::string &bagPath)
{
    py::scoped_interpreter guard{}; // 初始化 Python 解释器, 同lock_gurd，作用域结束自动回收

    try
    {
        auto sys = py::module::import("sys");
        sys.attr("path").attr("insert")(0, "../python_moudle/thirdparty/");
        auto path = sys.attr("path");

        // std::cout << path.cast<std::string>() << std::endl;
        auto rsBag = py::module::import("rosbag");
        auto bag = rsBag.attr("Bag")("../python_moudle/test.bag");
        py::list topics;
        topics.append("chatter");
        topics.append("numbers");
        // auto result = bag.attr("read_messages")("chatter");
        auto result = bag.attr("read_messages")(topics);
        for (auto &test : result)
        {
            auto tt = test.cast<py::tuple>();
            std::cout << "topic: " << tt[0].cast<std::string>() << std::endl; // topic

            // 调用Python的dir()函数来获取对象的成员
            py::list members = py::cast<py::list>(py::globals()["dir"](tt[1]));
            // 打印出成员名
            for (py::handle h : members)
            {
                std::cout << py::cast<std::string>(h) << std::endl;
            }

            auto data = tt[1].attr("data");
            if (py::isinstance<py::int_>(data))
            {
                // std::cout << "The object is an integer.\n";
                std::cout << "data: " << data.cast<int>() << std::endl;
            }
            else if (py::isinstance<py::str>(data))
            {
                // std::cout << "The object is a string.\n";
                std::cout << "data: " << data.cast<std::string>() << std::endl;
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