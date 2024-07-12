#include <linux/init.h>
#include <linux/module.h>
#include <linux/i2c.h>
#include <linux/device.h>
#include <linux/fs.h>
#include <linux/uaccess.h>

#define DRIVER_NAME "ina219_driver"
#define CLASS_NAME "ina219"
#define DEVICE_NAME "ina219"


#define INA219_CONFIG            0x00
#define INA219_SHUNT_VOLTAGE     0x01 /* readonly */
#define INA219_BUS_VOLTAGE       0x02 /* readonly */
#define INA219_POWER             0x03 /* readonly */
#define INA219_CURRENT           0x04 /* readonly */
#define INA219_CALIBRATION       0x05

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



static struct i2c_client *ina219_client;
static struct class* ina219_class = NULL;
static struct device* ina219_device = NULL;
static int major_number;

static int read_register(struct i2c_client *client, u8 reg,int num)
{
    u8 buf[2];
    int ret;

    ret = i2c_smbus_read_i2c_block_data(ina219_client, reg, sizeof(buf), buf);
    if (ret < 0) {
        pr_err("Failed to read register %x\n", reg);
        return ret;
    }

    return ((buf[0] << 8) | buf[1]);
}


static int write_register(u8 reg, u16 data)
{
    int ret;

    ret = i2c_smbus_write_word_data(ina219_client, reg, data);
    if (ret < 0) {
        pr_err("Failed to write register %x\n", reg);
        return ret;
    }

    return 0;
}




static long ina219_ioctl(struct file *file, unsigned int cmd, unsigned long arg)
{
    int ret;
    u16 data;

    switch (cmd) {
        case INA219_IOCTL_WRITE_CONFIG:
            if (copy_from_user(&data, (u16 __user *)arg, sizeof(data))) {
                return -EFAULT;
            }
            ret = write_register(0, data);
            break;
        case INA219_IOCTL_WRITE_CALIBRATION:
            if (copy_from_user(&data, (u16 __user *)arg, sizeof(data))) {
                return -EFAULT;
            }
            ret = write_register(5, data);
            break;
//---------------------------------------------------------------------------
        case INA219_IOCTL_READ_CALIBRATION:
            data = read_register(ina219_client,cmd,7);
            if (copy_to_user((u16 __user *)arg, &data, sizeof(data))) {
                return -EFAULT;
            }
            ret = 0;
            break;
        case INA219_IOCTL_READ_CONFIG:
            data = read_register(ina219_client,cmd,6);
            if (copy_to_user((u16 __user *)arg, &data, sizeof(data))) {
                return -EFAULT;
            }
            ret = 0;
            break;
        case INA219_IOCTL_READ_SVOLTAGE:
            data = read_register(ina219_client,cmd,1);
            if (copy_to_user((u16 __user *)arg, &data, sizeof(data))) {
                return -EFAULT;
            }
            ret = 0;
            break;
        case INA219_IOCTL_READ_BVOLTAGE:
            data = read_register(ina219_client,cmd,2);
            if (copy_to_user((u16 __user *)arg, &data, sizeof(data))) {
                return -EFAULT;
            }
            ret = 0;
            break;
        case INA219_IOCTL_READ_CURRENT:
            data = read_register(ina219_client,cmd,4);
            if (copy_to_user((u16 __user *)arg, &data, sizeof(data))) {
                return -EFAULT;
            }
            ret = 0;
            break;
        case INA219_IOCTL_READ_POWER:
            data = read_register(ina219_client,cmd,3);
            if (copy_to_user((u16 __user *)arg, &data, sizeof(data))) {
                return -EFAULT;
            }
            ret = 0;
            break;
        default:
            ret = -EINVAL;
            break;
    }

    return ret;
}


static int ina219_open(struct inode *inodep, struct file *filep)
{
    printk(KERN_INFO "ina219 device opened\n");
    return 0;
}


static int ina219_release(struct inode *inodep, struct file *filep)
{
    printk(KERN_INFO "ina219 device closed\n");
    return 0;
}


static struct file_operations fops = {
    .open = ina219_open,
    .unlocked_ioctl = ina219_ioctl,
    .release = ina219_release,
};

static int ina219_probe(struct i2c_client *client, const struct i2c_device_id *id)
{
    ina219_client = client;

    // Create a char device
    major_number = register_chrdev(0, DEVICE_NAME, &fops);
    if (major_number < 0) {
        printk(KERN_ERR "Failed to register a major number\n");
        return major_number;
    }

    ina219_class = class_create(THIS_MODULE, CLASS_NAME);
    if (IS_ERR(ina219_class)) {
        unregister_chrdev(major_number, DEVICE_NAME);
        printk(KERN_ERR "Failed to register device class\n");
        return PTR_ERR(ina219_class);
    }

    ina219_device = device_create(ina219_class, NULL, MKDEV(major_number, 0), NULL, DEVICE_NAME);
    if (IS_ERR(ina219_device)) {
        class_destroy(ina219_class);
        unregister_chrdev(major_number, DEVICE_NAME);
        printk(KERN_ERR "Failed to create the device\n");
        return PTR_ERR(ina219_device);
    }

    printk(KERN_INFO "ina219 driver installed\n");
    return 0;
}

static void ina219_remove(struct i2c_client *client)
{
    device_destroy(ina219_class, MKDEV(major_number, 0));
    class_unregister(ina219_class);
    class_destroy(ina219_class);
    unregister_chrdev(major_number, DEVICE_NAME);

    printk(KERN_INFO "ina219 driver removed\n");
}

static const struct of_device_id ina219_of_match[] = {
    { .compatible = "ti,ina219", },
    { },
};
MODULE_DEVICE_TABLE(of, ina219_of_match);

static struct i2c_driver ina219_driver = {
    .driver = {
        .name   = DRIVER_NAME,
        .owner  = THIS_MODULE,
        .of_match_table = of_match_ptr(ina219_of_match),
    },
    .probe      = ina219_probe,
    .remove     = ina219_remove,
};

static int __init ina219_init(void)
{
    printk(KERN_INFO "Initializing ina219 driver\n");
    return i2c_add_driver(&ina219_driver);
}

static void __exit ina219_exit(void)
{
    printk(KERN_INFO "Exiting ina219 driver\n");
    i2c_del_driver(&ina219_driver);
}

module_init(ina219_init);
module_exit(ina219_exit);

MODULE_AUTHOR("DacTan va nhung nguoi 'ban'");
MODULE_DESCRIPTION("ina219 I2C Client Driver with IOCTL Interface");
MODULE_LICENSE("GPL");
