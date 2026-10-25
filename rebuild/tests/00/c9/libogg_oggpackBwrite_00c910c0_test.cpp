// Behaviour test for 0x00C910C0 libogg oggpackB_write (copy of the model + logic, 32-bit word semantics).
#include <cstdio>
#include <cstdlib>
typedef unsigned int u32;
struct oggpack_buffer { long endbyte; int endbit; unsigned char* buffer; unsigned char* ptr; long storage; };
static u32 mask[33];
static void writeB(oggpack_buffer* b, u32 value, int bits)
{
    if (b->endbyte + 4 >= b->storage) {
        b->buffer = (unsigned char*)std::realloc(b->buffer, b->storage + 256);
        b->storage += 256;
        b->ptr = b->buffer + b->endbyte;
    }
    value = (value & mask[bits]) << (32 - bits);
    bits += b->endbit;
    b->ptr[0] |= (unsigned char)(value >> (24 + b->endbit));
    if (bits >= 8) {
        b->ptr[1] = (unsigned char)(value >> (16 + b->endbit));
        if (bits >= 16) {
            b->ptr[2] = (unsigned char)(value >> (8 + b->endbit));
            if (bits >= 24) {
                b->ptr[3] = (unsigned char)(value >> (b->endbit));
                if (bits >= 32) {
                    if (b->endbit) b->ptr[4] = (unsigned char)(value << (8 - b->endbit));
                    else b->ptr[4] = 0;
                }
            }
        }
    }
    b->endbyte += bits / 8;
    b->ptr += bits / 8;
    b->endbit = bits & 7;
}
int main()
{
    int i, ok = 1;
    for (i = 0; i < 32; i++) mask[i] = (1u << i) - 1u;
    mask[32] = 0xffffffffu;
    oggpack_buffer b;
    b.buffer = (unsigned char*)std::malloc(256); b.buffer[0] = 0; b.ptr = b.buffer; b.storage = 256; b.endbyte = 0; b.endbit = 0;
    writeB(&b, 0x5, 3);            /* 101 */
    writeB(&b, 0xF1, 1);           /* only the low bit (1) */
    writeB(&b, 0xABCD, 16);
    if (b.buffer[0] != 0xBA || b.buffer[1] != 0xBC || (b.buffer[2] & 0xF0) != 0xD0 || b.endbyte != 2 || b.endbit != 4) ok = 0;
    /* fill past the first 256 bytes with whole words: the buffer must grow and keep its contents */
    oggpack_buffer c;
    c.buffer = (unsigned char*)std::malloc(256); c.buffer[0] = 0; c.ptr = c.buffer; c.storage = 256; c.endbyte = 0; c.endbit = 0;
    for (i = 0; i < 70; i++) writeB(&c, 0x01020304u + (u32)i, 32);
    if (c.storage != 512 || c.endbyte != 280 || c.endbit != 0) ok = 0;
    for (i = 0; i < 70 && ok; i++) {
        u32 w = 0x01020304u + (u32)i;
        if (c.buffer[4 * i] != (w >> 24) || c.buffer[4 * i + 1] != ((w >> 16) & 0xff) ||
            c.buffer[4 * i + 2] != ((w >> 8) & 0xff) || c.buffer[4 * i + 3] != (w & 0xff)) ok = 0;
    }
    /* a 32-bit write at a non-zero bit offset spills into the fifth byte */
    writeB(&c, 0x1, 1);
    writeB(&c, 0x80000001u, 32);
    if (c.buffer[280] != 0xC0 || c.buffer[281] != 0 || c.buffer[284] != 0x80 || c.endbyte != 284 || c.endbit != 1) ok = 0;
    std::free(b.buffer); std::free(c.buffer);
    if (!ok) { std::printf("00c910c0_TEST FAIL\n"); return 1; }
    std::printf("00c910c0_TEST PASS\n");
    return 0;
}