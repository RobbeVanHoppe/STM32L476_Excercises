//
// Created by robbe on 7/05/24.
//
#include <cstdio>
#include <cstring>

// Linux includes
#include <fcntl.h> // Contains file controls like O_RDWR
#include <cerrno> // Error integer and strerror() function
#include <termios.h> // Contains POSIX terminal control definitions
#include <unistd.h> // write(), read(), close()

int serial_port = open("/dev/ttyACM0", O_RDWR);
struct termios tty;


int main()
{
    if (serial_port < 0) {
        printf("Serial port error: %i from open: %s\n", errno, strerror(errno));
    }
    else printf("Serial connection open\n");

    if(tcgetattr(serial_port, &tty) != 0) {
        printf("Error %i from tcgetattr: %s\n", errno, strerror(errno));
    }
    else printf("Attributes read\n");

    tty.c_cflag &= ~PARENB;
    tty.c_cflag &= ~CSTOPB;
    tty.c_cflag &= ~CSIZE;
    tty.c_cflag |= CS8;
    tty.c_cflag &= ~CRTSCTS;
    tty.c_cflag |= CREAD | CLOCAL;
    tty.c_lflag &= ~ICANON;
    tty.c_lflag &= ~ECHO;
    tty.c_lflag &= ~ECHOE;
    tty.c_lflag &= ~ECHONL;
    tty.c_lflag &= ~ISIG;
    tty.c_iflag &= ~(IXON | IXOFF | IXANY);
    tty.c_iflag &= ~(IGNBRK | BRKINT | PARMRK | ISTRIP | INLCR | IGNCR | ICRNL);
    tty.c_oflag &= ~OPOST;
    tty.c_oflag &= ~ONLCR;

    tty.c_cc[VTIME] = 10;
    tty.c_cc[VMIN] = 0;

    cfsetispeed(&tty, B115200);
    cfsetospeed(&tty, B115200);

    if (tcsetattr(serial_port, TCSANOW, &tty) != 0) {
        printf("Error %i from tcsetattr: %s\n", errno, strerror(errno));
        return 1;
    }

    unsigned char msg[] = { 'H', 'e', 'l', 'l', 'o', '\r' };
    write(serial_port, msg, sizeof(msg));

    char read_buf [13];
    memset(&read_buf, '\0', 13);

    int num_bytes = read(serial_port, &read_buf, 13);
    if (num_bytes < 0) {
        printf("Error reading: %s", strerror(errno));
        return 1;
    }

    printf("Read %i bytes. Received message: %s", num_bytes, read_buf);

    close(serial_port);
    return 0;
}
