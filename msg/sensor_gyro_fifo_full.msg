uint64 timestamp                    # time since system start (microseconds)
uint64 timestamp_sample

uint32 device_id                    # unique device ID for the sensor that does not change between power cycles
float32 scale
int16 rotation

float32 temperature                	# temperature in degrees celsius
float32[3] xyz_calibration_offset   # calibration_offset of X, Y, Z body

int16 x_raw                       # raw register values of X
int16 y_raw                       # raw register values of Y
int16 z_raw                       # raw register values of Z

float32 d00
float32 d01
float32 d02
float32 d10
float32 d11
float32 d12
float32 d20
float32 d21
float32 d22

float32 x                 # angular velocity in the NED X board axis in rad/s
float32 y                 # angular velocity in the NED Y board axis in rad/s
float32 z                 # angular velocity in the NED Z board axis in rad/s
