uint64 timestamp          # time since system start (microseconds)
uint64 timestamp_sample

uint32 device_id          # unique device ID for the sensor that does not change between power cycles
float32 scale

uint8 samples             # number of valid samples

float32 temperature	# temperature in degrees celsius

float32[3] xyz_calibration_offset   # calibration_offset of X, Y, Z body
float32[3] xyz_calibration_scale   # calibration_offset of X, Y, Z body

int16 x_raw                       # raw register values of X
int16 y_raw                       # raw register values of Y
int16 z_raw                       # raw register values of Z

int16[3]   xyz_scaled_no_cal                   # scaled uncalibrated values of X
float32[3] xyz_scaled_and_cal                # acceleration in the NED X board axis in m/s/s
