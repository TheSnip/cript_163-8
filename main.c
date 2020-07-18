#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define SWAP(_first, _second) do{       \
    register char _temp = *(_second);   \
    *(_second) = *(_first);             \
    *(_first) = _temp;                  \
}while(0)

/* Transform functions */
#define REVERSE(_str, _length) do{          \
    register int start = 0;                 \
    register int end = (_length) - 1;       \
                                            \
    while (start < end) {                   \
        SWAP(&(_str[start]), &(_str[end])); \
        ++start;                            \
        --end;                              \
    }                                       \
}while(0)

void itoa(const int _num_param, char* _str_param, const int _base_param) {
    register int num = _num_param;

    /* Process individual digits */
    register int i = 0;
    while (num != 0) {
        register int rem = num % _base_param;

        if (rem > 9)
            _str_param[i] = (char)((rem - 10) + 'a');
        else
            _str_param[i] = (char)(rem + '0');

        num /= _base_param;

        ++i;
    }

    _str_param[i] = '\0'; /* Append string terminator */

    /* Reverse the string */
    REVERSE(_str_param, i);
}


/* Encrypt data */
void encryption(char* _str_param) {
    const char alphabet[26] = "abcdefghijklmnopqrstuvwxyz";

    register int size = (int)strlen(_str_param) + 1;

    register char* _str_copy = (char*)alloca(size);

    strncpy(_str_copy, _str_param, size);
    strncpy(_str_param, "", 1);

    register int i = 0;
    while (i < size) {
        register int j = 0;
        while (j < 26) {
            if (_str_copy[i] == alphabet[j]) {
                /* Optimization */
                j = (int)alphabet[j]; /* Convert to number in alphabet */

                char buf[8]; /* Binding buffer */

                itoa(j, buf, 2); /* Convert to binary */

                /* Convert to Bacon */
                register int k = 0;
                while (k < 7) {
                    if (buf[k] == '1')
                        buf[k] = 'B';
                    else if (buf[k] == '0')
                        buf[k] = 'A';

                    ++k;
                }

                strncat(_str_param, buf, 8);
                break;
            }
            ++j;
        }
        ++i;
    }
}

/* Decrypt data */
void decryption(char* _str_param) {
    register int size = (int)strlen(_str_param) + 1;
    
    register char* _str_copy = (char*)alloca(size);

    strncpy(_str_copy, _str_param, size);
    strncpy(_str_param, "", 1);

    register int i = 0;
    register int length = size / 7;
    while (i < length) {
        char buf_binary[8]; /* Binding buffer for conversion */

        /* Convert from Bacon */
        register int j = 0;
        while (j < 7) {
            register int index = i * 7 + j;

            if ((_str_copy[index] == 'A') || (_str_copy[index] == 'a'))
                buf_binary[j] = '0';
            else if ((_str_copy[index] == 'B') || (_str_copy[index] == 'b'))
                buf_binary[j] = '1';

            ++j;
        }

        char* saveToken = NULL;
        register int num = (int)strtol(buf_binary, &saveToken, 2); /* Convert from binary */

        char buf[2] = { (char)num, '\0' }; /* Binding buffer for conversion */

        strncat(_str_param, buf, 2);

        ++i;
    }
}

void initArgs(const int __argc_param, const char** __argv_param) {
    const register char usage[166] = "\n"
        "usage: cript <option>\n"
        " Option              Description\n"
        " -e [string]         encrypting string\n"
        " -d [string]         decrypting string\n"
        " -h                  this page";
    register char* string = NULL;
    register unsigned char result = 0;

    if (__argc_param != 1) {
        switch (__argv_param[1][1]) {
        case 'e':
            result = 1;

            if (__argv_param[2] != NULL) {    
                string = (char *)__argv_param[2];
                encryption(string);
                printf("%s", string);

                result = 0;
            }
            break;
        case 'd':
            result = 1;

            if (__argv_param[2] != NULL) {
                string = (char *)__argv_param[2];
                decryption(string);
                printf("%s", string);

                result = 0;
            }
            break;
        default:
            result = 1;
            break;
        }
    }

    if ((__argc_param == 1) || result)
        printf("%s", usage);
}

int main(const int argc, const char** argv) {
    initArgs(argc, argv);
	return 0;
}
