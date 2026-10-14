#include <cstdio>
#include <stddef.h>
static int g_asserts = 0; static void __cdecl AssertHook() { ++g_asserts; }
void (__cdecl* g_pfnAssertionFailed)() = AssertHook;
#define FABLE_ASSERT(cond) do { if (!(cond)) { if (g_pfnAssertionFailed != 0) g_pfnAssertionFailed(); } } while (0)
static unsigned long g_produce = 0; static void* g_dest = 0; static unsigned long g_destSize = 0;
struct CCompressorZlib_Calls { unsigned long DecompressStream(void* dest, unsigned long destSize, bool* endOfStream) { g_dest = dest; g_destSize = destSize; *endOfStream = false; return g_produce; } };
struct CCompressedMemoryDataInputStream_Retail {
    virtual void v00() { }
    unsigned char _state[0x14]; unsigned char* Buffer; long BufferSize; CCompressorZlib_Calls* Compressor_ptr;
    void InternalReadUnbuffered(unsigned char* dest, long size) { bool endOfStream = false; unsigned long bytesProduced = Compressor_ptr->DecompressStream(dest, size, &endOfStream); FABLE_ASSERT(bytesProduced == (unsigned long)size); }
};
int main()
{
    CCompressorZlib_Calls z; CCompressedMemoryDataInputStream_Retail s; s.Compressor_ptr = &z; unsigned char out[16];
    g_produce = 16; s.InternalReadUnbuffered(out, 16);
    if (g_dest != out || g_destSize != 16 || g_asserts != 0) { std::printf("00994a40_TEST FAIL\n"); return 1; }
    g_produce = 15; s.InternalReadUnbuffered(out, 16);
    if (g_asserts != 1) { std::printf("00994a40_TEST FAIL\n"); return 1; }
    g_pfnAssertionFailed = 0; s.InternalReadUnbuffered(out, 16);
    if (g_asserts != 1) { std::printf("00994a40_TEST FAIL\n"); return 1; }
    std::printf("00994a40_TEST PASS\n"); return 0;
}