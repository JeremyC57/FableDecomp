#include <cstdio>
// oggpack_adv1: one bit forward, carrying into the next byte after bit 7.
struct oggpack_buffer { int endbyte; int endbit; unsigned char* buffer; unsigned char* ptr; int storage; };
static void oggpack_adv1(oggpack_buffer* b)
{
    if (++(b->endbit) > 7) { b->endbit = 0; b->ptr++; b->endbyte++; }
}
int main()
{
    unsigned char buf[4];
    oggpack_buffer b; b.endbyte = 0; b.endbit = 6; b.buffer = buf; b.ptr = buf; b.storage = 4;
    oggpack_adv1(&b);
    if (b.endbit != 7 || b.endbyte != 0 || b.ptr != buf) { std::printf("00c91570_TEST FAIL\n"); return 1; }
    oggpack_adv1(&b);
    if (b.endbit != 0 || b.endbyte != 1 || b.ptr != buf + 1) { std::printf("00c91570_TEST FAIL\n"); return 1; }
    std::printf("00c91570_TEST PASS\n");
    return 0;
}