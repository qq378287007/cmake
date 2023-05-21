# cmake_minimum_required(VERSION 3.15)
cmake_minimum_required(VERSION 3.0.0)

# project(demo)
project(demo VERSION 0.1.0)

# set(CMAKE_CXX_STANDARD 11)
# set(CMAKE_CXX_STANDARD_REQUIRED True)

# 从指定的文件加载、运行CMake代码
# file直接处理
# module，则寻找module.cmake文件(${CMAKE_MODULE_PATH}，CMake的module等目录中查找）
# include(<file|module> [OPTIONAL] [RESULT_VARIABLE <VAR>] [NO_POLICY_SCOPE])

# 支持gdb
# set(CMAKE_BUILD_TYPE "Debug")
# set(CMAKE_CXX_FLAGS_DEBUG "$ENV{CXXFLAGS} -O0 -Wall -g -ggdb")
# set(CMAKE_CXX_FLAGS_RELEASE "$ENV{CXXFLAGS} -O3 -Wall")

# set(CMAKE_INCLUDE_CURRENT_DIR ON)

#[[
CMAKE_ARCHIVE_OUTPUT_DIRECTORY
    默认存放静态库的文件夹位置
CMAKE_LIBRARY_OUTPUT_DIRECTORY
    默认存放动态库的文件夹位置
LIBRARY_OUTPUT_PATH
    默认存放库文件的位置，如果产生的是静态库并且没有指定CMAKE_ARCHIVE_OUTPUT_DIRECTORY则存放在该目录下，
    动态库也类似；
CMAKE_RUNTIME_OUTPUT_DIRECTORY
    存放可执行软件的目录
]]

#[[针对目标设置
set_target_properties( targets...
    PROPERTIES
    ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib"
    LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib"
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin"
)
]]

#[[针对目标设置
# set output directories for all builds (Debug, Release, etc.)
foreach( OUTPUTCONFIG ${CMAKE_CONFIGURATION_TYPES} )
    string( TOUPPER ${OUTPUTCONFIG} OUTPUTCONFIG )
    set( CMAKE_ARCHIVE_OUTPUT_DIRECTORY_${OUTPUTCONFIG} ${CMAKE_SOURCE_DIR}/lib )
    set( CMAKE_LIBRARY_OUTPUT_DIRECTORY_${OUTPUTCONFIG} ${CMAKE_SOURCE_DIR}/lib )
    set( CMAKE_RUNTIME_OUTPUT_DIRECTORY_${OUTPUTCONFIG} ${CMAKE_SOURCE_DIR}/bin )
endforeach( OUTPUTCONFIG CMAKE_CONFIGURATION_TYPES )
]]

# configure_file(config.h.in config.h)
configure_file(
	"${PROJECT_SOURCE_DIR}/config.h.in"
	"${PROJECT_BINARY_DIR}/config.h"
)

message(STATUS "CMAKE_SOURCE_DIR is " ${CMAKE_SOURCE_DIR})
message(STATUS "CMAKE_CURRENT_SOURCE_DIR is " ${CMAKE_CURRENT_SOURCE_DIR})
message(STATUS "PROJECT_SOURCE_DIR is " ${PROJECT_SOURCE_DIR})
message(STATUS "PROJECT_BINARY_DIR is " ${PROJECT_BINARY_DIR})

#[[
#find_package一般用于加载外部库到项目中，并且会加载库的细节信息，共支持两种模式

#mode1: Module, 此模式需访问Find<PackageName>.cmake文件
find_package(<PackageName> 
	[version] 
	[EXACT] 
	[QUIET] 
	[MODULE]
    [REQUIRED] 
	[[COMPONENTS] [components...] ]
    [OPTIONAL_COMPONENTS components...]
    [NO_POLICY_SCOPE])

#mode2: Config, 此模式需访问<lowercasePackageName>-config.cmake or <PackageName>Config.cmake
find_package(<PackageName> 
	[version] 
	[EXACT] 
	[QUIET]
    [REQUIRED] 
	[[COMPONENTS] [components...] ]
    [OPTIONAL_COMPONENTS components...]
    [CONFIG|NO_MODULE]
    [NO_POLICY_SCOPE]
    [NAMES name1 [name2 ...] ]
    [CONFIGS config1 [config2 ...] ]
    [HINTS path1 [path2 ... ] ]
    [PATHS path1 [path2 ... ] ]
    [PATH_SUFFIXES suffix1 [suffix2 ...] ]
    [NO_DEFAULT_PATH]
    [NO_PACKAGE_ROOT_PATH]
    [NO_CMAKE_PATH]
    [NO_CMAKE_ENVIRONMENT_PATH]
    [NO_SYSTEM_ENVIRONMENT_PATH]
    [NO_CMAKE_PACKAGE_REGISTRY]
    [NO_CMAKE_BUILDS_PATH]	# Deprecated; does nothing.
    [NO_CMAKE_SYSTEM_PATH]
    [NO_CMAKE_SYSTEM_PACKAGE_REGISTRY]
    [CMAKE_FIND_ROOT_PATH_BOTH|ONLY_CMAKE_FIND_ROOT_PATH|NO_CMAKE_FIND_ROOT_PATH])
]]
option(USE_MYMATH "Use provided math implementation" ON)

