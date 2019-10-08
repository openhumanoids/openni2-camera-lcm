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

option(INSTALL_BOT_SPY "Install a bot-spy script to launch lcm-spy with this project's lcmtypes." ON)

if(APPLE)
  # on mac you can install openni2 with homebrew but it doesn't come with a pkg-config .pc file
  set(PC_OPENNI2_INCLUDE_DIRS /usr/local/include/ni2)
  set(PC_OPENNI2_LIBRARY_DIRS /usr/local/lib/ni2)
  set(PC_OPENNI2_LIBRARIES OpenNI2 jpeg lcm)
else()
  pkg_check_modules(PC_OPENNI2 libopenni2)
  if (NOT PC_OPENNI2_FOUND)
    pkg_check_modules(PC_OPENNI2 REQUIRED openni2)
  endif()
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
   src/jpeg-utils-ijg.c
)
target_link_libraries(openni2_wrapper  ${PC_OPENNI2_LIBRARIES} ${Boost_LIBRARIES} jpeg)
pods_install_libraries(openni2_wrapper)

add_executable(test-wrapper test/test_wrapper.cpp )
target_link_libraries(test-wrapper openni2_wrapper ${Boost_LIBRARIES})

############### Wrapper Library #################
add_library(openni2_driver_lib src/openni2_driver.cpp)
pods_install_libraries(openni2_wrapper)
target_link_libraries(openni2_driver_lib openni2_wrapper  ${Boost_LIBRARIES} jpeg z)
pods_install_libraries(openni2_driver_lib)
#add_dependencies(openni2_driver_lib ${PROJECT_NAME}_gencfg)

############### Main Driver #################
add_executable(openni2-camera-lcm   ros/openni2_camera_node.cpp)
target_link_libraries(openni2-camera-lcm openni2_driver_lib ${catkin_LIBRARIES} ${Boost_LIBRARIES} )
add_dependencies(openni2-camera-lcm ${PROJECT_NAME}_gencfg ${PROJECT_NAME}_generate_messages_cpp)
find_package(lcm QUIET)
if(lcm_FOUND)
  target_link_libraries(openni2-camera-lcm lcm)
else()
  pods_use_pkg_config_packages ( openni2-camera-lcm lcm)
endif()
pods_install_executables(openni2-camera-lcm)

############### Auxilary Processes #################
add_executable(list-devices src/list_devices.cpp)
target_link_libraries(list-devices openni2_wrapper)
pods_install_executables(list-devices)


if(NOT APPLE)
  # this program includes a linux specific header
  add_executable(usb-reset src/usb_reset.c)
  pods_install_executables(usb-reset)
endif()


# bot-spy clone, needed to decode lcmtypes
set(script_name bot-spy)
file(WRITE ${CMAKE_CURRENT_BINARY_DIR}/${script_name} 
    "#!/bin/sh\n"
    "CLASSPATH=`PKG_CONFIG_PATH=PKG_CONFIG_PATH:${CMAKE_INSTALL_PREFIX}/lib/pkgconfig pkg-config --variable=classpath lcm-java`\n"
    "for d in . .. ../.. ../../.. ../../../.. \"${CMAKE_INSTALL_PREFIX}\"; do\n"
    #    "for d in \"${CMAKE_INSTALL_PREFIX}\"; do\n"
    "  if [ -d $d/share/java ]; then\n"
    "    jd=$d/share/java\n"
    "    echo Checking $jd\n"
    "    for f in $jd/lcmtypes_*.jar $jd/lcmspy_plugins_*.jar; do\n"
    "      if [ -e $f ]; then\n"
    "        echo \"   Found $f\"\n"
    "        CLASSPATH=\$CLASSPATH:\$f\n"
    "      fi\n"
    "    done\n"
    "  fi\n"
    "done\n"
    "exec java -ea -cp \$CLASSPATH lcm.spy.Spy $*\n")

# install it...
if(INSTALL_BOT_SPY)
  install(PROGRAMS ${CMAKE_CURRENT_BINARY_DIR}/${script_name} DESTINATION bin)
endif()
