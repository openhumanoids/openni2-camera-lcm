# openni2-camera-lcm
Openni2 Camera LCM driver

  openni2-camera-lcm -j -z -c OPENNI_FRAME

This driver is a direct conversion of the ROS driver, 
except publishing LCM data. It should be used for all
Primesense/Asus Xtion camera, with USB3.0.
Snapshot for ROS driver used:


For Kinect and older Asus cameras and USB2.0 devices
use kinect-openni-lcm found in kinect-lcm pod.

This driver outputs bot_core::images_t messages
which are also output by stereo cameras e.g. multisense.


# TODO:

- Support rate limiting
- Check the time synchronization of the rgb vrs depth (this may need fixing)
- Support different types of data e.g. 60Hz
