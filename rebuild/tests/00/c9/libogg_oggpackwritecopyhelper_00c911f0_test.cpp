// Behaviour test for libogg bitwise.c oggpack_writecopy_helper (0x00C911F0): byte-wise copy when unaligned, block copy when aligned.
#include <cstdio>
#include <cstdlib>
#include <cstring>
struct opb { long endbyte; int endbit; unsigned char* buffer; unsigned char* ptr; long storage; };
static int g_calls; static unsigned long g_last; static int g_lastbits;
static void w(opb*, unsigned long v, int bits) { g_calls++; g_last = v; g_lastbits = bits; }
static void writecopy_helper(opb* b, void* source, long bits, void (*wf)(opb*, unsigned long, int), int msb)
{
    unsigned char* ptr = (unsigned char*)source; long bytes = bits / 8; bits -= bytes * 8;
    if (b->endbit) { for (int i = 0; i < bytes; i++) wf(b, (unsigned long)ptr[i], 8); }
    else {
        if (b->endbyte + bytes + 1 >= b->storage) {
            b->storage = b->endbyte + bytes + 256; b->buffer = (unsigned char*)std::realloc(b->buffer, b->storage); b->ptr = b->buffer + b->endbyte;
        }
        std::memmove(b->ptr, source, bytes); b->ptr += bytes; b->buffer += bytes; *b->ptr = 0;
    }
    if (bits) { if (msb) wf(b, (unsigned long)(ptr[bytes] >> (8 - bits)), bits); else wf(b, (unsigned long)ptr[bytes], bits); }
}
int main()
{
    unsigned char src[3] = { 0x11, 0x22, 0xf0 };
    opb u = { 0, 3, 0, 0, 0 };
    writecopy_helper(&u, src, 20, w, 1);
    if (g_calls != 3 || g_last != 0x0f || g_lastbits != 4) { std::printf("00c911f0_TEST FAIL\n"); return 1; }
    unsigned char* mem = (unsigned char*)std::malloc(4);
    opb a = { 0, 0, mem, mem, 4 }; g_calls = 0;
    writecopy_helper(&a, src, 16, w, 0);
    if (g_calls != 0 || a.storage != 4 || a.ptr[-2] != 0x11 || a.ptr[-1] != 0x22 || a.ptr[0] != 0) { std::printf("00c911f0_TEST FAIL\n"); return 1; }
    std::printf("00c911f0_TEST PASS\n"); return 0;
}