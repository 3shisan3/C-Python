cmake_minimum_required(VERSION 3.10)  
  
project(C++_call_Python)  

# 判断操作系统类型  
if(UNIX)  
    # 在 Unix-like 系统中使用 $HOME 环境变量  
    execute_process(COMMAND sh -c "echo $HOME" OUTPUT_VARIABLE USER_HOME_DIR)  
    string(STRIP ${USER_HOME_DIR} USER_HOME_DIR)  
elseif(WIN32)  
    # 在 Windows 中使用 %USERPROFILE% 环境变量  
    execute_process(COMMAND cmd /C "echo %USERPROFILE%" OUTPUT_VARIABLE USER_HOME_DIR)  
    string(STRIP ${USER_HOME_DIR} USER_HOME_DIR)  
    # 将反斜杠替换为正斜杠（可选，但可以使路径更易于处理）  
    string(REPLACE "\\" "/" USER_HOME_DIR ${USER_HOME_DIR})  
endif() 

set(CMAKE_INSTALL_PREFIX "${USER_HOME_DIR}/program_running/${PROJECT_NAME}")
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
  
# 查找Python库  
find_package(PythonLibs 3)
# find_package(Python3 COMPONENTS Interpreter Development)
message(STATUS "${PROJECT_NAME} PYTHON_INCLUDE_DIRS: ${PYTHON_INCLUDE_DIRS}")
message(STATUS "${PROJECT_NAME} PYTHON_LIBRARIES: ${PYTHON_LIBRARIES}")

# C++代码路径
set(ccCodePath ${CMAKE_CURRENT_SOURCE_DIR}/c++)

# 添加源代码文件  
file(GLOB_RECURSE SOURCES ${ccCodePath}/src/*.cpp)  
# 添加头文件目录 
include_directories(${ccCodePath}/thirdparty/include
                    ${ccCodePath}/src
)

# 生成可执行程序  
add_executable(${PROJECT_NAME} ${SOURCES}
                               ${ccCodePath}/main.cpp
)

# 添加条件需要链接的库文件 
if(PythonLibs_FOUND)
    message(STATUS "Python3 Lib found")
    include_directories(${PYTHON_INCLUDE_DIRS})  
	target_link_libraries(${PROJECT_NAME} ${PYTHON_LIBRARIES})
    # 将库文件打包方法参考 git@github.com:3shisan3/communicate_lib.git 此处暂不使用
else()
    message(STATUS "Python3 Lib not found")
	include_directories(${ccCodePath}/thirdparty/include/python3.10)
	list(APPEND DEPENDENCY_LIB
		${ccCodePath}/thirdparty/lib/libpython3.10.so.1.0
	)
	target_link_libraries(${PROJECT_NAME} ${DEPENDENCY_LIB})
endif()

# 添加必要的安装动态库文件
file(GLOB SHARED_LIBS ${ccCodePath}/thirdparty/lib/*.so.*)

install(TARGETS ${PROJECT_NAME} RUNTIME DESTINATION ${CMAKE_INSTALL_PREFIX}/bin)
install(FILES ${SHARED_LIBS} DESTINATION ${CMAKE_INSTALL_PREFIX}/lib)
install(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/python_moudle DESTINATION ${CMAKE_INSTALL_PREFIX})

# 设置RPATH, 确保运行时可以找到依赖库
set_target_properties(${PROJECT_NAME} PROPERTIES INSTALL_RPATH "$ORIGIN;$ORIGIN/lib;$ORIGIN/../lib")