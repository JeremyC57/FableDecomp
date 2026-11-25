// Behaviour test for 0x00A3AA40 CCompressorZlib::InternalCompress: deflateInit_ at level 1 with zlib's version "1.2.1" (retail
// 0x0129CFE0) and this class's allocators, byte-by-byte deflate until the input is consumed or the output is full, then Z_FINISH until
// Z_STREAM_END; returns total_out (0 for empty input).
// NF420 (2026-11-25): LINKED test -- it links the row object (the row function is declared here, not defined); the zlib entry points and
// the allocators are defined here under the row's spellings, so the test asserts the version string and the stream protocol; a mutated
// literal fails.
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
extern "C" void __cdecl NF420TrapHit(const char* sym) { printf("FAIL unexpected call %s\n", sym); exit(3); }
extern "C" char NF420TrapData[4096] = { 0 };
typedef unsigned char Bytef; typedef unsigned int uInt; typedef unsigned long uLong;
typedef void* (__cdecl *alloc_func)(void* opaque, uInt items, uInt size);
typedef void  (__cdecl *free_func)(void* opaque, void* address);
struct internal_state;
struct z_stream { Bytef* next_in; uInt avail_in; uLong total_in; Bytef* next_out; uInt avail_out; uLong total_out; char* msg;
                  internal_state* state; alloc_func zalloc; free_func zfree; void* opaque; int data_type; uLong adler; uLong reserved; };
class CCompressorZlib { public:
private: static void* __cdecl Alloc(void* opaque, uInt items, uInt size);   // private static, as the row declares them
    static void  __cdecl Free(void* opaque, void* address); public:
    static alloc_func AllocFn() { return Alloc; }                         // test-side inline accessors (no symbol of their own is used)
    static free_func FreeFn() { return Free; }
    unsigned long InternalCompress(const void* source, unsigned long sourceSize, void* dest, unsigned long destSize);
};
static char g_version[16]; static int g_level = -1, g_size = 0, g_inits = 0, g_ends = 0, g_finish = 0, g_bad = 0;
extern "C" int __cdecl z_deflateInit_(z_stream* strm, int level, const char* version, int stream_size)
{ strcpy(g_version, version); g_level = level; g_size = stream_size; ++g_inits; strm->total_in = strm->total_out = 0;
  if (strm->zalloc != CCompressorZlib::AllocFn() || strm->zfree != CCompressorZlib::FreeFn() || strm->opaque) g_bad = 1; return 0; }
extern "C" int __cdecl z_deflate(z_stream* strm, int flush)
{
    if (flush == 4) { if (++g_finish == 2) return 1; *strm->next_out++ = 0xFF; ++strm->total_out; return 0; }
    if (strm->avail_in != 1 || strm->avail_out != 1) g_bad = 1;
    *strm->next_out++ = (Bytef)(*strm->next_in++ ^ 0x20); ++strm->total_in; ++strm->total_out; return 0;
}
extern "C" int __cdecl z_deflateEnd(z_stream*) { ++g_ends; return 0; }
void* __cdecl CCompressorZlib::Alloc(void*, uInt items, uInt size) { return malloc(items * size); }
void  __cdecl CCompressorZlib::Free(void*, void* address) { free(address); }
// Row-object externals this test does not model (generated): a call reaches a trap that prints FAIL and exits; a data
// symbol reads a zeroed buffer.
// (none)
int main()
{
    CCompressorZlib c; unsigned char out[16]; memset(out, 0, sizeof(out));
    unsigned long n0 = c.InternalCompress("abc", 0, out, 16);
    bool ok = n0 == 0 && g_inits == 0;
    unsigned long n = c.InternalCompress("abc", 3, out, 16);
    ok = ok && n == 4 && memcmp(out, "ABC\xff", 4) == 0 && strcmp(g_version, "1.2.1") == 0 && g_level == 1 && g_size == (int)sizeof(z_stream)
        && g_inits == 1 && g_ends == 1 && !g_bad;
    if (ok) printf("00a3aa40_TEST PASS\n");
    else printf("FAIL n=%lu version=%s level=%d size=%d bad=%d\n", n, g_version, g_level, g_size, g_bad);
    return ok ? 0 : 1;
}
