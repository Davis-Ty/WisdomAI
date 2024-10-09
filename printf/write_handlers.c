#include "main.h"

#include <unistd.h>

#include <string.h>

#include <errno.h>

#include <stdio.h>

#define WRITE_BUFFER_SIZE 1024

#define F_ZERO 1

#define F_PLUS 2

#define F_SPACE 4

/************************* WRITE TO BUFFER *************************/

ssize_t copy_data_to_buffer(char *buffer, const char *data, size_t bytes_to_write) {

    if (!buffer || !data || bytes_to_write > WRITE_BUFFER_SIZE) {

        errno = EINVAL; // Invalid argument

        return -1;

    }

    memcpy(buffer, data, bytes_to_write);

    return bytes_to_write;

}

/************************* WRITE CHAR TO BUFFER *************************/

int write_char_to_buffer(char c, char buffer[], int flags, int width) {

    int i = 0;

    char padd = (flags & F_ZERO) ? '0' : ' ';

    if (width > 1) {

        for (i = 0; i < width - 1; i++) {

            buffer[i] = padd; 

        }

        buffer[i++] = c;

    } else {

        buffer[i++] = c;

    }

    buffer[i] = '\0';

    return i;

}

/************************* CONVERT NUMBER TO STRING *************************/

int convert_num_to_string(long num, char buffer[]) {

    int i = 0;

    do {

        buffer[i++] = (num % 10) + '0';

        num /= 10;

    } while (num > 0);

    

    // Reverse the string in place

    for (int start = 0, end = i - 1; start < end; start++, end--) {

        char temp = buffer[start];

        buffer[start] = buffer[end];

        buffer[end] = temp;

    }

    

    buffer[i] = '\0';

    return i;

}

/************************* FORMAT NUMBER *************************/

int format_number(long num, char buffer[], int flags, int width) {

    char extra_ch = 0;

    if (num < 0) {

        num = -num;

        extra_ch = '-';

    } else if (flags & F_PLUS) {

        extra_ch = '+';

    } else if (flags & F_SPACE) {

        extra_ch = ' ';

    }

    int len = convert_num_to_string(num, buffer);

    if (extra_ch) {

        buffer[len++] = extra_ch;

    }

    // Padding logic

    int padd_len = width - len;

    if (padd_len > 0) {

        memmove(buffer + padd_len, buffer, len);

        memset(buffer, (flags & F_ZERO) ? '0' : ' ', padd_len);

        len = width;

    }

    buffer[len] = '\0';

    return len;

}

/************************* UNIT TESTS *************************/

void run_tests() {

    char buffer[WRITE_BUFFER_SIZE];

    

    // Test 1: write_char_to_buffer

    int result = write_char_to_buffer('A', buffer, 0, 5);

    printf("Test 1 - write_char_to_buffer: %s | Result: %d\n", buffer, result);

    

    // Test 2: format_number (positive number)

    result = format_number(12345, buffer, 0, 8);

    printf("Test 2 - format_number (12345): %s | Result: %d\n", buffer, result);

    // Test 3: format_number (negative number)

    result = format_number(-12345, buffer, 0, 8);

    printf("Test 3 - format_number (-12345): %s | Result: %d\n", buffer, result);

    

    // Test 4: format_number (with F_PLUS)

    result = format_number(12345, buffer, F_PLUS, 8);

    printf("Test 4 - format_number (12345, F_PLUS): %s | Result: %d\n", buffer, result);

    

    // Test 5: format_number (with padding)

    result = format_number(123, buffer, F_ZERO, 6);

    printf("Test 5 - format_number (123, F_ZERO): %s | Result: %d\n", buffer, result);

    // Test 6: copy_data_to_buffer

    const char *data = "Hello";

    result = copy_data_to_buffer(buffer, data, strlen(data));

    printf("Test 6 - copy_data_to_buffer: %s | Result: %ld\n", buffer, result);

}

int main(int argc, char *argv[]) {

    char buffer[WRITE_BUFFER_SIZE];

    int bytes_written;

    // Run unit tests

    run_tests();

    // Example: Writing a character

    bytes_written = write_char_to_buffer('A', buffer, 0, 5);

    if (bytes_written < 0) {

        perror("write_char_to_buffer");

        return 1;

    }

    if (write(1, buffer, bytes_written) < 0) {

        perror("write");

        return 1;

    }

    // Example: Writing a number

    bytes_written = format_number(12345, buffer, 0, 8);

    if (bytes_written < 0) {

        perror("format_number");

        return 1;

    }

    if (write(1, buffer, bytes_written) < 0) {

        perror("write");

        return 1;

    }

    return 0;

}
