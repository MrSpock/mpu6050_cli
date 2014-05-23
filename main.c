#include <errno.h>
#include <string.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <unistd.h>
#include <linux/i2c-dev.h>
#include <sys/ioctl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include "mpu6050.h"


char *i2c_mp6050name = "/dev/i2c-2";
int mpu6050_addr = MPU6050_ADDR_A0_L;
int fd_mpu6050;

int main(int argc, char *argv[]) 
{
    struct MPU6050 *sensor;
    sensor = malloc(sizeof(struct MPU6050));
    uint8_t whoami = 10 ;
    uint8_t costam;
    float temp = 0;
    fd_mpu6050 = mpu6050_init(i2c_mp6050name,mpu6050_addr);
    // disable SLEEP mode
    mpu6050_write_register(fd_mpu6050,MPU6050_PWR_MGMT_1,0x0);
    whoami = mpu6050_read_register(fd_mpu6050,MPU6050_WHO_AM_I);
    costam = mpu6050_read_register(fd_mpu6050,MPU6050_GYRO_XOUT_H);
    temp =  ((float) (mpu6050_read_register_pair(fd_mpu6050,MPU6050_TEMP_OUT_H)/340)) + 36.53;
    printf("MPU6050_WHO_AM_I register value: 0x%x\n",whoami);
    printf("COSTAM:%i\n",costam);
    printf("Temperature: %.2f\n",temp);
    while(1) {
          sensor->temp = ((float) (mpu6050_read_register_pair(fd_mpu6050,MPU6050_TEMP_OUT_H)/340)) + 36.53;  
          sensor->accel_x = ((float) (mpu6050_read_register_pair(fd_mpu6050,MPU6050_ACCEL_XOUT_H))) / 0x4000;
          sensor->accel_y = ((float) (mpu6050_read_register_pair(fd_mpu6050,MPU6050_ACCEL_YOUT_H))) / 0x4000;
          sensor->accel_z = ((float) (mpu6050_read_register_pair(fd_mpu6050,MPU6050_ACCEL_ZOUT_H))) / 0x4000;
    //    temp =  ((float) (mpu6050_read_register_pair(MPU6050_TEMP_OUT_H)/340)) + 36.53;
         //printf("%i\n",mpu6050_read_register_pair(fd_mpu6050,MPU6050_ACCEL_XOUT_H));
        printf("TEMP:\t%.2f\tX_ACCEL:%.2f\tY_ACCEL: %.2f\tZ_ACCEL:%2f\n",sensor->temp,sensor->accel_x,sensor->accel_y,sensor->accel_z);
        usleep(200000);
    }

    mpu6050_deinit(fd_mpu6050);
    free(sensor);
    return EXIT_SUCCESS;
}

