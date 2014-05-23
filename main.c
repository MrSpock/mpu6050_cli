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


int I2C_BUS = 2;
//char *i2c_mp6050name = "/dev/i2c-2";
int mpu6050_addr = MPU6050_ADDR_A0_L;
int fd_mpu6050;

int main(int argc, char *argv[]) 
{
    int whoami = 0;
    struct MPU6050 *sensor;
    sensor = malloc(sizeof(struct MPU6050));
    int sensitivity;
    fd_mpu6050 = mpu6050_init(I2C_BUS,mpu6050_addr);
    sensitivity = mpu6050_select_range(fd_mpu6050,AF_SEL_ACCEL_RANGE_8G);
    fprintf(stderr,"I2C handle: %i\n", fd_mpu6050);
    // SELECT ACCEL RANGE
    //mpu6050_select_range(fd_mpu6050,AF_SEL_ACCEL_RANGE_4G);
    // disable SLEEP mode
    mpu6050_write_register(fd_mpu6050,MPU6050_PWR_MGMT_1,0x0);
    whoami = mpu6050_read_register(fd_mpu6050,MPU6050_WHO_AM_I);
    fprintf(stderr,"Sensor ID: 0x%x\n",whoami);
    while(1) {
          sensor->temp = ((float) (mpu6050_read_register_pair(fd_mpu6050,MPU6050_TEMP_OUT_H)/340)) + 36.53;  
          sensor->accel_x = ((float) (mpu6050_read_register_pair(fd_mpu6050,MPU6050_ACCEL_XOUT_H))) / sensitivity;
          sensor->accel_y = ((float) (mpu6050_read_register_pair(fd_mpu6050,MPU6050_ACCEL_YOUT_H))) / sensitivity; 
          sensor->accel_z = ((float) (mpu6050_read_register_pair(fd_mpu6050,MPU6050_ACCEL_ZOUT_H))) / sensitivity;
    //    temp =  ((float) (mpu6050_read_register_pair(MPU6050_TEMP_OUT_H)/340)) + 36.53;
         //printf("%i\n",mpu6050_read_register_pair(fd_mpu6050,MPU6050_ACCEL_XOUT_H));
        printf("TEMP:%.2f\tX_ACCEL:%.3f\tY_ACCEL: %.3f\tZ_ACCEL:%.3f\n",sensor->temp,sensor->accel_x,sensor->accel_y,sensor->accel_z);
        usleep(200000);
    }

    mpu6050_deinit(I2C_BUS);
    free(sensor);
    return EXIT_SUCCESS;
}

