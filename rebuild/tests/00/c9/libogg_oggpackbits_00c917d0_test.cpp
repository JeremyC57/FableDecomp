#include <cstdio>
// oggpack_bits: endbyte*8 + endbit.
struct oggpack_buffer { int endbyte; int endbit; unsigned char* buffer; unsigned char* ptr; int storage; };
static int oggpack_bits(oggpack_buffer* b) { return (b->endbyte * 8 + b->endbit); }
int main()
{
    oggpack_buffer b; b.endbyte = 13; b.endbit = 5; b.buffer = 0; b.ptr = 0; b.storage = 0;
    if (oggpack_bits(&b) != 109) { std::printf("00c917d0_TEST FAIL\n"); return 1; }
    b.endbyte = 0; b.endbit = 0;
    if (oggpack_bits(&b) != 0) { std::printf("00c917d0_TEST FAIL\n"); return 1; }
    std::printf("00c917d0_TEST PASS\n");
    return 0;
}