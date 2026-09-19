CC = gcc
CFLAGS = -Wall -Wextra
LIBS = -lmosquitto

TARGET = sensor_app
SRC = sensor_app.c

all:
	$(CC) $(CFLAGS) $(SRC) -o $(TARGET) $(LIBS)

clean:
	rm -f $(TARGET)
