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

PX4Magnetometer::PX4Magnetometer(uint32_t device_id, enum Rotation rotation) :
	_device_id{device_id},
	_rotation{rotation}
{
	// advertise immediately to keep instance numbering in sync
	_sensor_pub.advertise();
	_sensor_full_pub.advertise();
}

PX4Magnetometer::~PX4Magnetometer()
{
	_sensor_pub.unadvertise();
	_sensor_full_pub.unadvertise();
}

void PX4Magnetometer::set_device_type(uint8_t devtype)
{
	// current DeviceStructure
	union device::Device::DeviceId device_id;
	device_id.devid = _device_id;

	// update to new device type
	device_id.devid_s.devtype = devtype;

	// copy back
	_device_id = device_id.devid;
}

void PX4Magnetometer::update(const hrt_abstime &timestamp_sample, float x, float y, float z)
{

	float x_raw = x;
	float y_raw = y;
	float z_raw = z;

	rotate_3f(_rotation, x, y, z);
	{
		sensor_mag_s report;
		report.timestamp_sample = timestamp_sample;
		report.device_id = _device_id;
		report.temperature = _temperature;
		report.error_count = _error_count;

		// Apply rotation (before scaling)

		const matrix::Vector3f raw_f{x, y, z};

		// Apply range scale and the calibrating offset/scale
		//const matrix::Vector3f val_calibrated{(((raw_f * _scale) - _calibration_offset).emult(_calibration_scale))};

		//report.x = val_calibrated(0);
		//report.y = val_calibrated(1);
		//report.z = val_calibrated(2);

		report.is_external = _external;

		report.x = x * _scale;
		report.y = y * _scale;
		report.z = z * _scale;

		report.timestamp = hrt_absolute_time();
		_sensor_pub.publish(report);
	}

	{
		sensor_mag_full_s report;
		report.timestamp_sample = timestamp_sample;
		report.device_id = _device_id;
		report.temperature = _temperature;
		report.error_count = _error_count;

		// Apply rotation (before scaling)

		const matrix::Vector3f raw_f{x, y, z};

		// Apply range scale and the calibrating offset/scale
		//const matrix::Vector3f val_calibrated{(((raw_f * _scale) - _calibration_offset).emult(_calibration_scale))};

		//report.x = val_calibrated(0);
		//report.y = val_calibrated(1);
		//report.z = val_calibrated(2);
		report.x = x * _scale;
		report.y = y * _scale;
		report.z = z * _scale;

		report.x_raw = x_raw;
		report.y_raw = y_raw;
		report.z_raw = z_raw;

		report.scale = _scale;

		report.is_external = _external;

		report.timestamp = hrt_absolute_time();
		_sensor_full_pub.publish(report);
	}
}

// void PX4Magnetometer::print_status()
// {
// 	PX4_INFO(MAG_BASE_DEVICE_PATH " device instance: %d", _class_device_instance);

// 	PX4_INFO("calibration scale: %.5f %.5f %.5f", (double)_calibration_scale(0), (double)_calibration_scale(1),
// 		 (double)_calibration_scale(2));
// 	PX4_INFO("calibration offset: %.5f %.5f %.5f", (double)_calibration_offset(0), (double)_calibration_offset(1),
// 		 (double)_calibration_offset(2));
// }
