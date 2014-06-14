
#include <stdint.h>

#ifndef MPU6050_H
#define MPU6050_H



#define MPU6050_ADDR_A0_L 0x68
#define MPU6050_ADDR_A0_H 0x69

/*
MPU6050 register definitions
 */

#define MPU6050_INT_STATUS 0x3A
#define MPU6050_ACCEL_XOUT_H 0x3B
#define MPU6050_ACCEL_XOUT_L 0x3C
#define MPU6050_ACCEL_YOUT_H 0x3D
#define MPU6050_ACCEL_YOUT_L 0x3E
#define MPU6050_ACCEL_ZOUT_H 0x3F
#define MPU6050_ACCEL_ZOUT_L 0x40
#define MPU6050_TEMP_OUT_H 0x41
#define MPU6050_TEMP_OUT_L 0x42
#define MPU6050_GYRO_XOUT_H 0x43
#define MPU6050_GYRO_XOUT_L 0x44
#define MPU6050_GYRO_YOUT_H 0x45
#define MPU6050_GYRO_YOUT_L 0x46
#define MPU6050_GYRO_ZOUT_H 0x47
#define MPU6050_GYRO_ZOUT_L 0x48

#define MPU6050_I2C_SLV0_DO 0x63
#define MPU6050_I2C_SLV1_DO 0x64
#define MPU6050_I2C_SLV2_DO 0x65
#define MPU6050_I2C_SLV3_DO 0x66
#define MPU6050_PWR_MGMT_1 0x6B
#define MPU6050_PWR_MGMT_2 0x6C
#define MPU6050_FIFO_COUNTH 0x72
#define MPU6050_FIFO_COUNTL 0x73

#define MPU6050_WHO_AM_I 0x75

#define ACCEL_CONFIG 0x1C

#define AF_SEL_ACCEL_RANGE_2G 0 << 3
#define AF_SEL_ACCEL_RANGE_4G 1 << 3
#define AF_SEL_ACCEL_RANGE_8G 2 << 3
#define AF_SEL_ACCEL_RANGE_16G 3 << 3


/*
structure holding results data
 */
struct MPU6050 {
    float accel_x;
    float accel_y;
    float accel_z;
    int16_t gyro_x;
    int16_t gyro_y;
    int16_t gyro_z;
    float temp;
};


// function definitions
/*
Initialize connection to IC
open i2c bus and set I2C_SLAVE mode to device
 */
int mpu6050_init(int bus_no,int mpu6050_addr);

/*
    deinitialize access to device
    close i2c-bus
 */
void mpu6050_deinit(int bus_no);

/*
write one byte to given register
 */
void mpu6050_write_register(int fd,uint8_t reg,uint8_t value) ;

/*
    read single byte from given register
 */
uint8_t mpu6050_read_register(int fd, uint8_t reg);

/*
    read two bytes from register and return as single 16 bit value
    first byte is msb and second is lsb in returned 16bit integer
 */
int16_t mpu6050_read_register_pair(int fd,uint8_t reg);

/*
set accelerometer max range
argument is one of AF_SEL_ACCEL_RANGE_ values
setting range have impact on accelerometer resolution !
 */
uint16_t mpu6050_select_range(int i2c_handle,uint16_t range);

/*
    bring device from sleep mode
*/
void mpu6050_power_on(int i2c_handle);

/*
get temperature value
 */
float mpu6050_get_temperature(int h2c_handle);

#endif
