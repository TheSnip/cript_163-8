#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define FOR_READ_STRING(x) do{               \
    scanf("%99[^\\n]", (x));                 \
                                             \
    register int size = (int)strlen(string); \
    REVERSE(string, size);                   \
    register int _expr = size - 1;           \
    string[_expr] = '\0';                    \
    REVERSE(string, _expr);                  \
    _expr -= 1;                              \
                                             \
    if (string[_expr] == '\n')               \
        string[_expr] = '\0';                \
}while(0)

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

        register char buf[2] = { (char)num, '\0' }; /* Binding buffer for conversion */

        strncat(_str_param, buf, 2);

        ++i;
    }

}

int main(void) {
    unsigned char count = 0U;

    scanf("%c", &count);

    char string[600] = { 0 };

    if ((count == 'e') || (count == 'd'))
        FOR_READ_STRING(string);

    switch (count) {
        case 'e':
            encryption(string);
            printf("%s\n", string);
            printf("End\n");
            break;
        case 'd':
            decryption(string);
            printf("%s\n", string);
            printf("End\n");
            break;
        default:
            printf("Unknown command!\ne - encrypt\nd - decrypt");
            break;
	}

	return 0;
}
