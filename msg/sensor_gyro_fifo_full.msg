uint64 timestamp                    # time since system start (microseconds)
uint64 timestamp_sample

uint32 device_id                    # unique device ID for the sensor that does not change between power cycles
float32 scale
float32 rotation

float32 temperature                	# temperature in degrees celsius
float32[3] xyz_calibration_offset   # calibration_offset of X, Y, Z body

int16 x_raw                       # raw register values of X
int16 y_raw                       # raw register values of Y
int16 z_raw                       # raw register values of Z

float32[3] xyz_rotated_and_scale_no_cal                   # scaled uncalibrated values of XYZ
float32[3] xyz_rotated_and_cal                  # gyro in the NED XYZ board axis in m/s/s
