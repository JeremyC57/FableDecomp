#include <cstdio>
#include <stddef.h>
#include <string.h>
struct z_stream_s { unsigned char* next_in; unsigned int avail_in; unsigned long total_in; unsigned char* next_out; unsigned int avail_out; unsigned long total_out; char* msg; void* state;
    void* (__cdecl* zalloc)(void*, unsigned int, unsigned int); void (__cdecl* zfree)(void*, void*); void* opaque; int data_type; unsigned long adler; unsigned long reserved; };
static int g_inits = 0, g_deflates = 0, g_level = 0, g_streamSize = 0, g_flush = 99; static const char* g_version = 0;
int __cdecl deflateInit_(z_stream_s*, int level, const char* version, int stream_size) { ++g_inits; g_level = level; g_version = version; g_streamSize = stream_size; return 0; }
int __cdecl deflate(z_stream_s* strm, int flush) { ++g_deflates; g_flush = flush; strm->avail_out -= 7; return 0; }
#define ZLIB_VERSION "1.1.4"
#define deflateInit(strm, level) deflateInit_((strm), (level), ZLIB_VERSION, sizeof(z_stream_s))
struct CCompressorZlib_Retail {
    virtual void v00() { }
    unsigned char _base[0x18]; z_stream_s Stream; bool ZlibInitialised; bool Compressing; bool ErrorFlag;
    unsigned long CompressStream(void* dest, unsigned long destSize)
    {
        if (!ZlibInitialised) { deflateInit(&Stream, 9); ZlibInitialised = true; Compressing = true; }
        Stream.next_out = (unsigned char*)dest; Stream.avail_out = destSize; deflate(&Stream, 0); return destSize - Stream.avail_out;
    }
};
int main()
{
    CCompressorZlib_Retail c; memset(&c.Stream, 0, sizeof(c.Stream)); c.ZlibInitialised = false; c.Compressing = false; unsigned char out[64];
    unsigned long n = c.CompressStream(out, 64);
    if (n != 7 || g_inits != 1 || g_level != 9 || g_streamSize != 56 || strcmp(g_version, "1.1.4") != 0 || g_deflates != 1 || g_flush != 0 || c.Stream.next_out != out || !c.ZlibInitialised || !c.Compressing
        || offsetof(CCompressorZlib_Retail, Stream) != 0x1c || offsetof(CCompressorZlib_Retail, ZlibInitialised) != 0x54) { std::printf("00a3a7f0_TEST FAIL\n"); return 1; }
    n = c.CompressStream(out, 32);
    if (n != 7 || g_inits != 1 || g_deflates != 2) { std::printf("00a3a7f0_TEST FAIL\n"); return 1; }
    std::printf("00a3a7f0_TEST PASS\n"); return 0;
}
