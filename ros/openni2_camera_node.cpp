/*
 * Copyright (c) 2013, Willow Garage, Inc.
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 *     * Redistributions of source code must retain the above copyright
 *       notice, this list of conditions and the following disclaimer.
 *     * Redistributions in binary form must reproduce the above copyright
 *       notice, this list of conditions and the following disclaimer in the
 *       documentation and/or other materials provided with the distribution.
 *     * Neither the name of the Willow Garage, Inc. nor the names of its
 *       contributors may be used to endorse or promote products derived from
 *       this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 *
 *      Author: Julius Kammerl (jkammerl@willowgarage.com)
 */

#include "openni2_camera/openni2_driver.h"
#include <boost/thread/thread.hpp>
#include <lcm/lcm-cpp.hpp>
#include <openni2_camera/ConciseArgs.hpp>

int main(int argc, char **argv){

  openni2_wrapper::CommandLineConfig cl_cfg;
  cl_cfg.verbose = false;
  cl_cfg.use_jpeg = true;
  cl_cfg.use_zlib = 1;
  cl_cfg.jpeg_quality = 94;
  cl_cfg.image_standalone = false;
  cl_cfg.depth_standalone = false;
  cl_cfg.skip_combined = false;
  cl_cfg.msg_channel = "OPENNI_FRAME";
  cl_cfg.data_skip = 1;

  ConciseArgs parser(argc, argv, "openni2-camera-lcm");
  parser.add(cl_cfg.verbose, "v", "verbose", "Verbose printf");
  //parser.add(cl_cfg.use_jpeg, "j", "use_jpeg", "JPEG-compress RGB images"); // now implicit from quality
  parser.add(cl_cfg.use_zlib, "z", "zlib", "depth compression (0-9), 0=no compression)");
  parser.add(cl_cfg.jpeg_quality, "j", "jpeg", "JPEG quality (1-100), 100=compression");
  parser.add(cl_cfg.image_standalone, "si", "image_standalone", "Publish RGB image individually");
  parser.add(cl_cfg.depth_standalone, "sd", "depth_standalone", "Publish Depth image individually");
  parser.add(cl_cfg.skip_combined, "sc", "skip_combined", "Don't publish RGB-D images message");
  parser.add(cl_cfg.msg_channel, "c", "msg_channel", "LCM Channel");
  parser.add(cl_cfg.data_skip, "d", "data_skip", "Period been images (1 = don't skip, 0 skip all), use to select fps");
  parser.parse();

  if (cl_cfg.jpeg_quality == 100){
    std::cout << "JPEG compression disabled, will not compress\n";
    cl_cfg.use_jpeg = false;
  }

  boost::shared_ptr<lcm::LCM> lcm(new lcm::LCM);
  if(!lcm->good()){
    std::cerr <<"ERROR: lcm is not good()" <<std::endl;
  }

  openni2_wrapper::OpenNI2Driver drv(lcm, cl_cfg);

  while(1==1) {
    boost::this_thread::sleep(boost::posix_time::milliseconds(30));
  }

  return 0;
}

