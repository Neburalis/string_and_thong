#include <stdio.h>
#include <stdlib.h>

namespace mystr {

int put(const char * str) {
    while (*str != '\0') {
        putchar(*str);
        ++str;
    }
    return 0;
}

char * chr(const char * str, const char c) {
    do {
        if (*str == c)
            return (char *) str;
        ++str;
    } while (*str != '\0');
    return NULL;
}

size_t len(const char * str) {
    size_t counter = 0;
    while (*str != '\0') {
        ++counter;
        ++str;
    }
    return counter;
}

char * copy(char * dst, const char * src) {
    char * ans = dst;
    while (*src != '\0') {
        *dst = *src;
        ++src;
        ++dst;
    }
    *dst = '\0';
    return ans;
}

char * ncopy(char * dst, const char * src, size_t count) {
    char * ans = dst;
    if (count == 0)
        return NULL;
    while (*src != '\0' && count > 0) {
        *dst = *src;
        ++src;
        ++dst;
        --count;
    }
    *dst = '\0';
    while (count > 0) {
        *dst = '\0';
        ++dst;
        --count;
    }
    return ans;
}

char * concat(char * dst, const char * src) {
    char * ans = dst;
    while (*dst != '\0')
        ++dst;
    while (*src != '\0') {
        *dst = *src;
        ++src;
        ++dst;
    }
    *dst = '\0';
    return ans;
}

char * nconcat(char * dst, const char * src, size_t count) {
    char * ans = dst;
    if (count == 0)
        return NULL;
    while (*dst != '\0' && count > 0) {
        ++dst;
        --count;
    }
    while (*src != '\0' && count > 0) {
        *dst = *src;
        ++src;
        ++dst;
        --count;
    }
    *dst = '\0';
    return ans;
}

char * dupeconcat(const char * first, const char * second) {
    size_t size = len(first) + len(second) + 1;
    char * str = (char *) calloc(size, sizeof(char));
    concat(str, first);
    concat(str, second);
    return str;
}

char * fget(char * str, FILE *stream) {
    if (!stream)
        return NULL;
    char * ans = str;
    int ch = 0; // ?
    while ((ch = getc(stream)) != '\n' && ch != EOF) {
        *str = ch;
        ++str;
    }
    *str = '\0';
    return ans;
}

char * fnget(char * str, size_t count, FILE *stream) {
    if (!stream)
        return NULL;
    char * ans = str;
    int ch = 0; // ?
    while (count > 0 && (ch = getc(stream)) != '\n' && ch != EOF) {
        *str = ch;
        ++str;
        --count;
    }
    *str = '\0';
    return ans;
}

char * dupe(const char * str) {
    size_t str_len = len(str) + 1; // Чтобы поместился \0
    char * ans = (char *) calloc(str_len, sizeof(char));
    ncopy(ans, str, str_len);
    return ans;
}

// Нахождение ближайшей степени 2 >= x
unsigned cpl2(unsigned x)
{
    x = x - 1;
    x = x | (x >> 1);
    x = x | (x >> 2);
    x = x | (x >> 4);
    x = x | (x >> 8);
    x = x | (x >> 16);
    return x+1;
}

ssize_t getline(char ** ptr, size_t * len, FILE * stream) {
    if (stream == NULL) {
        return -1;
    }
    if (*ptr == NULL){
        printf("*ptr is NULL");
        char * str = (char *) calloc(1, sizeof(char));
        ptr = &str;
        *len = 1;
    }
    size_t count = 0;
    int ch = 0;
    printf("*len = %zu\n", *len);
    do {
        ch = getc(stream);
        // printf("ch = %c (%d) \t", ch, ch);
        if (ch == EOF) return EOF;
        if (*len - 1 <= count) { // оставляем один символ для \0
            *len = cpl2(*len+1);
            char * str = (char *) realloc(*ptr, *len);
            ptr = &str;
        }
        // printf("count = %zu, len = %zu\n", count, *len);
        (*ptr)[count] = (char) ch;
        ++count;
    } while (ch != '\n');
    (*ptr)[count+1] = '\0'; // Конец строки
    return count;
}

int el_comp(const char * first, const char * second) {
    do {
        if (*first > *second)
            return 1;
        else if (*first < *second)
            return -1;
        ++first;
        ++second;
    } while (*first != '\0' && *second != '\0');
    return 0;
}

int comp(const char * first, const char * second) {
    size_t first_hash = 0;
    size_t second_hash = 0;
    while (*first != 0 && *second != 0) {
        first_hash += *first;
        second_hash += *second;
        ++first;
        ++second;
        if (first_hash == second_hash)
            continue;
        return (first_hash - second_hash);
    }
    return 0;
}

int ncomp(const char * first, const char * second, size_t size) {
    if (size == 0) {
        return 0;
    }
    do {
        if (*first > *second)
            return 1;
        else if (*first < *second)
            return -1;
        ++first;
        ++second;
        --size;
    } while (size > 0 && *first != '\0' && *second != '\0');
    return 0;
}

const char * err(int errcode) {
    switch (errcode) { // todo errno consts
        case 0:     return "Success";
        case 1:     return "Operation not permitted";
        case 2:     return "No such file or directory";
        case 3:     return "No such process";
        case 4:     return "Interrupted system call";
        case 5:     return "Input/output error";
        case 6:     return "Device not configured";
        case 7:     return "Argument list too long";
        case 8:     return "Exec format error";
        case 9:     return "Bad file descriptor";
        case 10:    return "No child processes";
        case 11:    return "Resource deadlock avoided";
        case 12:    return "Cannot allocate memory";
        case 13:    return "Permission denied";
        case 14:    return "Bad address";
        case 15:    return "Block device required";
        case 16:    return "Device / Resource busy";
        case 17:    return "File exists";
        case 18:    return "Cross-device link";
        case 19:    return "Operation not supported by device";
        case 20:    return "Not a directory";
        case 21:    return "Is a directory";
        case 22:    return "Invalid argument";
        case 23:    return "Too many open files in system";
        case 24:    return "Too many open files";
        case 25:    return "Inappropriate ioctl for device";
        case 26:    return "Text file busy";
        case 27:    return "File too large";
        case 28:    return "No space left on device";
        case 29:    return "Illegal seek";
        case 30:    return "Read-only file system";
        case 31:    return "Too many links";
        case 32:    return "Broken pipe";
        case 33:    return "Numerical argument out of domain";
        case 34:    return "Result too large";
        case 35:    return "Resource temporarily unavailable";
        case 36:    return "Operation now in progress";
        case 37:    return "Operation already in progress";
        case 38:    return "Socket operation on non-socket";
        case 39:    return "Destination address required";
        case 40:    return "Message too long";
        case 41:    return "Protocol wrong type for socket";
        case 42:    return "Protocol not available";
        case 43:    return "Protocol not supported";
        case 44:    return "Socket type not supported";
        case 45:    return "Operation not supported";
        case 46:    return "Protocol family not supported";
        case 47:    return "Address family not supported by protocol family";
        case 48:    return "Address already in use";
        case 49:    return "Can't assign requested address";
        case 50:    return "Network is down";
        case 51:    return "Network is unreachable";
        case 52:    return "Network dropped connection on reset";
        case 53:    return "Software caused connection abort";
        case 54:    return "Connection reset by peer";
        case 55:    return "No buffer space available";
        case 56:    return "Socket is already connected";
        case 57:    return "Socket is not connected";
        case 58:    return "Can't send after socket shutdown";
        case 59:    return "Too many references: can't splice";
        case 60:    return "Operation timed out";
        case 61:    return "Connection refused";
        case 62:    return "Too many levels of symbolic links";
        case 63:    return "File name too long";
        case 64:    return "Host is down";
        case 65:    return "No route to host";
        case 66:    return "Directory not empty";
        case 67:    return "Too many processes";
        case 68:    return "Too many users";
        case 69:    return "Disc quota exceeded";
        case 70:    return "Stale NFS file handle";
        case 71:    return "Too many levels of remote in path";
        case 72:    return "RPC struct is bad";
        case 73:    return "RPC version wrong";
        case 74:    return "RPC prog. not avail";
        case 75:    return "Program version wrong";
        case 76:    return "Bad procedure for program";
        case 77:    return "No locks available";
        case 78:    return "Function not implemented";
        case 79:    return "Inappropriate file type or format";
        case 80:    return "Authentication error";
        case 81:    return "Need authenticator";
        case 82:    return "Device power is off";
        case 83:    return "Device error, e.g. paper out";
        case 84:    return "Value too large to be stored in data type";
        case 85:    return "Bad executable";
        case 86:    return "Bad CPU type in executable";
        case 87:    return "Shared library version mismatch";
        case 88:    return "Malformed Macho file";
        case 89:    return "Operation canceled";
        case 90:    return "Identifier removed";
        case 91:    return "No message of desired type";
        case 92:    return "Illegal byte sequence";
        case 93:    return "Attribute not found";
        case 94:    return "Bad message";
        case 95:    return "Reserved";
        case 96:    return "No message available on STREAM";
        case 97:    return "Reserved";
        case 98:    return "No STREAM resources";
        case 99:    return "Not a STREAM";
        case 100:   return "Protocol error";
        case 101:   return "STREAM ioctl timeout";
        case 102:   return "Operation not supported on socket";
        case 103:   return "No such policy registered";
        case 104:   return "State not recoverable";
        case 105:   return "Previous owner died";
        case 106:   return "Interface output queue is full";
        default:    return "Unknown error code";
    }
}

char * mult(const char * src, size_t count) {
    size_t src_len = len(src);
    size_t size = src_len * count + 2;
    char * str = (char *) calloc(size, sizeof(char));
    char * ans = str;
    for (; count > 0; --count) {
        concat(str, src);
        str += src_len;
    }
    return ans;
}

const char * str(const char * haystack, const char * needle) {
    size_t needle_len = len(needle);
    if (needle_len == 0)
        return haystack;
    if (len(haystack) < needle_len)
        return NULL;
    size_t needle_hash = 0;
    for (int i = 0; i < needle_len; ++i) {
        needle_hash += needle[i];
    }
    const char * ans = haystack;
    size_t rolling_hash = 0;
    for (int i = 0; i < needle_len; ++i) {
        rolling_hash += *ans;
        ++ans;
    }
    while (*ans != '\0') {
        if (rolling_hash == needle_hash && mystr::comp(haystack, needle) == 0)
            return haystack;
        rolling_hash += *ans;
        rolling_hash -= *haystack;
        ++ans;
        ++haystack;
    }
    return NULL;
}

/*
this algorithm (k=33) was first reported by dan bernstein many years ago in comp.lang.c.
another version of this algorithm (now favored by bernstein) uses xor:
hash(i) = hash(i - 1) * 33 ^ str[i];
the magic of number 33 (why it works better than many other constants, prime or not)
has never been adequately explained.
*/
unsigned long hash(unsigned char *str) {
    unsigned long hash = 5381;
    int c;

    while (c = *str++)
        hash = ((hash << 5) + hash) + c; /* hash * 33 + c */

    return hash;
}

/*
this algorithm was created for sdbm (a public-domain reimplementation of ndbm) database library.
it was found to do well in scrambling bits, causing better distribution of the keys and fewer splits.
it also happens to be a good general hashing function with good distribution.
the actual function is hash(i) = hash(i - 1) * 65599 + str[i];
what is included below is the faster version used in gawk.
[there is even a faster, duff-device version]
the magic prime constant 65599 (2^6 + 2^16 - 1) was picked out of thin air
while experimenting with many different constants. this is one of the algorithms
used in berkeley db (see sleepycat) and elsewhere.
*/
unsigned long sdbm(const char * str) {
    unsigned long hash = 0;
    int c;

    while (c = *str++)
        hash = c + (hash << 16) + (hash << 6) - hash;

    return hash;
}

}