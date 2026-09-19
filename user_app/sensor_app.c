#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <mosquitto.h>

#define DEVICE_PATH "/dev/virtual_sensor"
#define MQTT_HOST "localhost"
#define MQTT_PORT 1883
#define MQTT_TOPIC "virtual_sensor/data"

int main(void)
{
    int fd;
    char buffer[128];
    ssize_t bytes_read;
    struct mosquitto *mosq;

    mosquitto_lib_init();

    mosq = mosquitto_new(NULL, true, NULL);

    if (mosq == NULL)
    {
        fprintf(stderr, "Failed to create MQTT client\n");
        mosquitto_lib_cleanup();
        return 1;
    }    if (mosquitto_connect(mosq, MQTT_HOST, MQTT_PORT, 60) != MOSQ_ERR_SUCCESS)
    {
        fprintf(stderr, "Failed to connect to MQTT broker\n");
        mosquitto_destroy(mosq);
        mosquitto_lib_cleanup();
        return 1;
    }

    printf("Connected to MQTT broker\n");

    while (1)
    {
        fd = open(DEVICE_PATH, O_RDONLY);

        if (fd < 0)
        {
            perror("Failed to open virtual sensor");
            break;
        }

        bytes_read = read(fd, buffer, sizeof(buffer) - 1);

        close(fd);

        if (bytes_read < 0)
        {
            perror("Failed to read virtual sensor");
            break;
        }

        buffer[bytes_read] = '\0';

        printf("Sensor Data:\n%s", buffer);
        if (mosquitto_publish(mosq,
                              NULL,
                              MQTT_TOPIC,
                              bytes_read,
                              buffer,
                              0,
                              false) != MOSQ_ERR_SUCCESS)
        {
            fprintf(stderr, "Failed to publish sensor data\n");
            break;
        }

        printf("Published to %s\n\n", MQTT_TOPIC);

        sleep(5);
    }

    mosquitto_disconnect(mosq);
    mosquitto_destroy(mosq);
    mosquitto_lib_cleanup();

    return 0;
}
