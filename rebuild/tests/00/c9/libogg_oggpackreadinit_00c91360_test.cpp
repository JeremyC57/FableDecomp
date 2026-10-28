// Test: oggpack_readinit -- the bit-packer is zeroed, then reads start at buf with bytes of storage.
#include <cstdio>
#include <cstring>
struct oggpack_buffer { int endbyte; int endbit; unsigned char* buffer; unsigned char* ptr; int storage; };
static void oggpack_readinit(oggpack_buffer* b, unsigned char* buf, int bytes) { std::memset(b, 0, sizeof(*b)); b->buffer = b->ptr = buf; b->storage = bytes; }
int main()
{
    unsigned char data[16]; oggpack_buffer b; std::memset(&b, 0x5A, sizeof b);
    oggpack_readinit(&b, data, 16);
    bool ok = b.endbyte == 0 && b.endbit == 0 && b.buffer == data && b.ptr == data && b.storage == 16;
    std::printf(ok ? "00c91360_TEST PASS\n" : "00c91360_TEST FAIL\n");
    return ok ? 0 : 1;
}