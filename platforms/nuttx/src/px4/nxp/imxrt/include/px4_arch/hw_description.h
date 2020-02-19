/****************************************************************************
 *
 *   Copyright (C) 2019 PX4 Development Team. All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * 1. Redistributions of source code must retain the above copyright
 *	notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *	notice, this list of conditions and the following disclaimer in
 *	the documentation and/or other materials provided with the
 *	distribution.
 * 3. Neither the name PX4 nor the names of its contributors may be
 *	used to endorse or promote products derived from this software
 *	without specific prior written permission.
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

#pragma once


#include <stdint.h>

#include "hardware/imxrt_flexpwm.h"

#include <px4_platform_common/constexpr_util.h>

#include <board_config.h>
#ifndef CONFIG_ARCH_CHIP_MIMXRT1062DVL6A
# error "This code has only been validated with IMXRT1062. Make sure it is correct before using it on another board."
#endif

/*
 * PWM
 */

namespace PWM
{
enum FlexPWM {
	FlexPWM1 = 0,
	FlexPWM2,
	FlexPWM3,
	FlexPWM4,
};

enum FlexPWMModule {
	PWM1_PWM_A = 0,
	PWM1_PWM_B,
	PWM1_PWM_X,

	PWM2_PWM_A,
	PWM2_PWM_B,

	PWM3_PWM_A,
	PWM3_PWM_B,

	PWM4_PWM_A,
	PWM4_PWM_B,
};

enum FlexPWMSubmodule {
	Submodule0 = 0,
	Submodule1,
	Submodule2,
	Submodule3,
};

struct FlexPWMConfig {
	FlexPWMModule module;
	FlexPWMSubmodule submodule;
};
}

static inline constexpr uint32_t getFlexPWMBaseRegister(PWM::FlexPWM pwm)
{
	switch (pwm) {
	case PWM::FlexPWM1: return IMXRT_FLEXPWM1_BASE;

	case PWM::FlexPWM2: return IMXRT_FLEXPWM2_BASE;

	case PWM::FlexPWM3: return IMXRT_FLEXPWM3_BASE;

	case PWM::FlexPWM4: return IMXRT_FLEXPWM4_BASE;
	}

	return 0;
}

