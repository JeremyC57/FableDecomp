#include <cstdio>
// Behaviour test for CCompressorZlib::InternalCompress @ 0x00A3AA40 against a mock byte-copy deflate.
struct z_stream {
    unsigned char* next_in; unsigned int avail_in; unsigned long total_in;
    unsigned char* next_out; unsigned int avail_out; unsigned long total_out;
    void* zalloc; void* zfree; void* opaque;
};
static int g_inits = 0, g_ends = 0, g_finishCalls = 0;
static int deflateInit_(z_stream* s, int level, const char*, int) { s->total_in = s->total_out = 0; ++g_inits; return level == 1 ? 0 : -1; }
static int deflate(z_stream* s, int flush)
{
    if (flush == 0) {
        if (s->avail_in && s->avail_out) { *s->next_out++ = (unsigned char)(*s->next_in++ ^ 0x5a); --s->avail_in; --s->avail_out; ++s->total_in; ++s->total_out; }
        return 0;
    }
    ++g_finishCalls;
    if (g_finishCalls < 3) return 0;   // pretend two partial flushes
    *s->next_out++ = 0xff; ++s->total_out; return 1;
}
static void deflateEnd(z_stream*) { ++g_ends; }
static unsigned long InternalCompress(const void* source, unsigned long sourceSize, void* dest, unsigned long destSize)
{
    if (sourceSize == 0) return 0;
    z_stream stream; stream.zalloc = 0; stream.zfree = 0; stream.opaque = 0;
    deflateInit_(&stream, 1, "1.1.4", (int)sizeof(z_stream));
    stream.next_in = (unsigned char*)source; stream.next_out = (unsigned char*)dest;
    while (stream.total_in != sourceSize && stream.total_out < destSize) { stream.avail_in = stream.avail_out = 1; deflate(&stream, 0); }
    for (;;) { stream.avail_out = 1; if (deflate(&stream, 4) == 1) break; }
    deflateEnd(&stream);
    return stream.total_out;
}
static int Fail() { std::printf("00a3aa40_TEST FAIL\n"); return 1; }
int main()
{
    unsigned char src[5] = {1, 2, 3, 4, 5}; unsigned char dst[16] = {0};
    if (InternalCompress(src, 0, dst, 16) != 0 || g_inits != 0) return Fail();
    unsigned long n = InternalCompress(src, 5, dst, 16);
    if (n != 6 || dst[0] != (1 ^ 0x5a) || dst[4] != (5 ^ 0x5a) || dst[5] != 0xff || g_ends != 1 || g_finishCalls != 3) return Fail();
    g_finishCalls = 2;
    n = InternalCompress(src, 5, dst, 3);   // output-limited pump stops at destSize
    if (n != 4 || dst[3] != 0xff) return Fail();
    std::printf("00a3aa40_TEST PASS\n");
    return 0;
}