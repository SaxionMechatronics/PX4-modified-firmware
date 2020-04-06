uint64 timestamp	# time since system start (microseconds)
uint32 device_id	# unique device ID for the sensor that does not change between power cycles

uint64 error_count

float32[3] xyz_scaled_and_cal		# magnetic field in the NED XYZ board axis in Gauss

float32 temperature	# temperature in degrees celsius
float32[3] sensitivity

float32[3] xyz_calibration_offset   # calibration_offset of X, Y, Z body
float32[3] xyz_calibration_scale   # calibration_offset of X, Y, Z body

float32 scaling		# scaling from raw to Gauss
int16 x_raw
int16 y_raw
int16 z_raw

float32 d00
float32 d01
float32 d02
float32 d10
float32 d11
float32 d12
float32 d20
float32 d21
float32 d22

bool  is_external	# if true the mag is external (i.e. not built into the board)
