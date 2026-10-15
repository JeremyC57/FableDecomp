// Behaviour test for 00a3ab00 CCompressorZlib::InternalDecompress with stub zlib calls: an empty source
// returns 0 untouched, the first call installs the allocators and initialises, later calls reset, and
// the static stream's total_out is returned.
#include <cstdio>

struct z_stream_s { const unsigned char* next_in; unsigned int avail_in; unsigned char* next_out; unsigned int avail_out; unsigned long total_out; void* zalloc; void* zfree; };
static struct { z_stream_s Stream; bool Initialised; } g_state;
static int g_inits = 0, g_resets = 0, g_inflates = 0;
static int s_allocTag, s_freeTag;
static int inflateInit(z_stream_s*) { ++g_inits; return 0; }
static int inflateReset(z_stream_s* s) { ++g_resets; s->total_out = 0; return 0; }
static int inflate(z_stream_s* s, int flush) { ++g_inflates; if (flush == 4) s->total_out += s->avail_in * 2; return 1; }

static unsigned long InternalDecompress(const void* src, unsigned long srcSize, void* dest, unsigned long destSize)
{
    if (srcSize == 0) return 0;
    g_state.Stream.next_in = (const unsigned char*)src; g_state.Stream.avail_in = srcSize;
    g_state.Stream.next_out = (unsigned char*)dest; g_state.Stream.avail_out = destSize;
    if (!g_state.Initialised) { g_state.Stream.zalloc = &s_allocTag; g_state.Stream.zfree = &s_freeTag; inflateInit(&g_state.Stream); g_state.Initialised = true; }
    else inflateReset(&g_state.Stream);
    inflate(&g_state.Stream, 4);
    return g_state.Stream.total_out;
}

int main()
{
    unsigned char in[8], out[32];
    bool ok = InternalDecompress(in, 0, out, 32) == 0 && g_inflates == 0;
    ok = ok && InternalDecompress(in, 8, out, 32) == 16 && g_inits == 1 && g_resets == 0 && g_state.Stream.zalloc == &s_allocTag;
    ok = ok && InternalDecompress(in, 4, out, 32) == 8 && g_inits == 1 && g_resets == 1 && g_state.Stream.next_out == out;
    if (!ok) { std::printf("00a3ab00_TEST FAIL\n"); return 1; }
    std::printf("00a3ab00_TEST PASS\n");
    return 0;
}