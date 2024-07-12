#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/ioctl.h>

#define DEVICE_PATH "/dev/ina219"
#define INA219_IOCTL_MAGIC 'i'
#define INA219_IOCTL_WRITE_CONFIG       _IOW(INA219_IOCTL_MAGIC, 0, int) 
#define INA219_IOCTL_READ_CONFIG        _IOR(INA219_IOCTL_MAGIC, 6, int) 
#define INA219_IOCTL_READ_SVOLTAGE      _IOR(INA219_IOCTL_MAGIC, 1, int)
#define INA219_IOCTL_READ_BVOLTAGE      _IOR(INA219_IOCTL_MAGIC, 2, int)
#define INA219_IOCTL_READ_POWER         _IOR(INA219_IOCTL_MAGIC, 3, int)
#define INA219_IOCTL_READ_CURRENT       _IOR(INA219_IOCTL_MAGIC, 4, int)
#define INA219_IOCTL_WRITE_CALIBRATION  _IOW(INA219_IOCTL_MAGIC, 5, int)
#define INA219_IOCTL_READ_CALIBRATION   _IOR(INA219_IOCTL_MAGIC, 7, int)

// ham init
int ina219_Init();

// ham doc gia tri thanh ghi
int Read_SVoltage();
int Read_BVoltage();
int Read_Power();
int Read_Current();
int Read_Config();
int Read_Calib();

// ham ghi gia tri thanh ghi
int Write_Calib(int value);
int Write_Config(int Mode, int Sadc, int Badc, int Pg, int Brng, int Rst);

// ham tat che do doc/viet
int shut_Down();
