/****************************************************************************
 *
 *   Copyright (c) 2018 PX4 Development Team. All rights reserved.
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


#include "PX4Magnetometer.hpp"

#include <lib/drivers/device/Device.hpp>

PX4Magnetometer::PX4Magnetometer(uint32_t device_id, uint8_t priority, enum Rotation rotation) :
	CDev(nullptr),
	_sensor_mag_pub{ORB_ID(sensor_mag), priority},
	_sensor_mag_full_pub{ORB_ID(sensor_mag_full), priority},
	_rotation{rotation}
{
	_class_device_instance = register_class_devname(MAG_BASE_DEVICE_PATH);

	_sensor_mag_pub.get().device_id = device_id;
	_sensor_mag_pub.get().scaling = 1.0f;

}

PX4Magnetometer::~PX4Magnetometer()
{
	if (_class_device_instance != -1) {
		unregister_class_devname(MAG_BASE_DEVICE_PATH, _class_device_instance);
	}
}

int PX4Magnetometer::ioctl(cdev::file_t *filp, int cmd, unsigned long arg)
{
	switch (cmd) {
	case MAGIOCSSCALE: {
			// Copy offsets and scale factors in
			mag_calibration_s cal{};
			memcpy(&cal, (mag_calibration_s *) arg, sizeof(cal));

			_calibration_offset = matrix::Vector3f{cal.x_offset, cal.y_offset, cal.z_offset};
			_calibration_scale = matrix::Vector3f{cal.x_scale, cal.y_scale, cal.z_scale};

			float misalgn_data[9] = {	cal.d00, 	cal.d01, 	cal.d02,
																cal.d10,	cal.d11,	cal.d12,
																cal.d20,	cal.d21,	cal.d22 };
			_D = matrix::SquareMatrix<float, 3>{misalgn_data};
		}

		return PX4_OK;

	case MAGIOCGSCALE: {
			// copy out scale factors
			mag_calibration_s cal{};
			cal.x_offset = _calibration_offset(0);
			cal.y_offset = _calibration_offset(1);
			cal.z_offset = _calibration_offset(2);
			cal.d00 = _D(0,0);
			cal.d01 = _D(0,1);
			cal.d02 = _D(0,2);
			cal.d10 = _D(1,0);
			cal.d11 = _D(1,1);
			cal.d12 = _D(1,2);
			cal.d20 = _D(2,0);
			cal.d21 = _D(2,1);
			cal.d22 = _D(2,2);
			memcpy((mag_calibration_s *)arg, &cal, sizeof(cal));
		}

		return 0;

	case DEVIOCGDEVICEID:
		return _sensor_mag_pub.get().device_id;

	default:
		return -ENOTTY;
	}
}

void PX4Magnetometer::set_device_type(uint8_t devtype)
{
	// current DeviceStructure
	union device::Device::DeviceId device_id;
	device_id.devid = _sensor_mag_pub.get().device_id;

	// update to new device type
	device_id.devid_s.devtype = devtype;

	// copy back to report
	_sensor_mag_pub.get().device_id = device_id.devid;
}

void PX4Magnetometer::update(hrt_abstime timestamp_sample, float x, float y, float z)
{
	float x_raw = x;
	float y_raw = y;
	float z_raw = z;

	// Apply rotation (before scaling)
	rotate_3f(_rotation, x, y, z);

	{
		sensor_mag_s &report = _sensor_mag_pub.get();
		report.timestamp = timestamp_sample;
		// report.scaling = tmp_r.scaling;

		const matrix::Vector3f raw_f{x, y, z};

		// Apply range scale and the calibrating offset/scale
		// const matrix::Vector3f val_calibrated{(inv(_D) * matrix::Vector3f{x, y, z}).emult(_sensitivity) - _calibration_offset};
		const matrix::Vector3f val_calibrated{(((raw_f * report.scaling) - _calibration_offset))};


		// Raw values (ADC units 0 - 65535)
		report.x_raw = x;
		report.y_raw = y;
		report.z_raw = z;

		report.x = val_calibrated(0);
		report.y = val_calibrated(1);
		report.z = val_calibrated(2);

		_sensor_mag_pub.update();
	}

	{
		sensor_mag_full_s report{};
		sensor_mag_s &tmp_r = _sensor_mag_pub.get();
		report.timestamp = timestamp_sample;
		report.scaling = tmp_r.scaling;

		const matrix::Vector3f raw_f{x, y, z};
		const matrix::Vector3f val_calibrated{(inv(_D) * matrix::Vector3f{x, y, z}).emult(_sensitivity) - _calibration_offset};

		// Raw values (ADC units 0 - 65535)
		report.x_raw = x_raw;
		report.y_raw = y_raw;
		report.z_raw = z_raw;

		report.d00 = _D(0,0);
		report.d01 = _D(0,1);
		report.d02 = _D(0,2);
		report.d10 = _D(1,0);
		report.d11 = _D(1,1);
		report.d12 = _D(1,2);
		report.d20 = _D(2,0);
		report.d21 = _D(2,1);
		report.d22 = _D(2,2);

		for(int i = 0; i < 3; i++){ //for x y and z
			report.xyz_calibration_offset[i] = _calibration_offset(i);
			report.sensitivity[i] = _sensitivity(i);
		}

		_sensor_mag_full_pub.update(report);
	}

}

void PX4Magnetometer::print_status()
{
	PX4_INFO(MAG_BASE_DEVICE_PATH " device instance: %d", _class_device_instance);

	PX4_INFO("calibration offset: %.5f %.5f %.5f", (double)_calibration_offset(0), (double)_calibration_offset(1),
		 (double)_calibration_offset(2));
}
