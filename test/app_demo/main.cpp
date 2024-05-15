/* #include "pybind11/pybind11.h"
#include "pybind11/embed.h"
#include "pybind11/pytypes.h"
#include <pybind11/functional.h>
#include <exception>
#include <iostream>
#include <tuple>

namespace py = pybind11;

int main()
{
    py::scoped_interpreter guard{}; // start the interpreter and keep it alive
    try {
        // auto os = py::module::import("os");
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
            py::list members = py::module_::import("builtins").attr("dir")(tt[1]);
            // 遍历成员并检查它们是否可调用
            for (py::handle h : members)
            {
                std::string member_name = py::cast<std::string>(h);
                if (member_name.size() >= 2 &&
                    (member_name.front() == '_' && member_name.front() + 1 == '_') ||
                    (member_name.back() == '_' && member_name.back() - 1 == '_') ||
                    (member_name.front() == '_' && member_name.back() == '_'))
                {
                  continue;
                }

                // 尝试获取成员，并检查它是否可调用
                if (!py::hasattr(tt[1], member_name.c_str()))
                {
                  continue;
                }
                py::object member = tt[1].attr(member_name.c_str());
                if (!member.is_none() &&
                    !py::isinstance<py::function>(member) &&
                    !py::isinstance<py::staticmethod>(member) &&
                    !py::hasattr(member, "__call__"))
                {
                  // 如果不是方法，则可能是数据成员
                  std::cout << member_name << std::endl;
                  // py::dict globals = py::module_::import("__main__").attr("__dict__");
                  // py::function print_func = globals["print"];
                  if (py::isinstance<py::int_>(member))
                  {
                    // std::cout << "The object is an integer.\n";
                    std::cout << "member: " << member.cast<int>() << std::endl;
                  }
                  else if (py::isinstance<py::str>(member))
                  {
                    std::cout << "member: " << member.cast<std::string>() << std::endl;
                  }
                  else if (py::isinstance<py::list>(member))
                  {
                    py::print(member);
                  }
                  else
                  {
                    std::string type_name = member.get_type().attr("__name__").cast<std::string>();
                    std::cout << "Type of object: " << type_name << std::endl;
                  }
                }
                // if (member.is_none())
                // {
                //     std::cout << member_name << std::endl;
                // }
            }

            std::cout << "-------------------------" << std::endl;
          auto data = tt[1].attr("data");
          if (py::isinstance<py::int_>(data)) {
            // std::cout << "The object is an integer.\n";
            std::cout << "data: " << data.cast<int>() << std::endl;
          } else if (py::isinstance<py::str>(data)) {
            // std::cout << "The object is a string.\n";
            std::cout << "data: " << data.cast<std::string>() << std::endl;
          }
        }
    } catch(std::exception &e) {
        std::cout << e.what() << std::endl;
    }
    return 0;
} */

#include <iostream>

#include "utils_method.h"
#include "msg_deal.h"

int main(int argc, char *argv[])
{
    auto runPath = getCurrentFolder(argc, argv);
    std::string dependDir = findSubdirectoryPath(runPath, "python_moudle");
    while (dependDir.empty())
    {
        runPath += "/..";
        dependDir = findSubdirectoryPath(runPath, "python_moudle");
    }
    MsgDeal::setDependPyMoudleDir(dependDir);
    // 检查是否提供了足够的参数
    if (argc < 2)
    {
        std::cerr << "Usage: " << argv[0] << " <file_path>" << std::endl;
        auto test = new MsgDeal();

        test->readRosBagContent("../test.bag");
        return 1;
    }

    // 第一个参数是程序名称，所以文件路径从第二个参数开始
    std::string filePath = argv[1]; // 假设我们只关心第一个文件路径参数

    // 输出捕获到的文件路径
    std::cout << "File path: " << filePath << std::endl;

    auto test = new MsgDeal();
    test->readRosBagContent(filePath);

    return 0;
}