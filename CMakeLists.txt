cmake_minimum_required(VERSION 2.8.3)

# pull in the pods macros. See cmake/pods.cmake for documentation
set(POD_NAME openni2)
include(cmake/pods.cmake)

# automatically build LCM types.  This also defines a number of CMake
# variables, see cmake/lcmtypes.cmake for details
include(cmake/lcmtypes.cmake)
lcmtypes_build()

#############################################
project(openni2)

############### System Find #################
find_package(Boost REQUIRED COMPONENTS system thread)

find_package(PkgConfig)

pkg_check_modules(PC_OPENNI2 libopenni2)
if (NOT PC_OPENNI2_FOUND)
  pkg_check_modules(PC_OPENNI2 REQUIRED openni2)
endif()

include_directories(include 
                    ${Boost_INCLUDE_DIRS}
                    ${PC_OPENNI2_INCLUDE_DIRS}
                    )

link_directories(${PC_OPENNI2_LIBRARY_DIRS})

############### Main Library #################
add_library(openni2_wrapper
   src/openni2_convert.cpp
   src/openni2_device.cpp 
   src/openni2_device_info.cpp 
   src/openni2_timer_filter.cpp 
   src/openni2_frame_listener.cpp
   src/openni2_device_manager.cpp
   src/openni2_exception.cpp
   src/openni2_video_mode.cpp
)
target_link_libraries(openni2_wrapper  ${PC_OPENNI2_LIBRARIES} ${Boost_LIBRARIES} )
pods_install_libraries(openni2_wrapper)

add_executable(test-wrapper test/test_wrapper.cpp )
target_link_libraries(test-wrapper openni2_wrapper ${Boost_LIBRARIES})

############### Wrapper Library #################
add_library(openni2_driver_lib src/openni2_driver.cpp)
pods_install_libraries(openni2_wrapper)
target_link_libraries(openni2_driver_lib openni2_wrapper  ${Boost_LIBRARIES} )
pods_install_libraries(openni2_driver_lib)
#add_dependencies(openni2_driver_lib ${PROJECT_NAME}_gencfg)

############### Main Driver #################
add_executable(openni2-camera-lcm   ros/openni2_camera_node.cpp)
target_link_libraries(openni2-camera-lcm openni2_driver_lib ${catkin_LIBRARIES} ${Boost_LIBRARIES} )
add_dependencies(openni2-camera-lcm ${PROJECT_NAME}_gencfg ${PROJECT_NAME}_generate_messages_cpp)
pods_use_pkg_config_packages ( openni2-camera-lcm lcm bot2-core glib-2.0)
pods_install_executables(openni2-camera-lcm)

############### Auxilary Processes #################
add_executable(list-devices src/list_devices.cpp)
target_link_libraries(list-devices openni2_wrapper)
pods_install_executables(list-devices)

add_executable(usb-reset src/usb_reset.c)
pods_install_executables(usb-reset)
