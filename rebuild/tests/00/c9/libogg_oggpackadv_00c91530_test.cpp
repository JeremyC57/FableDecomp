// Behaviour test for 0x00C91530 libogg oggpack_adv (copy of the model + logic).
#include <cstdio>
struct oggpack_buffer { long endbyte; int endbit; unsigned char* buffer; unsigned char* ptr; long storage; };
static void adv(oggpack_buffer* b, int bits)
{
    bits += b->endbit;
    b->ptr += bits / 8;
    b->endbyte += bits / 8;
    b->endbit = bits & 7;
}
int main()
{
    unsigned char data[16];
    oggpack_buffer b; b.buffer = data; b.ptr = data + 1; b.endbyte = 1; b.endbit = 5; b.storage = 16;
    int ok = 1;
    adv(&b, 12);
    if (b.ptr != data + 3 || b.endbyte != 3 || b.endbit != 1) ok = 0;
    adv(&b, 7);
    if (b.ptr != data + 4 || b.endbyte != 4 || b.endbit != 0) ok = 0;
    adv(&b, 0);
    if (b.ptr != data + 4 || b.endbyte != 4 || b.endbit != 0) ok = 0;
    if (!ok) { std::printf("00c91530_TEST FAIL\n"); return 1; }
    std::printf("00c91530_TEST PASS\n");
    return 0;
}