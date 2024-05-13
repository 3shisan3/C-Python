cmake_minimum_required(VERSION 3.10)  
  
project(C++_call_Python)  

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
  
# 查找Python库  
find_package(PythonLibs 3 REQUIRED)
include_directories(${PYTHON_INCLUDE_DIRS})  
# message(STATUS "${PROJECT_NAME} PYTHON_INCLUDE_DIRS: ${PYTHON_INCLUDE_DIRS}")
# message(STATUS "${PROJECT_NAME} PYTHON_LIBRARIES: ${PYTHON_LIBRARIES}")

# C++代码路径
set(ccCodePath ${CMAKE_CURRENT_SOURCE_DIR}/c++)

include_directories(${ccCodePath}/thirdparty/include)
# 添加你的源代码文件  
add_executable(${PROJECT_NAME} ${ccCodePath}/main.cpp)
  
# 链接Python库  
target_link_libraries(${PROJECT_NAME} ${PYTHON_LIBRARIES})  
  
# （可选）如果你的系统上Python库的安装位置不是标准的，你可能需要设置PYTHON_LIBRARY_DIR和PYTHON_INCLUDE_DIR  
# set(PYTHON_LIBRARY_DIR "/path/to/python/lib")  
# set(PYTHON_INCLUDE_DIR "/path/to/python/include")  
# find_library(PYTHON_LIBRARY NAMES python3.x m PATHS ${PYTHON_LIBRARY_DIR} NO_DEFAULT_PATH)  
# target_link_libraries(my_program ${PYTHON_LIBRARY})  
# include_directories(${PYTHON_INCLUDE_DIR})