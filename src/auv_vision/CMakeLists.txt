cmake_minimum_required(VERSION 2.8.3)
project(auv_vision)

## Compile as C++11, supported in ROS Kinetic and newer
add_compile_options(-std=c++11)

## Helps with finding OpenCV's cmake configs. Uncomment to use locally built OpenCV.
## set(OpenCV_DIR /usr/local/share/OpenCV)

## Find catkin macros and libraries
## if COMPONENTS list like find_package(catkin REQUIRED COMPONENTS xyz)
## is used, also find other catkin packages
find_package(catkin REQUIRED COMPONENTS
  roscpp
  rospy
  std_msgs
  genmsg
  cv_bridge
  sensor_msgs
  image_transport
  dynamic_reconfigure
  auv_common
)

## Find OpenCV
find_package(OpenCV)
## For locally built OpenCV, comment previous line and uncomment following:
## find_package(OpenCV REQUIRED HINTS ${OpenCV_DIR} NO_DEFAULT_PATH)

################################################
## Declare ROS messages, services and actions ##
################################################

## Generate messages in the 'msg' folder
#add_message_files(
#  FILES
#)

## Generate added messages and services with any dependencies listed here
#generate_messages(
#  DEPENDENCIES
#  std_msgs
#)

################################################
## Declare ROS dynamic reconfigure parameters ##
################################################

generate_dynamic_reconfigure_options(
        cfg/gate_locator.cfg
        cfg/drums_locator.cfg
)


###################################
## catkin specific configuration ##
###################################

catkin_package(
  CATKIN_DEPENDS roscpp rospy std_msgs auv_common
#  INCLUDE_DIRS include
#  LIBRARIES auv_vision
#  CATKIN_DEPENDS roscpp rospy std_msgs
#  DEPENDS system_lib
)

###########
## Build ##
###########

include_directories(
  ## Uncomment to use locally built OpenCV:
  ## ${OpenCV_INCLUDE_DIRS}
  ${catkin_INCLUDE_DIRS}
  "include"
)

add_executable(gate_locator
        src/gate_locator.cpp
        include/common/AbstractImageConverter.h
        src/common/AbstractImageConverter.cpp
        include/gate/GateDescriptor.h
        src/gate/GateDescriptor.cpp
        include/util/ImgprocPipeline.h
        src/util/ImgprocPipeline.cpp
        include/util/ImgprocUtil.h
        src/util/ImgprocUtil.cpp src/gate/IGateDetector.cpp src/gate/AngleGateDetector.cpp src/gate/VerticalGateDetector.cpp)
target_link_libraries(gate_locator ${catkin_LIBRARIES}
        ${OpenCV_LIBS})
add_dependencies(gate_locator auv_common_generate_messages_cpp ${PROJECT_NAME}_gencfg)

add_executable(tile_locator
        src/tile_locator.cpp
        include/common/AbstractImageConverter.h
        src/common/AbstractImageConverter.cpp
        include/util/ImgprocPipeline.h
        src/util/ImgprocPipeline.cpp
        include/util/ImgprocUtil.h
        src/util/ImgprocUtil.cpp)
target_link_libraries(tile_locator
        ${catkin_LIBRARIES}
        ${OpenCV_LIBRARIES})

add_executable(drums_locator
        src/drums_locator.cpp
        include/common/AbstractImageConverter.h
        src/common/AbstractImageConverter.cpp
        include/mat/MatDetector.h
        src/mat/MatDetector.cpp
        include/drums/DrumDetector.h
        src/drums/DrumDetector.cpp
        include/mat/MatDescriptor.h
        src/mat/MatDescriptor.cpp
        include/drums/DrumDescriptor.h
        src/drums/DrumDescriptor.cpp
        include/util/ImgprocPipeline.h
        src/util/ImgprocPipeline.cpp
        include/util/ImgprocUtil.h
        src/util/ImgprocUtil.cpp include/gate/IGateDetector.h include/gate/VerticalGateDetector.h include/gate/AngleGateDetector.h)
target_link_libraries(drums_locator
        ${catkin_LIBRARIES}
        ${OpenCV_LIBRARIES})
add_dependencies(drums_locator auv_common_generate_messages_cpp)

add_executable(flare_locator
        src/flare_locator.cpp
        include/common/AbstractImageConverter.h
        src/common/AbstractImageConverter.cpp
        include/util/ImgprocPipeline.h
        src/util/ImgprocPipeline.cpp
        include/util/ImgprocUtil.h
        src/util/ImgprocUtil.cpp)
target_link_libraries(flare_locator
        ${catkin_LIBRARIES}
        ${OpenCV_LIBRARIES})
add_dependencies(flare_locator auv_common_generate_messages_cpp)


add_executable(video_recorder
        src/video_recorder.cpp
        include/util/ConcurrentQueue.h)
target_link_libraries(video_recorder
        ${catkin_LIBRARIES}
        ${OpenCV_LIBRARIES})

