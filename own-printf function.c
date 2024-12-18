#include <stdarg.h> // Needed for va_list and va_arg

// Function to find the length of a string
int my_strlen(const char *str) {
    int len = 0;
    while (str[len] != '\0') {
        len++;
    }
    return len;
}

// Function to print a single character
void my_putchar(char c) {
    // Replace this with low-level system calls if needed
    asm volatile (
        "mov $1, %%rax;"      // syscall: write
        "mov $1, %%rdi;"      // file descriptor: stdout
        "mov %[char], %%rsi;" // character address
        "mov $1, %%rdx;"      // size: 1 byte
        "syscall;"
        :
        : [char] "r" (&c)
    );
}

// Function to print a string
void my_puts(const char *str) {
    while (*str) {
        my_putchar(*str++);
    }
}

// Function to convert an integer to a string
void my_itoa(int num, char *str, int base) {
    int i = 0;
    bool isNegative = false;

    // Handle negative numbers for base 10
    if (num < 0 && base == 10) {
        isNegative = true;
        num = -num;
    }

    // Convert number to string
    do {
        int rem = num % base;
        str[i++] = (rem > 9) ? (rem - 10) + 'a' : rem + '0';
        num /= base;
    } while (num > 0);

    // Add negative sign if needed
    if (isNegative) {
        str[i++] = '-';
    }

    str[i] = '\0'; // Null-terminate the string

    // Reverse the string
    for (int j = 0, k = i - 1; j < k; j++, k--) {
        char temp = str[j];
        str[j] = str[k];
        str[k] = temp;
    }
}

// Custom printf function
void my_printf(const char *format, ...) {
    va_list args;
    va_start(args, format);

    while (*format) {
        if (*format == '%') {
            format++;
            switch (*format) {
                case 'c': { // Character
                    char c = (char)va_arg(args, int);
                    my_putchar(c);
                    break;
                }
                case 's': { // String
                    const char *str = va_arg(args, const char *);
                    my_puts(str);
                    break;
                }
                case 'd': { // Integer
                    int num = va_arg(args, int);
                    char buffer[20];
                    my_itoa(num, buffer, 10);
                    my_puts(buffer);
                    break;
                }
                case 'x': { // Hexadecimal
                    int num = va_arg(args, int);
                    char buffer[20];
                    my_itoa(num, buffer, 16);
                    my_puts(buffer);
                    break;
                }
                default: // Unsupported format
                    my_putchar('%');
                    my_putchar(*format);
            }
        } else {
            my_putchar(*format); // Regular character
        }
        format++;
    }

    va_end(args);
}

int main() {
    my_printf("Hello, %s! Your score is %d, and in hex: %x\n", "World", 42, 42);
    my_printf("Character test: %c\n", 'A');
    return 0;
}