if(USE_MYMATH)
	# 全局包含，向下传递，其下所有的子目录默认也包含了指定目录
	# include_directories(${CMAKE_CURRENT_SOURCE_DIR}/math)
	include_directories("${PROJECT_SOURCE_DIR}/math")

	# binary_dir指存放输出文件目录，默认source_dir
	# add_subdirectory(source_dir [binary_dir] [EXCLUDE_FROM_ALL])
	add_subdirectory(math)

	set(EXTRA_LIBS ${EXTRA_LIBS} MathFunctions)

	# list(APPEND EXTRA_LIBS MathFunctions)
	# list(APPEND EXTRA_INCLUDES ${PROJECT_SOURCE_DIR}/math)
endif(USE_MYMATH)

# 为链接器添加库的搜索路径，此命令调用之后生成的目标才能生效
# link_directories()要放在add_executable()之前
# link_directories([AFTER|BEFORE] directory1 [directory2 ...])
aux_source_directory(. DIR_SRCS)

#[[
#第一种：Normal Executables
#WIN32创建windows下WinMain入口的可执行文件
#MACOSX_BUNDLE创建mac或IOS下GUI可执行程序
add_executable(<name> [WIN32] [MACOSX_BUNDLE]
               [EXCLUDE_FROM_ALL]
               [source1] [source2 ...])

#第二种：Imported Executables
add_executable(<name> IMPORTED [GLOBAL])

#第三种：Alias Executables
add_executable(<name> ALIAS <target>)
]]
# add_executable(demo main.cpp MathFunctions.c)
# add_executable(${PROJECT_NAME} main.cpp MathFunctions.c)
# add_executable(${PROJECT_NAME} main.cpp MathFunctions.c MathFunctions.h)
add_executable(${PROJECT_NAME} ${DIR_SRCS})

# target_include_directories(${PROJECT_NAME} PRIVATE ${PROJECT_SOURCE_DIR}/math)
target_include_directories(${PROJECT_NAME} PRIVATE
	${PROJECT_BINARY_DIR} # config.h目录

	# ${CMAKE_CURRENT_SOURCE_DIR}/math
	# ${EXTRA_INCLUDES}
)

#[[
target_link_libraries(<target>
	<PRIVATE|PUBLIC|INTERFACE> <item>...
	[<PRIVATE|PUBLIC|INTERFACE> <item>...]
	...)
PUBLIC修饰的库或目标会被链接，并成为链接接口的一部分
PRIVATE修饰的目标或库会被链接，但不是链接接口的一部分
INTERFACE修饰的库会追加到链接接口中，但不会用来链接目标文件<target>
]]
# target_link_libraries(${PROJECT_NAME} MathFunctions)
target_link_libraries(${PROJECT_NAME} PUBLIC ${EXTRA_LIBS})

# 指定目标的编译选项
# target_compile_options()

# 安装根目录
# CMAKE_INSTALL_PREFIX

# install(TARGETS ${PROJECT_NAME} DESTINATION bin)
# install(FILES "${PROJECT_BINARY_DIR}/config.h" DESTINATION include)

#[[
#启用测试
enable_testing()

#测试程序是否成功运行
add_test(test_run ${PROJECT_NAME} 5 2)

#测试帮助信息是否可以正常提示
add_test(test_usage ${PROJECT_NAME})
set_tests_properties(test_usage PROPERTIES PASS_REGULAR_EXPRESSION "Usage: .* base exponent")

#测试5的平方
#add_test(test_5_2 ${PROJECT_NAME} 5 2)
#set_tests_properties(test_5_2 PROPERTIES PASS_REGULAR_EXPRESSION "is 25")

#测试10的5次方
#add_test(test_10_5 ${PROJECT_NAME} 10 5)
#set_tests_properties(test_10_5 PROPERTIES PASS_REGULAR_EXPRESSION "is 100000")

#测试2的10次方
#add_test(test_2_10 ${PROJECT_NAME} 2 10)
#set_tests_properties(test_2_10 PROPERTIES PASS_REGULAR_EXPRESSION "is 1024")

#定义一个宏，用来简化测试工作
macro(do_test arg1 arg2 result)
	add_test(test_${arg1}_${arg2} ${PROJECT_NAME} ${arg1} ${arg2})
	set_tests_properties(test_${arg1}_${arg2} PROPERTIES PASS_REGULAR_EXPRESSION ${result})
endmacro(do_test)

#利用do_test宏，测试一系列数据。
do_test(5 2 "is 25")
do_test(10 5 "is 100000")
do_test(2 10 "is 1024")
]]

# mkdir build
# cd build

# #指定MinGW编译器
# cmake -G"MinGW Makefiles" -DUSE_MYMATH=OFF ..
# cmake .. -G "Visual Studio 10 Win64"

#cmake --build .

# #指定编译生成的文件存放目录
# cmake --build .

#[[
.vscode/settings.json
{
	"cmake.generator": "MinGW Makefiles"
}
]]