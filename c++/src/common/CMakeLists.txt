# 设置CMake最低版本要求  
cmake_minimum_required(VERSION 3.10)  
  
# 设置项目名称  
project(rosmsg_deal_base)

# 指定编译输出的库路径
if (EXPOSE_SUBMODULES)
	set(LIBRARY_OUTPUT_PATH ${SUBLIB_OUT_PATH})
endif()

# 添加源代码文件  
file(GLOB SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/*.cpp")  

# 生成静态库文件  
add_library(${PROJECT_NAME} STATIC ${SOURCES})

# 添加头文件目录  
target_include_directories(${PROJECT_NAME} PUBLIC
		${CMAKE_CURRENT_SOURCE_DIR}
		${ccCodePath}/src/base
		${ccCodePath}/thirdparty/include
)


# 查找Python库  
find_package(PythonLibs 3)
# find_package(Python3 COMPONENTS Interpreter Development)

# 添加条件需要链接的库文件 
if(PythonLibs_FOUND)
    message(STATUS "Python3 Lib found")
    include_directories(${PYTHON_INCLUDE_DIRS})  
	target_link_libraries(${PROJECT_NAME} ${PYTHON_LIBRARIES})

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
	target_link_libraries(${PROJECT_NAME} ${PYTHON_LIBRARIES_NOTE})
endif()
message(STATUS "${PROJECT_NAME} PYTHON_INCLUDE_DIRS: ${PYTHON_INCLUDE_DIRS_NOTE}")
message(STATUS "${PROJECT_NAME} PYTHON_LIBRARIES: ${PYTHON_LIBRARIES_NOTE}")

# 将对外输出文件导出
if (EXPOSE_SUBMODULES)
	install(FILES ${CMAKE_CURRENT_SOURCE_DIR}/msg_deal.h DESTINATION ${CMAKE_INSTALL_PREFIX}/include)
	install(TARGETS ${PROJECT_NAME} 
		LIBRARY DESTINATION ${CMAKE_INSTALL_PREFIX}/lib
		ARCHIVE DESTINATION ${CMAKE_INSTALL_PREFIX}/lib
	)
endif()