// Behaviour test for 00a3a840 CCompressorZlib::DecompressStream with a stub inflate: first use runs
// inflateInit once, Z_STREAM_END sets the end flag, an error raises ErrorFlag, and the result is the
// number of output bytes consumed.
#include <cstdio>

struct z_stream_s { unsigned char* next_out; unsigned int avail_out; };
static int g_inits = 0, g_result = 0, g_produce = 0;
static int inflateInit(z_stream_s*) { ++g_inits; return 0; }
static int z_inflate(z_stream_s* s, int flush) { if (flush != 2) return -99; s->avail_out -= g_produce; return g_result; }

struct CCompressorZlib {
    z_stream_s Stream; bool ZlibInitialised, Compressing, ErrorFlag;
    unsigned long DecompressStream(void* dest, unsigned long destSize, bool* end)
    {
        if (!ZlibInitialised) { inflateInit(&Stream); ZlibInitialised = true; Compressing = false; }
        Stream.next_out = (unsigned char*)dest; Stream.avail_out = destSize;
        int r = z_inflate(&Stream, 2);
        if (r == 1) { *end = true; return destSize - Stream.avail_out; }
        if (r != 0) ErrorFlag = true;
        *end = false;
        return destSize - Stream.avail_out;
    }
};

int main()
{
    unsigned char buf[64]; bool end = true;
    CCompressorZlib z; z.ZlibInitialised = false; z.Compressing = true; z.ErrorFlag = false;
    g_produce = 10; g_result = 0;
    bool ok = z.DecompressStream(buf, 64, &end) == 10 && !end && g_inits == 1 && z.ZlibInitialised && !z.Compressing && !z.ErrorFlag;
    g_produce = 64; g_result = 1;
    ok = ok && z.DecompressStream(buf, 64, &end) == 64 && end && g_inits == 1 && !z.ErrorFlag;
    g_produce = 0; g_result = -3;
    ok = ok && z.DecompressStream(buf, 64, &end) == 0 && !end && z.ErrorFlag;
    if (!ok) { std::printf("00a3a840_TEST FAIL\n"); return 1; }
    std::printf("00a3a840_TEST PASS\n");
    return 0;
}