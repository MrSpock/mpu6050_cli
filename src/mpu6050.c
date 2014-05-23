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
#include "lsquaredc.h"

int mpu6050_init(int bus_no,int mpu6050_addr) {
    int i2c_mp6050;
    i2c_mp6050 = i2c_open(bus_no);
// if ((i2c_mp6050 = open(i2c_dev, O_RDWR)) < 0) {
//     /* ERROR HANDLING: you can check errno to see what went wrong */
//     perror("Failed to open the i2c bus");
//     exit(EXIT_FAILURE);
// }
// printf("Trying to connect to MPU6050 at address 0x%x\n",mpu6050_addr);
 if (ioctl(i2c_mp6050, I2C_SLAVE, mpu6050_addr) < 0) {
     printf("Failed to acquire bus access and/or talk to slave at address 0x%x.\n",mpu6050_addr);
     /* ERROR HANDLING; you can check errno to see what went wrong */
     exit(1);
}
return i2c_mp6050;
}

void mpu6050_deinit(int i2c_no) {
    i2c_close(i2c_no);
}

void mpu6050_write_register(int fd, uint8_t reg,uint8_t value) {
    uint8_t data[2];
    data[0] = reg;
    data[1] = value;
    if(write(fd,data,2) != 2) {
        perror("mpu6050_write_register write error");
    }
}

uint8_t mpu6050_read_register(int i2c_mp6050, uint8_t reg) {
    unsigned char data[3];
    data[0] = reg;
    if(write(i2c_mp6050, data,1) != 1) {
        perror("Error sending read request via i2c\n");
        return -1;
    }
        if(read(i2c_mp6050,data,1) != 1) {
            perror("Error getting read request via i2c\n");
    }
    return data[0];
}

int16_t mpu6050_read_register_pair(int i2c_mp6050,uint8_t reg) {
    uint8_t data[3];
    data[0] = reg;
    if(write(i2c_mp6050, data,1) != 1) {
        perror("Error sending read request via i2c\n");
        return -1;
    }
        if(read(i2c_mp6050,data,2) != 2) {
            perror("Error getting read request via i2c\n");
    }
    return (data[0]<<8)|data[1];
}

uint16_t mpu6050_select_range(int i2c_handle,uint16_t range)
{
    int sensitivity;
    mpu6050_write_register(i2c_handle,ACCEL_CONFIG,range);
    // confirm values for debug
    fprintf(stderr,"D mpu6050_select_range(): 0x%x\n",mpu6050_read_register(i2c_handle,ACCEL_CONFIG));
    switch(range) {
        case AF_SEL_ACCEL_RANGE_2G:
        sensitivity = 0x4000;
        break;
        case AF_SEL_ACCEL_RANGE_4G:
        sensitivity = 0x2000;
        break;
        case AF_SEL_ACCEL_RANGE_8G:
        sensitivity = 0x1000;
        break;
        case AF_SEL_ACCEL_RANGE_16G:
        sensitivity = 0x800;
        break;
        default:
        sensitivity = 1;
        break;
    }
    return sensitivity;
};

