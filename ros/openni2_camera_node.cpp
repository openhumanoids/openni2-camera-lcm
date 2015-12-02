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
int main(int argc, char **argv){

//  ros::init(argc, argv, "openni2_camera");
//  ros::NodeHandle n;
//  ros::NodeHandle pnh("~");

  std::cout << "a\n";
  openni2_wrapper::OpenNI2Driver drv(1);//n, pnh);
  std::cout << "b\n";

    boost::this_thread::sleep(boost::posix_time::milliseconds(10000));
//  ros::spin();
//  while (1==1);

  return 0;
}

/*





#include <stdio.h>
#include <stdlib.h>
#include <poll.h>
#include <lcmtypes/bot_core.h>
#include <bot_core/lcm_util.h>
#include <bot_core/signal_pipe.h>

#include "openni2_camera/openni2_driver.h"
//#include "kinectOpenniLCM.h"

typedef struct {
    lcm_t *lcm;
    openni2_wrapper::OpenNI2Driver * kinectOpenniLCM;
} app_t;

static void
app_run(app_t *self)
{
  GMainLoop *mainloop = g_main_loop_new(NULL, FALSE);
  bot_glib_mainloop_attach_lcm(self->lcm);
  bot_signal_pipe_glib_quit_on_kill(mainloop);
  
  g_main_loop_run(mainloop);
  
  printf("main: exiting\n");
  
  g_main_loop_unref(mainloop);
}

static void
app_destroy(app_t *self)
{
  delete self->kinectOpenniLCM;
  lcm_destroy(self->lcm);
  free(self);
}

int main(int argc, char **argv)
{
    setlinebuf(stdout);    

    g_thread_init(NULL);

    app_t *app = (app_t*) calloc(1, sizeof(app_t));
    app->lcm = bot_lcm_get_global(NULL);
    app->kinectOpenniLCM = new openni2_wrapper::OpenNI2Driver(0);

    app_run(app);
    app_destroy(app);

    return 0;
}


*/
