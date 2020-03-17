/****************************************************************************
 *
 *   Copyright (c) 2012-2015 PX4 Development Team. All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in
 *    the documentation and/or other materials provided with the
 *    distribution.
 * 3. Neither the name PX4 nor the names of its contributors may be
 *    used to endorse or promote products derived from this software
 *    without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
 * FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE
 * COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
 * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
 * BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS
 * OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED
 * AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN
 * ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 *
 ****************************************************************************/

/**
 * @file px4_mavlink_debug.cpp
 * Debug application example for PX4 autopilot
 *
 * @author Example User <mail@example.com>
 */
extern "C" __EXPORT int imu_to_csv_main(int argc, char *argv[]);
#include <px4_platform_common/px4_config.h>
#include <px4_platform_common/tasks.h>
#include <px4_platform_common/posix.h>
#include <parameters/param.h>
#include <uORB/uORB.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>


using namespace std;
static const char *csv_file = PX4_ROOTFSDIR"/fs/microsd/imu_params.csv";

const char* getfield(char* line, int num)
{
    const char* tok;
    for (tok = strtok(line, ";");
            tok && *tok;
            tok = strtok(NULL, ";\n"))
    {
        if (!--num)
            return tok;
    }
    return NULL;
}

int imu_to_csv_main(int argc, char *argv[])
{
	 FILE *stream = fopen(csv_file,"r");
   PX4_INFO("Still alive");
   char cwd[1024];
  if (getcwd(cwd, sizeof(cwd)) != NULL) {
      PX4_INFO("Current working dir: %s\n", cwd);
  }

	 char line[1024];

    PX4_INFO("IM DOING SOMETHING!!!");
	  while (fgets(line, 1024, stream)){
      PX4_INFO("IM DOING SOMETHING!!!");
			char* tmp = strdup(line);
      PX4_INFO("Field 1 would be %s\n", getfield(tmp, 0));
			free(tmp);
	}

  return 0;
}
