#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/fs.h>
#include <linux/cdev.h>
#include <linux/device.h>
#include <linux/uaccess.h>
#include <linux/random.h>

#define DEVICE_NAME "virtual_sensor"
#define CLASS_NAME "virtual_sensor_class"

static dev_t device_number;
static struct cdev virtual_sensor_cdev;
static struct class *virtual_sensor_class;

static int virtual_sensor_open(struct inode *inode, struct file *file)
{
    pr_info("virtual_sensor: device opened\n");
    return 0;
}

static int virtual_sensor_release(struct inode *inode, struct file *file)
{
    pr_info("virtual_sensor: device closed\n");
    return 0;
}

static ssize_t virtual_sensor_read(struct file *file,
                                   char __user *buffer,
                                   size_t len,
                                   loff_t *offset)
{
    char sensor_data[64];
    int temperature;
    int humidity;
    int data_length;

    if (*offset > 0)
        return 0;

    temperature = 20 + (get_random_u32() % 11);
    humidity = 40 + (get_random_u32() % 31);

    data_length = snprintf(sensor_data,
                           sizeof(sensor_data),
                           "Temperature: %d C\nHumidity: %d %%\n",
                           temperature,
                           humidity);

    if (len < data_length)
        return -EINVAL;

    if (copy_to_user(buffer, sensor_data, data_length))
        return -EFAULT;

    *offset += data_length;

    pr_info("virtual_sensor: data sent to user space\n");

    return data_length;
}

static const struct file_operations virtual_sensor_fops = {
    .owner = THIS_MODULE,
    .open = virtual_sensor_open,
    .read = virtual_sensor_read,
    .release = virtual_sensor_release,
};

static int __init virtual_sensor_init(void)
{
    int ret;

    pr_info("virtual_sensor: initializing\n");

    ret = alloc_chrdev_region(&device_number, 0, 1, DEVICE_NAME);
    if (ret < 0) {
        pr_err("virtual_sensor: failed to allocate device number\n");
        return ret;
    }

    cdev_init(&virtual_sensor_cdev, &virtual_sensor_fops);

    ret = cdev_add(&virtual_sensor_cdev, device_number, 1);
    if (ret < 0) {
        pr_err("virtual_sensor: failed to add character device\n");
        unregister_chrdev_region(device_number, 1);
        return ret;
    }

    virtual_sensor_class = class_create(CLASS_NAME);

    if (IS_ERR(virtual_sensor_class)) {
        pr_err("virtual_sensor: failed to create device class\n");
        cdev_del(&virtual_sensor_cdev);
        unregister_chrdev_region(device_number, 1);
        return PTR_ERR(virtual_sensor_class);
    }

    if (IS_ERR(device_create(virtual_sensor_class,
                             NULL,
                             device_number,
                             NULL,
                             DEVICE_NAME))) {
        pr_err("virtual_sensor: failed to create device\n");
        class_destroy(virtual_sensor_class);
        cdev_del(&virtual_sensor_cdev);
        unregister_chrdev_region(device_number, 1);
        return -1;
    }

    pr_info("virtual_sensor: /dev/%s created\n", DEVICE_NAME);

    return 0;
}

static void __exit virtual_sensor_exit(void)
{
    device_destroy(virtual_sensor_class, device_number);
    class_destroy(virtual_sensor_class);
    cdev_del(&virtual_sensor_cdev);
    unregister_chrdev_region(device_number, 1);

    pr_info("virtual_sensor: module unloaded\n");
}

module_init(virtual_sensor_init);
module_exit(virtual_sensor_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("EC535 Project");
MODULE_DESCRIPTION("Virtual IoT Sensor Character Device");
