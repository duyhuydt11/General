#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include "thu_vien.h"
#include <errno.h>



// global variable
int data_read;
int fd;

// hàm init
int ina219_Init(int a){
    int value_conf = 7; 
    int value_calib = 0x4096;
    // mở file
    fd = open(DEVICE_PATH, O_RDWR);
    if (fd < 0) {
        perror("Failed to open the device\n");
        return errno;
    }

    // Khởi tạo cho thanh ghi config
    if (ioctl(fd, INA219_IOCTL_WRITE_CONFIG, &value_conf) < 0) { //ktra lại giá trị
        perror("Failed to config Configuration register\n");
        close(fd);
        return errno;
    }
    printf("Succeed config Configuration register\n");

    if (ioctl(fd, INA219_IOCTL_WRITE_CALIBRATION, &value_calib) < 0) {
        perror("Failed to config Calibration register\n");
        close(fd);
        return errno;
    }
    printf("Succeed config Calibration register\n");
}

// hàm đọc giá trị thanh ghi cụ thể
int Read_SVoltage(){
    int value;
    if (ioctl(fd, INA219_IOCTL_READ_SVOLTAGE, &value) < 0) {
        perror("Failed to read Shunt voltage register\n");
        close(fd);
        return errno;
    }
    printf("Shunt voltage = %d\n", value);
    return value;
}

int Read_BVoltage(){
    int value;
    if (ioctl(fd, INA219_IOCTL_READ_BVOLTAGE, &value) < 0) {
        perror("Failed to read Bus voltage register\n");
        close(fd);
        return errno;
    }
    printf("Bus voltage = %d\n", value);
    return value;
}

int Read_Power(){
    int value;
    if (ioctl(fd, INA219_IOCTL_READ_POWER, &value) < 0) {
        perror("Failed to read Power register\n");
        close(fd);
        return errno;
    }
    printf("Power  = %d\n", value);
    return value;
}

int Read_Current(){
    int value;
    if (ioctl(fd, INA219_IOCTL_READ_POWER, &value) < 0) {
        perror("Failed to read Current register\n");
        close(fd);
        return errno;
    }
    printf("Current  = %d\n", value);
    return value;
}

int Read_Config(){
    int value;
    if (ioctl(fd, INA219_IOCTL_READ_CONFIG, &value) < 0) {
        perror("Failed to read Configuration register\n");
        close(fd);
        return errno;
    }
    printf("Configuration  = %d\n", value);
    return value;
}

int Read_Calib(){
    int value;
    if (ioctl(fd, INA219_IOCTL_READ_CALIBRATION, &value) < 0) {
        perror("Failed to read Calibration register\n");
        close(fd);
        return errno;
    }
    printf("Calibration  = %d\n", value);
    return value;
}

// hàm write thanh ghi cụ thể
int Write_Calib(int value){
    if (ioctl(fd, INA219_IOCTL_WRITE_CALIBRATION , &value) < 0) {
        perror("Failed to write Calibration register\n");
        close(fd);
        return errno;
    }
    printf("Write Calibration register Succeed\n");
    return value;
}

int Write_Config(int Mode, int Sadc, int Badc, int Pg, int Brng, int Rst){
    int value = (Rst << 15) | (Brng << 13) | (Pg << 11) | (Badc << 7) | (Sadc << 3) | Mode;
    if (ioctl(fd, INA219_IOCTL_WRITE_CONFIG, &value) < 0) {
        perror("Failed to write Configuration register\n");
        close(fd);
        return errno;
    }
    printf("Write Configuration register Succeed\n");
    return value;
}

// hàm shutdown
int shut_Down(){
    int value_shutDown = 0;
    if (ioctl(fd, INA219_IOCTL_WRITE_CONFIG , &value_shutDown) < 0) {
        perror("Failed to write Calibration register\n");
        close(fd);
        return errno;
    }
    printf("Shutting down\n");
    
}