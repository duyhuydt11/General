#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include "thu_vien.h"
#include <time.h>
int main(){
    ina219_Init();
    Write_Config(7, 7, 7, 3, 1, 0);
    Write_Calib(0x4096);
    while(1){
        Read_Power();
        Read_BVoltage();
        Read_Current();
        Read_SVoltage();
        sleep(1);
    }
    return 0;
}