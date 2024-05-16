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
		${PYTHON_INCLUDE_DIRS_NOTE}
)
target_link_libraries(${PROJECT_NAME} ${PYTHON_LIBRARIES_NOTE})

# 指定当其他目标链接到该库时也需要链接的库
# set_target_properties(${PROJECT_NAME} PROPERTIES  
#     INTERFACE_LINK_LIBRARIES "${PYTHON_LIBRARIES_NOTE};"  
# )

# 将对外输出文件导出
if (EXPOSE_SUBMODULES)
	install(FILES ${CMAKE_CURRENT_SOURCE_DIR}/msg_deal.h DESTINATION ${CMAKE_INSTALL_PREFIX}/include)
	install(TARGETS ${PROJECT_NAME} 
		LIBRARY DESTINATION ${CMAKE_INSTALL_PREFIX}/lib
		ARCHIVE DESTINATION ${CMAKE_INSTALL_PREFIX}/lib
	)
endif()