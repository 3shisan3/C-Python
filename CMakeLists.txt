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

# 设置make install安装父目录
set(CMAKE_INSTALL_PREFIX "${USER_HOME_DIR}/program_running/${PROJECT_NAME}")
# 暴露所有编译单元的编译命令
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# 设置构建选项前提
option(BUILD_TEST "编译测试部分用例"	ON)
option(EXPOSE_SUBMODULES "子模块单独编译"	ON)

# 对外输出主目录(编译产出)
set(SHARED_ROOT_DIR ${PROJECT_SOURCE_DIR}/sdk)
# 指定编译输出的动态库路径
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${SHARED_ROOT_DIR}/lib)
set(SUBLIB_OUT_PATH ${CMAKE_LIBRARY_OUTPUT_DIRECTORY}/inner)

#设置构建中用到的变量
set(ROOT_PROJ_DIR ${PROJECT_SOURCE_DIR})
set(ROOT_BUILD_DIR ${CMAKE_BINARY_DIR})
message(STATUS "ROOT_BUILD_DIR: ${CMAKE_CURRENT_BINARY_DIR}")

# C++代码路径
set(ccCodePath ${CMAKE_CURRENT_SOURCE_DIR}/c++)

# 查找Python库  
find_package(PythonLibs 3)
# find_package(Python3 COMPONENTS Interpreter Development)

# 添加条件需要链接的库文件 
if(PythonLibs_FOUND)
    message(STATUS "Python3 Lib found")
    include_directories(${PYTHON_INCLUDE_DIRS})  

	set(PYTHON_INCLUDE_DIRS_NOTE ${PYTHON_INCLUDE_DIRS})
	list(APPEND PYTHON_LIBRARIES_NOTE ${PYTHON_LIBRARIES})
    # 将库文件打包方法参考 git@github.com:3shisan3/communicate_lib.git 此处暂不使用
else()
    message(STATUS "Python3 Lib not found")
	set(PYTHON_INCLUDE_DIRS_NOTE ${ccCodePath}/thirdparty/include/python3.10)
	include_directories(${PYTHON_INCLUDE_DIRS_NOTE})
	list(APPEND PYTHON_LIBRARIES_NOTE
		${ccCodePath}/thirdparty/lib/libpython3.10.so.1.0
	)
endif()
message(STATUS "${PROJECT_NAME} PYTHON_INCLUDE_DIRS: ${PYTHON_INCLUDE_DIRS_NOTE}")
message(STATUS "${PROJECT_NAME} PYTHON_LIBRARIES: ${PYTHON_LIBRARIES_NOTE}")

# 添加源代码文件  
file(GLOB_RECURSE SOURCES ${ccCodePath}/src/*.cpp)
# 子模块库存储
list(APPEND SUBMOUDLES_DIR_LIST ${ccCodePath}/src/common)
# 记录子项目的源文件
set(SubSources "")
# 添加模块编译项
foreach(subproject ${SUBMOUDLES_DIR_LIST})
	# 添加子模块编译
	add_subdirectory(${subproject})
	# 排除子模块文件在父项目的源文件
	file(GLOB_RECURSE subSources "${subproject}/*.cpp")
	list(APPEND SubSources ${subSources})
	foreach(subSource ${subSources})
		list(REMOVE_ITEM SOURCES "${subSource}")
	endforeach()
endforeach()

# 添加头文件目录 
include_directories(${ccCodePath}/src
                    ${SUBMOUDLES_DIR_LIST}
)

# 生成动态库文件
add_library(${PROJECT_NAME} SHARED ${SOURCES})
# 链接依赖关系，将子项目的源文件添加到主项目的构建过程中
target_sources(${PROJECT_NAME} PUBLIC ${SubSources})

# 添加需要链接的库文件
target_link_libraries(${PROJECT_NAME} rosmsg_deal_base)

# 测试项目
if (BUILD_TEST)
	# 设置日志打印级别
	# add_definitions(-DGLOBAL_LOG_LEVEL=1)

	message(STATUS "build test modules")
    # add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/test/app_demo ${CMAKE_BINARY_DIR}/app_demo)
	# add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/test/interface_test ${CMAKE_BINARY_DIR}/interface_test)
endif()

# 添加必要的安装动态库文件
file(GLOB SHARED_LIBS ${ccCodePath}/thirdparty/lib/*.so.*)

install(TARGETS ${PROJECT_NAME} RUNTIME DESTINATION ${CMAKE_INSTALL_PREFIX}/bin)
install(FILES ${SHARED_LIBS} DESTINATION ${CMAKE_INSTALL_PREFIX}/lib)
install(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/python_moudle DESTINATION ${CMAKE_INSTALL_PREFIX})

# 设置RPATH, 确保运行时可以找到依赖库
set_target_properties(${PROJECT_NAME} PROPERTIES INSTALL_RPATH "$ORIGIN;$ORIGIN/lib;$ORIGIN/../lib")