namespace IOMUX
{
enum class Pad {
	GPIO_EMC_00 = 0,
	GPIO_EMC_01 = 1,
	GPIO_EMC_02 = 2,
	GPIO_EMC_03 = 3,
	GPIO_EMC_04 = 4,
	GPIO_EMC_05 = 5,
	GPIO_EMC_06 = 6,
	GPIO_EMC_07 = 7,
	GPIO_EMC_08 = 8,
	GPIO_EMC_09 = 9,
	GPIO_EMC_10 = 10,
	GPIO_EMC_11 = 11,
	GPIO_EMC_12 = 12,
	GPIO_EMC_13 = 13,
	GPIO_EMC_14 = 14,
	GPIO_EMC_15 = 15,
	GPIO_EMC_16 = 16,
	GPIO_EMC_17 = 17,
	GPIO_EMC_18 = 18,
	GPIO_EMC_19 = 19,
	GPIO_EMC_20 = 20,
	GPIO_EMC_21 = 21,
	GPIO_EMC_22 = 22,
	GPIO_EMC_23 = 23,
	GPIO_EMC_24 = 24,
	GPIO_EMC_25 = 25,
	GPIO_EMC_26 = 26,
	GPIO_EMC_27 = 27,
	GPIO_EMC_28 = 28,
	GPIO_EMC_29 = 29,
	GPIO_EMC_30 = 30,
	GPIO_EMC_31 = 31,
	GPIO_EMC_32 = 32,
	GPIO_EMC_33 = 33,
	GPIO_EMC_34 = 34,
	GPIO_EMC_35 = 35,
	GPIO_EMC_36 = 36,
	GPIO_EMC_37 = 37,
	GPIO_EMC_38 = 38,
	GPIO_EMC_39 = 39,
	GPIO_EMC_40 = 40,
	GPIO_EMC_41 = 41,
	GPIO_AD_B0_00 = 42,
	GPIO_AD_B0_01 = 43,
	GPIO_AD_B0_02 = 44,
	GPIO_AD_B0_03 = 45,
	GPIO_AD_B0_04 = 46,
	GPIO_AD_B0_05 = 47,
	GPIO_AD_B0_06 = 48,
	GPIO_AD_B0_07 = 49,
	GPIO_AD_B0_08 = 50,
	GPIO_AD_B0_09 = 51,
	GPIO_AD_B0_10 = 52,
	GPIO_AD_B0_11 = 53,
	GPIO_AD_B0_12 = 54,
	GPIO_AD_B0_13 = 55,
	GPIO_AD_B0_14 = 56,
	GPIO_AD_B0_15 = 57,
	GPIO_AD_B1_00 = 58,
	GPIO_AD_B1_01 = 59,
	GPIO_AD_B1_02 = 60,
	GPIO_AD_B1_03 = 61,
	GPIO_AD_B1_04 = 62,
	GPIO_AD_B1_05 = 63,
	GPIO_AD_B1_06 = 64,
	GPIO_AD_B1_07 = 65,
	GPIO_AD_B1_08 = 66,
	GPIO_AD_B1_09 = 67,
	GPIO_AD_B1_10 = 68,
	GPIO_AD_B1_11 = 69,
	GPIO_AD_B1_12 = 70,
	GPIO_AD_B1_13 = 71,
	GPIO_AD_B1_14 = 72,
	GPIO_AD_B1_15 = 73,
	GPIO_B0_00 = 74,
	GPIO_B0_01 = 75,
	GPIO_B0_02 = 76,
	GPIO_B0_03 = 77,
	GPIO_B0_04 = 78,
	GPIO_B0_05 = 79,
	GPIO_B0_06 = 80,
	GPIO_B0_07 = 81,
	GPIO_B0_08 = 82,
	GPIO_B0_09 = 83,
	GPIO_B0_10 = 84,
	GPIO_B0_11 = 85,
	GPIO_B0_12 = 86,
	GPIO_B0_13 = 87,
	GPIO_B0_14 = 88,
	GPIO_B0_15 = 89,
	GPIO_B1_00 = 90,
	GPIO_B1_01 = 91,
	GPIO_B1_02 = 92,
	GPIO_B1_03 = 93,
	GPIO_B1_04 = 94,
	GPIO_B1_05 = 95,
	GPIO_B1_06 = 96,
	GPIO_B1_07 = 97,
	GPIO_B1_08 = 98,
	GPIO_B1_09 = 99,
	GPIO_B1_10 = 100,
	GPIO_B1_11 = 101,
	GPIO_B1_12 = 102,
	GPIO_B1_13 = 103,
	GPIO_B1_14 = 104,
	GPIO_B1_15 = 105,
	GPIO_SD_B0_00 =  106,
	GPIO_SD_B0_01 =  107,
	GPIO_SD_B0_02 =  108,
	GPIO_SD_B0_03 =  109,
	GPIO_SD_B0_04 =  110,
	GPIO_SD_B0_05 =  111,
	GPIO_SD_B1_00 =  112,
	GPIO_SD_B1_01 =  113,
	GPIO_SD_B1_02 =  114,
	GPIO_SD_B1_03 =  115,
	GPIO_SD_B1_04 =  116,
	GPIO_SD_B1_05 =  117,
	GPIO_SD_B1_06 =  118,
	GPIO_SD_B1_07 =  119,
	GPIO_SD_B1_08 =  120,
	GPIO_SD_B1_09 =  121,
	GPIO_SD_B1_10 =  122,
	GPIO_SD_B1_11 =  123,
};

}

