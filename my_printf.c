#include <stdio.h>
#include <unistd.h>
#include <stdarg.h>
/* function for collecting the char to be printed in the output */
int putchar_func(int m) {
    char n[1];
    n[0] = (char)m;
    return write(1, n, 1);
}

/* Function to ctrl format and returns the total size of arguments with the total size of the base string */
int printf_func_aux(char *str, va_list list, int len);


int putnum_func(unsigned long long n, unsigned int base, const char *digits) {
    int res = 1;
    if (n >= base)
        res += putnum_func(n / base, base, digits);
    putchar_func(digits[n % base]);
    return res;
}

/* Function for int (or listpropriate variant) argument convertion to signed decimal (d) va_arg() fetches the 
next arg in the para list of the function with the var type returns the next additional argument as an expression 
of the type of var. va_start() must be called first to initialize the list or object bf  using the va_arg()*/
int print_d_func(char *str, va_list list, int len) {
    int t = va_arg(list, int);
    unsigned long long a;
    if (t < 0) {
        putchar_func('-');
        len++;
        a = -(unsigned)t;
    } else {
        a = t;
    }
    len += putnum_func(a, 10, "0123456789");
    return printf_func_aux(str, list, len);
}

/* Function for int (or listpropriate variant) argument convertion to unsigned octal (o) */
int print_o_func(char *str, va_list list, int len) {
    unsigned int n = va_arg(list, unsigned int);
    len += putnum_func(n, 8, "01234567");
    return printf_func_aux(str, list, len);
}

/* Function for int (or listpropriate variant) argument convertion to unsigned decimal (u) */
int print_u_func(char *str, va_list list, int len) {
    int n = va_arg(list, int);
    if(n<0){
        n = -n;
    }
    len += putnum_func(n, 10, "0123456789");
    return printf_func_aux(str, list, len);
}

/* Function for int (or listpropriate variant) argument convertion to unsigned hexadecimal (x) */
int print_x_func(char *str, va_list list, int len) {
    unsigned int n = va_arg(list, unsigned int);
    len += putnum_func(n, 16, "0123456789abcdef");
    return printf_func_aux(str, list, len);
}

/* Function for int argument convertion to an unsigned char (c) */
int print_c_func(char *str, va_list list, int len) {
    int c = va_arg(list, int);
    putchar_func(c);
    return printf_func_aux(str, list, len + 1);
}

/* Function for int char* argument expected to be a pointer to an array of character type (p) (pointer to 
a string). Characters from the array are written up to (but not including) a terminating NULL character */
int print_p_func(char *str, va_list list, int len) {
    unsigned int n = va_arg(list, unsigned int);
    len += putnum_func(n, 16, "0123456789abcdef");
    return printf_func_aux(str, list, len);
}

/* Function void * pointer argument (s) printed in hexadecimal */
int print_s_func(char *str, va_list list, int len) {
    const char *s = va_arg(list, const char *);
    if (s == NULL) {
        s = "(null)";
    }
    while (*s) {
        putchar_func(*s++);
        len++;
    }
    return printf_func_aux(str, list, len);
}

/*  */
typedef int (*ftn_print_dispatch_f)(char *str, va_list list, int len);

/*Function to handle the percentile. It controls percent format. */
ftn_print_dispatch_f const ftn_print_dispatch[256] = {
    ['d'] = print_d_func,
    ['o'] = print_o_func,
    ['u'] = print_u_func,
    ['x'] = print_x_func,
    ['c'] = print_c_func,
    ['s'] = print_s_func,
    ['p'] = print_p_func,
};

/* Function for the size of the elements of the specifier to be printed, where; char *str 
is the string format, va_list list is the list of arguements and int len is the 
iteration/positions/length/size etc parameter*/
int printf_func_aux(char *str, va_list list, int len) {
    int q;

    while (*str) {
        q = (unsigned char)*str++;
        if (q != '%') {
            putchar_func(q);
            len++;
        } else {
            q = (unsigned char)*str++;
            if (ftn_print_dispatch[q] == NULL) {
                if (q == '\0')
                    break;
                putchar_func(q);
                len++;
            } else {
                return ftn_print_dispatch[q](str, list, len);
            }
        }
    }
    return len;
}


int vprintf_func(char *str, va_list list) {
    return printf_func_aux(str, list, 0);
}

/* Function for my_printf i.e custom printf() */
int my_printf(char *str, ...) {
    va_list list;
    int r;
    va_start(list, str);
    r = printf_func_aux(str, list, 0);
    va_end(list);
    return r;
}

/* Main function to test the functionality of the program locally */
int main(void) {
    int total = my_printf("%d\n", 123);
    my_printf("%d\n", total);
    my_printf("%cello %s\n", 'H', "World!");
    my_printf("%d == 0%o == 0x%x == 0x%X\n", 1, 1, 1, 1);
    my_printf("%d == 0%o == 0x%x == 0x%X\n", 0xdead, 0xdead, 0xdead, 0xdead);
    return 0;
}