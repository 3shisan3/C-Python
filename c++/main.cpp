#include "pybind11/pybind11.h"
#include "pybind11/embed.h"
#include "pybind11/pytypes.h"
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
        for (auto &test : result) {
          auto tt = test.cast<py::tuple>();
          std::cout << "topic: " << tt[0].cast<std::string>() << std::endl; // topic
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
}