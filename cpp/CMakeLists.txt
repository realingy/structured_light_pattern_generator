cmake_minimum_required( VERSION 3.5)

project(structured_light_pattern_generator)

# include_directories(${CMAKE_SOURCE_DIR}/..)

set(CMAKE_CONFIGRATION_TYPES "Release" CACHE STRING "" FORCE)

#add_definitions( -D_HAS_EXCEPTIONS=0 )

set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# 设置头文件目录
include_directories(thirdparty/glog/include)
include_directories(thirdparty/opencv/include)
# include_directories(thirdparty/dsStream/include)
 
# 设置静态库目录
link_directories(thirdparty/glog/lib)
link_directories(thirdparty/opencv/lib)
# link_directories(thirdparty/dsStream/lib)

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
