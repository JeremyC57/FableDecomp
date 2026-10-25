// Behaviour test for 0x00C913A0 libogg oggpack_look (copy of the model + logic, 32-bit word semantics).
#include <cstdio>
typedef unsigned int u32;
struct oggpack_buffer { long endbyte; int endbit; unsigned char* buffer; unsigned char* ptr; long storage; };
static u32 mask[33];
static long look(oggpack_buffer* b, int bits)
{
    u32 ret;
    u32 m = mask[bits];
    bits += b->endbit;
    if (b->endbyte + 4 >= b->storage) {
        if (b->endbyte * 8 + bits > b->storage * 8) return -1;
    }
    ret = b->ptr[0] >> b->endbit;
    if (bits > 8) {
        ret |= (u32)b->ptr[1] << (8 - b->endbit);
        if (bits > 16) {
            ret |= (u32)b->ptr[2] << (16 - b->endbit);
            if (bits > 24) {
                ret |= (u32)b->ptr[3] << (24 - b->endbit);
                if (bits > 32 && b->endbit) ret |= (u32)b->ptr[4] << (32 - b->endbit);
            }
        }
    }
    return (long)(m & ret);
}
int main()
{
    int i, ok = 1;
    for (i = 0; i < 32; i++) mask[i] = (1u << i) - 1u;
    mask[32] = 0xffffffffu;
    unsigned char data[8] = { 0x12, 0x34, 0x56, 0x78, 0x9A, 0xBC, 0xDE, 0xF0 };
    oggpack_buffer b; b.buffer = data; b.ptr = data; b.endbyte = 0; b.endbit = 4; b.storage = 8;
    if (look(&b, 8) != 0x41) ok = 0;
    if ((u32)look(&b, 32) != 0xA7856341u) ok = 0;            /* straddles into the fifth byte */
    if (b.endbyte != 0 || b.endbit != 4) ok = 0;             /* no advance */
    b.endbit = 0;
    if (look(&b, 12) != 0x412) ok = 0;
    b.endbyte = 6; b.ptr = data + 6; b.endbit = 0;
    if (look(&b, 16) != 0xF0DE) ok = 0;                      /* exactly to the end */
    if (look(&b, 17) != -1) ok = 0;                          /* one bit past the end */
    if (!ok) { std::printf("00c913a0_TEST FAIL\n"); return 1; }
    std::printf("00c913a0_TEST PASS\n");
    return 0;
}