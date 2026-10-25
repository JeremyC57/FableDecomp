// Behaviour test for 0x00C917B0 libogg oggpack_bytes (copy of the model + logic).
#include <cstdio>
struct oggpack_buffer { long endbyte; int endbit; unsigned char* buffer; unsigned char* ptr; long storage; };
static long bytes(oggpack_buffer* b) { return b->endbyte + (b->endbit + 7) / 8; }
int main()
{
    oggpack_buffer b; b.buffer = 0; b.ptr = 0; b.storage = 0;
    int ok = 1;
    b.endbyte = 3; b.endbit = 0; if (bytes(&b) != 3) ok = 0;
    b.endbit = 1; if (bytes(&b) != 4) ok = 0;
    b.endbit = 7; if (bytes(&b) != 4) ok = 0;
    b.endbyte = 0; b.endbit = 0; if (bytes(&b) != 0) ok = 0;
    if (!ok) { std::printf("00c917b0_TEST FAIL\n"); return 1; }
    std::printf("00c917b0_TEST PASS\n");
    return 0;
}