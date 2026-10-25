// Behaviour test for 00c911b0: libogg oggpack_writealign -- pads with zero bits to the next byte, no-op when already aligned
#include <cstdio>
#include <cstdlib>
#include <cstring>
struct oggpack_buffer { long endbyte; int endbit; unsigned char* buffer; unsigned char* ptr; long storage; };
static unsigned long mask[33];
static void init_mask() { for (int i = 0; i < 32; i++) mask[i] = (1ul << i) - 1ul; mask[32] = 0xfffffffful; }
static void oggpack_writeinit(oggpack_buffer* b)
{
    std::memset(b, 0, sizeof(*b));
    b->ptr = b->buffer = (unsigned char*)std::malloc(256);
    b->buffer[0] = '\0';
    b->storage = 256;
}
static void oggpack_write(oggpack_buffer* b, unsigned long value, int bits)
{
    if (b->endbyte + 4 >= b->storage) {
        b->buffer = (unsigned char*)std::realloc(b->buffer, b->storage + 256);
        b->storage += 256;
        b->ptr = b->buffer + b->endbyte;
    }
    value &= mask[bits];
    bits += b->endbit;
    b->ptr[0] |= value << b->endbit;
    if (bits >= 8) {
        b->ptr[1] = (unsigned char)(value >> (8 - b->endbit));
        if (bits >= 16) {
            b->ptr[2] = (unsigned char)(value >> (16 - b->endbit));
            if (bits >= 24) {
                b->ptr[3] = (unsigned char)(value >> (24 - b->endbit));
                if (bits >= 32) {
                    if (b->endbit) b->ptr[4] = (unsigned char)(value >> (32 - b->endbit));
                    else b->ptr[4] = 0;
                }
            }
        }
    }
    b->endbyte += bits / 8;
    b->ptr += bits / 8;
    b->endbit = bits & 7;
}
static void oggpack_writealign(oggpack_buffer* b) { int bits = 8 - b->endbit; if (bits < 8) oggpack_write(b, 0, bits); }
static void oggpack_reset(oggpack_buffer* b) { b->ptr = b->buffer; b->buffer[0] = 0; b->endbit = b->endbyte = 0; }
static void oggpack_writeclear(oggpack_buffer* b) { std::free(b->buffer); std::memset(b, 0, sizeof(*b)); }
static void oggpack_readinit(oggpack_buffer* b, unsigned char* buf, int bytes) { std::memset(b, 0, sizeof(*b)); b->buffer = b->ptr = buf; b->storage = bytes; }
static long oggpack_read(oggpack_buffer* b, int bits)
{
    unsigned long ret;
    unsigned long m = mask[bits];
    bits += b->endbit;
    if (b->endbyte + 4 >= b->storage) {
        ret = (unsigned long)-1;
        if (b->endbyte * 8 + bits > b->storage * 8) goto overflow;
    }
    ret = b->ptr[0] >> b->endbit;
    if (bits > 8) {
        ret |= b->ptr[1] << (8 - b->endbit);
        if (bits > 16) {
            ret |= b->ptr[2] << (16 - b->endbit);
            if (bits > 24) {
                ret |= b->ptr[3] << (24 - b->endbit);
                if (bits > 32 && b->endbit) ret |= b->ptr[4] << (32 - b->endbit);
            }
        }
    }
    ret &= m;
overflow:
    b->ptr += bits / 8;
    b->endbyte += bits / 8;
    b->endbit = bits & 7;
    return (long)ret;
}
int main()
{
    init_mask();
    int ok = 1;
    oggpack_buffer b; oggpack_writeinit(&b);
    oggpack_writealign(&b); ok &= b.endbyte == 0 && b.endbit == 0;
    oggpack_write(&b, 0x7, 3); oggpack_writealign(&b);
    ok &= b.endbyte == 1 && b.endbit == 0 && b.buffer[0] == 0x7;
    oggpack_write(&b, 0xFF, 8); oggpack_writealign(&b); ok &= b.endbyte == 2 && b.endbit == 0;
    oggpack_writeclear(&b);
    std::printf(ok ? "00c911b0_TEST PASS\n" : "00c911b0_TEST FAIL\n");
    return ok ? 0 : 1;
}
