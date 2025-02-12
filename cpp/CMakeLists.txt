cmake_minimum_required(VERSION 3.5)

# 工程信息
project(region)

set(CMAKE_CXX_STANDARD 17) #强制使用17标准

# 设置头文件目录
include_directories(thirdparty/opencv/include)
include_directories(thirdparty/glog/include)
 
# 设置静态库目录
link_directories(thirdparty/opencv/lib)
link_directories(thirdparty/glog/lib)

set(EXENAME PatternGenerator)
add_executable(${EXENAME}  
    head.h
    main.cpp
    EncoderGray.cpp
    EncoderPS.cpp
)
target_link_libraries(${EXENAME} 
    glog
    opencv_world401
)

# set(JSON_ROOT_DIR ${PROJECT_SOURCE_DIR}/json)
# set(JSON_SOURCE_DIR ${JSON_ROOT_DIR}/cpp)
# set(JSON_INCLUDES_DIR ${JSON_ROOT_DIR}/include)
# include_directories(${JSON_INCLUDES_DIR})

# set(LIBNAME cvCal)
# add_library(${LIBNAME} 
    # SHARED
    # region.cpp
    # region.h
    # cal_bright.cpp
    # ${JSON_SOURCE_DIR}/json_reader.cpp
    # ${JSON_SOURCE_DIR}/json_writer.cpp
    # ${JSON_SOURCE_DIR}/json_value.cpp
# )
# target_link_libraries (${LIBNAME} 
    # opencv_world401
# )

# set(LIBNAME cvCal_test)
# add_executable(${LIBNAME} 
    # region.cpp
    # region.h
    # cal_bright.cpp
    # ${JSON_SOURCE_DIR}/json_reader.cpp
    # ${JSON_SOURCE_DIR}/json_writer.cpp
    # ${JSON_SOURCE_DIR}/json_value.cpp
# )
# target_link_libraries (${LIBNAME} 
    # opencv_world401
# )