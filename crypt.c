#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define ERROR    \
    result = 0U; \
    printf("Error: %s invalid", __argv_param[1]);

#define FOR_ENCR do{                       \
    if (__argv_param[2] != NULL) {         \
        string = (char *)__argv_param[2];  \
        encryption(string);                \
        printf("%s", string);              \
                                           \
        result = 0U;                       \
    }                                      \
}while(0)

#define FOR_DECR do{                       \
    if (__argv_param[2] != NULL) {         \
        string = (char *)__argv_param[2];  \
        decryption(string);                \
        printf("%s", string);              \
                                           \
        result = 0U;                       \
    }                                      \
}while(0)

#define SWAP(_first, _second) do{ \
    *(_first)  ^= *(_second);     \
    *(_second) ^= *(_first);      \
    *(_first)  ^= *(_second);     \
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

static void itoa(const int _num_param, char* _str_param, const int _base_param) {
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
static void encryption(char* _str_param) {
    const char* alphabet = "abcdefghijklmnopqrstuvwxyz";

    register const unsigned size = (unsigned)strlen(_str_param) + 1U;

    register char* _str_copy = (char*)alloca(size);

    strncpy(_str_copy, _str_param, size);
    strncpy(_str_param, "", 1);

    register unsigned i = 0U;
    while (i < size) {
        register unsigned j = 0U;
        while (j < 26U) {
            if (_str_copy[i] == alphabet[j]) {
                /* Optimization */
                j = (unsigned)alphabet[j]; /* Convert to number in alphabet */

                char buf[8]; /* Binding buffer */

                itoa(j, buf, 2); /* Convert to binary */

                /* Convert to Bacon */
                register unsigned k = 0U;
                while (k < 7U) {
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
static void decryption(char* _str_param) {
    register const unsigned size = (unsigned)strlen(_str_param) + 1U;

    register char* _str_copy = (char*)alloca(size);

    strncpy(_str_copy, _str_param, size);
    strncpy(_str_param, "", 1);

    register unsigned i = 0U;
    register const unsigned length = size / 7U;
    while (i < length) {
        char buf_binary[8]; /* Binding buffer for conversion */

        /* Convert from Bacon */
        register unsigned j = 0U;
        while (j < 7U) {
            register unsigned index = i * 7U + j;

            if ((_str_copy[index] == 'A') || (_str_copy[index] == 'a'))
                buf_binary[j] = '0';
            else if ((_str_copy[index] == 'B') || (_str_copy[index] == 'b'))
                buf_binary[j] = '1';

            ++j;
        }

        char* saveToken = NULL;
        register const int num = (int)strtol(buf_binary, &saveToken, 2); /* Convert from binary */

        char buf[2] = { (char)num, '\0' }; /* Binding buffer for conversion */

        strncat(_str_param, buf, 2);

        ++i;
    }
}

void initArgs(const int __argc_param, const char** __argv_param) {
    const char usage[166] =
	    "Usage: crypt [option] <string>\n"
        "Option\n"
        " -d, --decrypt         decrypting text\n"
        " -e, --encrypt         encrypting text\n"
        " -h, --help            This help text";
    register char* string = NULL;
    register unsigned char result = 1U;

    if (__argc_param > 1) {
        register const unsigned size = (unsigned)strlen(__argv_param[1]);
        if (size < 3U) {
            switch (__argv_param[1][1]) {
            case 'e':
                FOR_ENCR;
                break;
            case 'd':
                FOR_DECR;
                break;
            case 'h':
                break;
            default:
                ERROR;
                break;
            }
        } else {
            char* buf = (char*)alloca(size - 1U);
            strncpy(buf, "", 1);

            register unsigned i = 2U;
            while (i < size) {
                char buf_char[2] = { __argv_param[1][i], '\0' };
                strncat(buf, buf_char, 2);
                ++i;
            }

            if (strncmp(buf, "encrypt", size - 1U) == 0) {
                FOR_ENCR;
            } else if (strncmp(buf, "decrypt", size - 1U) == 0) {
                FOR_DECR;
            } else if (strncmp(buf, "help", size - 1U) == 0) {
                result = 1U;
            } else {
                ERROR;
            }
        }
    }

    if ((__argc_param == 1) || result)
        printf("%s", usage);
}

int main(const int argc, const char** argv) {
    initArgs(argc, argv);
    return 0;
}
