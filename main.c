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
#include <signal.h>
#include "mpu6050.h"

#define MPU6050_ADDR 0x68
#define I2C_BUS 2

int mpu6050_addr = MPU6050_ADDR_A0_L;
int fd_mpu6050;
struct MPU6050 *sensor;

int main(int argc, char *argv[])
{

    int whoami = 0;
    // data storage struct
    //struct MPU6050 *sensor;
    sensor = malloc(sizeof(struct MPU6050));
    int sensitivity;
    // initialize connection to MPU6050 and get fd for communication
    if((fd_mpu6050 = mpu6050_init(I2C_BUS,MPU6050_ADDR)) < 0) goto cleanup;

    // select sensivity_range
    sensitivity = mpu6050_select_range(fd_mpu6050,AF_SEL_ACCEL_RANGE_8G);

    // bring it online :)
    mpu6050_power_on(fd_mpu6050);
    // check if we are connected to proper chip - should return it's I2C address
    whoami = mpu6050_read_register(fd_mpu6050,MPU6050_WHO_AM_I);
    fprintf(stderr,"Sensor ID: 0x%x\n",whoami);

    // collect and print accel data every 200ms
    while(1) {
          //sensor->temp = ((float) (mpu6050_read_register_pair(fd_mpu6050,MPU6050_TEMP_OUT_H)/340)) + 36.53;
          sensor->temp = mpu6050_get_temperature(fd_mpu6050);
          sensor->accel_x = ((float) (mpu6050_read_register_pair(fd_mpu6050,MPU6050_ACCEL_XOUT_H))) / sensitivity;
          sensor->accel_y = ((float) (mpu6050_read_register_pair(fd_mpu6050,MPU6050_ACCEL_YOUT_H))) / sensitivity;
          sensor->accel_z = ((float) (mpu6050_read_register_pair(fd_mpu6050,MPU6050_ACCEL_ZOUT_H))) / sensitivity;
        printf("TEMP:%.2f\tX_ACCEL:%.3f\tY_ACCEL: %.3f\tZ_ACCEL:%.3f\n",sensor->temp,sensor->accel_x,sensor->accel_y,sensor->accel_z);
        usleep(200000);
    }
    goto cleanup;
cleanup:
    //printf("Cleaning up\n");
    mpu6050_deinit(I2C_BUS);
    free(sensor);
    return EXIT_SUCCESS;
}

