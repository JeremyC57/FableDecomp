#include <cstdio>
#include <stddef.h>
static int g_asserts = 0; static void __cdecl AssertHook() { ++g_asserts; }
void (__cdecl* g_pfnAssertionFailed)() = 0;
#define FABLE_ASSERT(cond) do { if (!(cond)) { if (g_pfnAssertionFailed != 0) g_pfnAssertionFailed(); } } while (0)
static unsigned long g_produce = 0; static void* g_dest = 0; static unsigned long g_destSize = 0; static bool* g_flag = 0;
struct CCompressorZlib_Calls { unsigned long DecompressStream(void* dest, unsigned long destSize, bool* endOfStream) { g_dest = dest; g_destSize = destSize; g_flag = endOfStream; return g_produce; } };
template<class T> struct CScopedPointer { T* ptr; T* operator->() const { return ptr; } };
struct CCompressedMemoryDataInputStream_Retail {
    virtual void v00() { }
    unsigned char _state[0x14]; unsigned char* Buffer; long BufferSize; CScopedPointer<CCompressorZlib_Calls> Compressor;
    void InternalReadNextSourceChunk(unsigned char** chunkData, unsigned long* chunkSize)
    { bool endOfStream = false; unsigned long bytesProduced = Compressor->DecompressStream(Buffer, BufferSize, &endOfStream); FABLE_ASSERT(bytesProduced > 0); *chunkData = Buffer; *chunkSize = bytesProduced; }
};
int main()
{
    CCompressorZlib_Calls z; CCompressedMemoryDataInputStream_Retail s; unsigned char buffer[32]; s.Buffer = buffer; s.BufferSize = 32; s.Compressor.ptr = &z;
    unsigned char* data = 0; unsigned long size = 0; g_produce = 12;
    s.InternalReadNextSourceChunk(&data, &size);
    if (data != buffer || size != 12 || g_dest != buffer || g_destSize != 32 || g_flag == 0 || *g_flag || g_asserts != 0 || offsetof(CCompressedMemoryDataInputStream_Retail, Compressor) != 0x20) { std::printf("009949f0_TEST FAIL\n"); return 1; }
    g_produce = 0; s.InternalReadNextSourceChunk(&data, &size);
    if (g_asserts != 0 || size != 0) { std::printf("009949f0_TEST FAIL\n"); return 1; }   // no hook installed
    g_pfnAssertionFailed = AssertHook; s.InternalReadNextSourceChunk(&data, &size);
    if (g_asserts != 1) { std::printf("009949f0_TEST FAIL\n"); return 1; }
    std::printf("009949f0_TEST PASS\n"); return 0;
}