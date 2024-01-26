#include <stdint.h>
#include <stdlib.h>

// reverse memcpy
void *rmemcpy(void *dest, const void *src, size_t n) {
    uint8_t *dest_start = (uint8_t *)dest + n;
    uint8_t *src_start = (uint8_t *)src;
    while (n-- > 0) *(--dest_start) = *(src_start++);
    return dest;
}