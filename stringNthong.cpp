#include "stringNthong.h"
#include <ctype.h>
#include <stdlib.h>
#include <sys/types.h>

namespace mystr {

static size_t actual_len(const mystr_t *s) { return s && s->str ? (s->len ? s->len : strlen(s->str)) : 0; }

static unsigned long djb2_impl(const char *s) {
    unsigned long h = 5381;
    if (!s) return 0;
    for (unsigned char c; (c = (unsigned char)(*s++));) h = ((h << 5) + h) + c;
    return h ? h : 1;
}
static unsigned long hash_sdbm_impl(const char *s) {
    unsigned long h = 0;
    if (!s) return 1;
    for (unsigned char c; (c = (unsigned char)(*s++));) h = c + (h << 6) + (h << 16) - h;
    return h ? h : 1;
}

mystr_t construct(char *str) {
    mystr_t res {str, 0, 0};
    if (!str) return res;
    res.len = strlen(str);
    res.hash = djb2(&res);
    return res;
}

int put(const mystr_t *str) {
    if (!str || !str->str) return -1;
    return fputs(str->str, stdout) >= 0 ? 0 : -1;
}

const char * find_char(const mystr_t *str, char c) {
    return str && str->str ? strchr(str->str, c) : nullptr;
}

const char * find_last_char(const mystr_t *str, char c) {
    return str && str->str ? strrchr(str->str, c) : nullptr;
}

const char * find_any(const mystr_t *str, const mystr_t *accept) {
    return str && str->str && accept && accept->str ? strpbrk(str->str, accept->str) : nullptr;
}

size_t span(const mystr_t *str, const mystr_t *accept) {
    return str && str->str && accept && accept->str ? strspn(str->str, accept->str) : 0;
}

size_t cspan(const mystr_t *str, const mystr_t *reject) {
    return str && str->str && reject && reject->str ? strcspn(str->str, reject->str) : 0;
}

size_t len(const mystr_t *str) { return actual_len(str); }

mystr_t * copy(mystr_t *dst, const mystr_t *src) {
    if (!dst || !src || !dst->str || !src->str) return nullptr;
    strcpy(dst->str, src->str);
    dst->len = actual_len(src);
    dst->hash = djb2(dst);
    return dst;
}

mystr_t * ncopy(mystr_t *dst, const mystr_t *src, size_t count) {
    if (!count || !dst || !src || !dst->str || !src->str) return nullptr;
    strncpy(dst->str, src->str, count);
    size_t written = strnlen(src->str, count);
    if (written < count) dst->str[written] = '\0';
    dst->len = written;
    dst->hash = djb2(dst);
    return dst;
}

mystr_t * concat(mystr_t *dst, const mystr_t *src) {
    if (!dst || !src || !dst->str || !src->str) return nullptr;
    size_t l1 = actual_len(dst), l2 = actual_len(src);
    strcat(dst->str, src->str);
    dst->len = l1 + l2;
    dst->hash = djb2(dst);
    return dst;
}

mystr_t * nconcat(mystr_t *dst, const mystr_t *src, size_t count) {
    if (!count || !dst || !src || !dst->str || !src->str) return nullptr;
    strncat(dst->str, src->str, count);
    dst->len = strlen(dst->str);
    dst->hash = djb2(dst);
    return dst;
}

mystr_t dupe_concat(const mystr_t *first, const mystr_t *second) {
    size_t l1 = actual_len(first), l2 = actual_len(second);
    char *buf = (char *)(malloc(l1 + l2 + 1));
    if (!buf) return {nullptr, 0, 0};
    if (first && first->str) memcpy(buf, first->str, l1);
    if (second && second->str) memcpy(buf + l1, second->str, l2);
    buf[l1 + l2] = '\0';
    mystr_t out{buf, l1 + l2, 0};
    out.hash = djb2(&out);
    return out;
}

static mystr_t * read_line(mystr_t *str, FILE *stream, ssize_t limit) {
    if (!str || !stream) return nullptr;
    size_t cap = str->str ? actual_len(str) + 1 : 64;
    if (!cap) cap = 64;
    char *buf = str->str ? str->str : (char *)(malloc(cap));
    if (!buf) return nullptr;
    size_t i = 0;
    int ch;
    while ((ch = fgetc(stream)) != EOF) {
        if (limit >= 0 && static_cast<ssize_t>(i) >= limit) break;
        if (i + 1 >= cap) {
            cap <<= 1;
            char *tmp = (char *)(realloc(buf, cap));
            if (!tmp) {
                if (!str->str) free(buf);
                return nullptr;
            }
            buf = tmp;
        }
        buf[i++] = static_cast<char>(ch);
        if (ch == '\n') break;
    }
    if (ch == EOF && i == 0) {
        if (!str->str) free(buf);
        return nullptr;
    }
    buf[i] = '\0';
    str->str = buf;
    str->len = i;
    str->hash = djb2(str);
    return str;
}

mystr_t * fget(mystr_t *str, FILE *stream) {
    return read_line(str, stream, -1);
}

mystr_t * fnget(mystr_t *str, size_t count, FILE *stream) {
    if (!count) return nullptr;
    ssize_t limit = static_cast<ssize_t>(count);
    return read_line(str, stream, limit - 1);
}

mystr_t dupe(const mystr_t *str) {
    if (!str || !str->str) return {nullptr, 0, 0};
    char *copy_str = strdup(str->str);
    if (!copy_str) return {nullptr, 0, 0};
    mystr_t out{copy_str, actual_len(str), 0};
    out.hash = djb2(&out);
    return out;
}

ssize_t getline(mystr_t *line, FILE *stream) {
    if (!read_line(line, stream, -1)) return -1;
    return static_cast<ssize_t>(line->len);
}

int comp(const mystr_t *first, const mystr_t *second) {
    if (!first || !second || !first->str || !second->str) return 0;
    return strcmp(first->str, second->str);
}

int comp_to(const mystr_t *first, const mystr_t *second, char final) {
    if (!first || !second || !first->str || !second->str) return 0;
    const char *end1 = strchr(first->str, final);
    const char *end2 = strchr(second->str, final);
    size_t l1 = end1 ? (size_t)(end1 - first->str + 1) : actual_len(first);
    size_t l2 = end2 ? (size_t)(end2 - second->str + 1) : actual_len(second);
    size_t min_len = l1 < l2 ? l1 : l2;
    int res = strncmp(first->str, second->str, min_len);
    if (res) return res;
    if (l1 == l2) return 0;
    return l1 < l2 ? -1 : 1;
}

int ncomp(const mystr_t *first, const mystr_t *second, size_t size) {
    if (!first || !second || !first->str || !second->str) return 0;
    int res = strncmp(first->str, second->str, size);
    return res ? (res > 0 ? 1 : -1) : 0;
}

mystr_t mult(const mystr_t *src, size_t count) {
    if (!src || !src->str) return {nullptr, 0, 0};
    size_t l = actual_len(src);
    size_t total = l * count;
    char *buf = (char *)(malloc(total + 1));
    if (!buf) return {nullptr, 0, 0};
    char *dst_ptr = buf;
    for (size_t i = 0; i < count; ++i) {
        if (l) {
            memcpy(dst_ptr, src->str, l);
            dst_ptr += l;
        }
    }
    *dst_ptr = '\0';
    mystr_t out{buf, total, 0};
    out.hash = djb2(&out);
    return out;
}

const char * find_str(const mystr_t *haystack, const mystr_t *needle) {
    return haystack      &&
           needle        &&
           haystack->str &&
           needle->str ? strstr(haystack->str, needle->str) : nullptr;
}

ssize_t count_needle_in_haystack(const mystr_t *haystack, char needle) {
    if (!haystack || !haystack->str) return -1;
    ssize_t cnt = 0;
    const char *ptr = haystack->str;
    while ((ptr = strchr(ptr, needle))) {
        ++cnt;
        ++ptr;
    }
    return cnt;
}

ssize_t replace_needle_in_haystack(mystr_t *haystack, char src, char dst) {
    if (!haystack || !haystack->str) return -1;
    ssize_t cnt = 0;
    for (size_t i = 0; haystack->str[i]; ++i)
        if (haystack->str[i] == src) {
            haystack->str[i] = dst;
            ++cnt;
        }
    if (cnt) {
        haystack->len = strlen(haystack->str);
        haystack->hash = djb2(haystack);
    }
    return cnt;
}

size_t move_ptr_to_first_alpha_symbol(const mystr_t *str, size_t start, int backword) {
    size_t l = actual_len(str);
    if (!str || !str->str || !l) return l;
    if (start >= l) start = backword ? l - 1 : l;
    if (backword) {
        for (size_t i = start + 1; i-- > 0;) {
            if (isalpha((unsigned char)(str->str[i]))) return i;
            if (!i) break;
        }
        return l;
    }
    for (size_t i = start; i < l; ++i)
        if (isalpha((unsigned char)(str->str[i]))) return i;
    return l;
}

size_t move_ptr_to_first_not_space_symbol(const mystr_t *str, size_t start, int backword) {
    size_t l = actual_len(str);
    if (!str || !str->str || !l) return l;
    if (start >= l) start = backword ? l - 1 : l;
    if (backword) {
        for (size_t i = start + 1; i-- > 0;) {
            if (!isspace((unsigned char)(str->str[i]))) return i;
            if (!i) break;
        }
        return l;
    }
    for (size_t i = start; i < l; ++i)
        if (!isspace((unsigned char)(str->str[i]))) return i;
    return l;
}

int is_not_empty(const mystr_t *str) {
    return str && str->str && (str->len ? str->len : strlen(str->str));
}

unsigned long djb2(const mystr_t *str) {
    return djb2_impl(str && str->str ? str->str : nullptr);
}

unsigned long sdbm(const mystr_t *str) {
    return hash_sdbm_impl(str && str->str ? str->str : nullptr);
}

}
