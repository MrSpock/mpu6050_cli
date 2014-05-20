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

int i2c_mp6050;
char *i2c_mp6050name = "/dev/i2c-2";
int mpu6050_addr = MPU6050_ADDR_A0_L;
//unsigned char io_buffer[10] = {0};

void mpu6050_init(void) {
if ((i2c_mp6050 = open(i2c_mp6050name, O_RDWR)) < 0) {
    /* ERROR HANDLING: you can check errno to see what went wrong */
    perror("Failed to open the i2c bus");
    exit(EXIT_FAILURE);
}
printf("Trying to connect to MPU6050 at address 0x%x\n",mpu6050_addr);
if (ioctl(i2c_mp6050, I2C_SLAVE, mpu6050_addr) < 0) {
    printf("Failed to acquire bus access and/or talk to slave at address 0x%x.\n",mpu6050_addr);
    /* ERROR HANDLING; you can check errno to see what went wrong */
    exit(1);
}

}

void mpu6050_deinit(void) {
    close(i2c_mp6050);
}
void mpu6050_write_register(uint8_t reg,uint8_t value) {
    uint8_t data[2];
    data[0] = reg;
    data[1] = value;
    if(write(i2c_mp6050,data,2) != 2) {
        perror("mpu6050_write_register write error");
    }
}

uint8_t mpu6050_read_register(uint8_t reg) {
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

uint16_t mpu6050_read_register_pair(uint8_t reg) {
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

int main(int argc, char *argv[]) 
{
uint8_t whoami = 10 ;
uint8_t costam;
float temp = 0;
mpu6050_init();
// disable SLEEP mode
mpu6050_write_register(MPU6050_PWR_MGMT_1,0x0);
whoami = mpu6050_read_register(MPU6050_WHO_AM_I);
costam = mpu6050_read_register(MPU6050_PWR_MGMT_1);
temp =  ((float) (mpu6050_read_register_pair(MPU6050_TEMP_OUT_H)/340)) + 36.53;
printf("MPU6050_WHO_AM_I register value: 0x%x\n",whoami);
printf("COSTAM:%i\n",costam);
printf("Temperature: %.2f\n",temp);
mpu6050_deinit();
return EXIT_SUCCESS;
}

