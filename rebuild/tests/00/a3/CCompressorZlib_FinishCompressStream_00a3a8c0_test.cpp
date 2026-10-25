// Behaviour test for 00a3a8c0 CCompressorZlib::FinishCompressStream with a stub deflate: Z_FINISH is
// requested, the stream end tears the deflate state down and clears both flags, an error raises
// ErrorFlag, and the result is the number of output bytes written.
#include <cstdio>

struct z_stream_s { unsigned char* next_out; unsigned int avail_out; };
static int g_result = 0, g_produce = 0, g_ends = 0;
static int z_deflate(z_stream_s* s, int flush) { if (flush != 4) return -99; s->avail_out -= g_produce; return g_result; }
static int z_deflateEnd(z_stream_s*) { ++g_ends; return 0; }

struct CCompressorZlib {
    z_stream_s Stream; bool ZlibInitialised, Compressing, ErrorFlag;
    unsigned long FinishCompressStream(void* dest, unsigned long destSize, bool* finished)
    {
        Stream.next_out = (unsigned char*)dest; Stream.avail_out = destSize;
        int r = z_deflate(&Stream, 4);
        if (r == 1) { *finished = true; z_deflateEnd(&Stream); ZlibInitialised = false; Compressing = false; *finished = true; return destSize - Stream.avail_out; }
        if (r != 0) ErrorFlag = true;
        *finished = false;
        return destSize - Stream.avail_out;
    }
};

int main()
{
    unsigned char buf[32]; bool fin = true;
    CCompressorZlib z; z.ZlibInitialised = true; z.Compressing = true; z.ErrorFlag = false;
    g_result = 0; g_produce = 32;
    bool ok = z.FinishCompressStream(buf, 32, &fin) == 32 && !fin && z.ZlibInitialised && g_ends == 0;
    g_result = 1; g_produce = 5;
    ok = ok && z.FinishCompressStream(buf, 32, &fin) == 5 && fin && !z.ZlibInitialised && !z.Compressing && g_ends == 1 && !z.ErrorFlag;
    g_result = -2; g_produce = 0;
    ok = ok && z.FinishCompressStream(buf, 32, &fin) == 0 && !fin && z.ErrorFlag;
    if (!ok) { std::printf("00a3a8c0_TEST FAIL\n"); return 1; }
    std::printf("00a3a8c0_TEST PASS\n");
    return 0;
}