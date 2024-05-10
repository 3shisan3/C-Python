#include <Python.h>

int main()
{
    Py_Initialize();    // 初始化

    PyRun_SimpleString("print('hello')");

    Py_Finalize();      //释放资源
}