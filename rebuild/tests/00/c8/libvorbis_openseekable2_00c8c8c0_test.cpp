// Behaviour test for vorbisfile.c _open_seekable2 (0x00C8C8C0): a chained file bisects from 0, a single link from the last page; a negative last-page offset is returned as is.
#include <cstdio>
#include <cstring>
#ifdef _MSC_VER
typedef __int64 i64;
#else
#ifdef _MSC_VER
typedef __int64 i64;
#else
#include <stdint.h>
typedef int64_t i64;
#endif
#endif
#define FAIL(a) do { std::printf(a "_TEST FAIL\n"); return 1; } while (0)
struct VF { i64 last; int last_serial; long serial; i64 bis_searched; int prefetched; };
static int _open_seekable2(VF* vf)
{
    i64 end = vf->last;
    if (end < 0) return (int)end;
    if (vf->last_serial != vf->serial) vf->bis_searched = 0; else vf->bis_searched = end;
    vf->prefetched = 1;
    return 0;
}
int main()
{
    VF a = { 5000, 7, 7, -1, 0 }; if (_open_seekable2(&a) != 0 || a.bis_searched != 5000 || !a.prefetched) FAIL("00c8c8c0");
    VF b = { 5000, 9, 7, -1, 0 }; if (_open_seekable2(&b) != 0 || b.bis_searched != 0) FAIL("00c8c8c0");
    VF c = { -128, 7, 7, -1, 0 }; if (_open_seekable2(&c) != -128 || c.prefetched) FAIL("00c8c8c0");
    std::printf("00c8c8c0_TEST PASS\n"); return 0;
}