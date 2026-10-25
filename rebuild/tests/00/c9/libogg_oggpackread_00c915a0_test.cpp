// Behaviour test for 00c915a0: libogg oggpack_read -- LSb-first reads, 32-bit reads at an odd bit offset, -1 past the end
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
    unsigned char data[8] = { 0xA5, 0x3C, 0xFF, 0x00, 0x81, 0x7E, 0x12, 0x34 };
    oggpack_buffer r; oggpack_readinit(&r, data, 8);
    ok &= oggpack_read(&r, 4) == 0x5 && oggpack_read(&r, 4) == 0xA && oggpack_read(&r, 1) == 0 && r.endbit == 1;
    ok &= (unsigned long)oggpack_read(&r, 32) == ((0x3Cul >> 1) | (0xFFul << 7) | (0x00ul << 15) | (0x81ul << 23) | ((0x7Eul & 1) << 31));
    ok &= r.endbyte == 5 && r.endbit == 1;
    ok &= oggpack_read(&r, 16) == (long)((((0x7Eul >> 1) | (0x12ul << 7) | (0x34ul << 15))) & 0xFFFF);
    ok &= oggpack_read(&r, 16) == -1;
    std::printf(ok ? "00c915a0_TEST PASS\n" : "00c915a0_TEST FAIL\n");
    return ok ? 0 : 1;
}
