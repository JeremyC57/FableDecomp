// Behaviour test for vorbisfile.c _make_decode_ready (0x00C8A4B0): 0 above STREAMSET, OV_EFAULT below, OV_EBADLINK when synthesis init fails, else INITSET with tracking zeroed.
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
struct VF { int ready_state; int seekable; int current_link; int initfail; int initlink; double bittrack, samptrack; };
static int _make_decode_ready(VF* vf)
{
    if (vf->ready_state > 3) return 0;
    if (vf->ready_state < 3) return -129;
    vf->initlink = vf->seekable ? vf->current_link : 0;
    if (vf->initfail) return -137;
    vf->ready_state = 4; vf->bittrack = 0.f; vf->samptrack = 0.f;
    return 0;
}
int main()
{
    VF a = { 4, 0, 0, 0, -1, 1, 1 }; if (_make_decode_ready(&a) != 0 || a.bittrack != 1) FAIL("00c8a4b0");
    VF b = { 2, 0, 0, 0, -1, 1, 1 }; if (_make_decode_ready(&b) != -129) FAIL("00c8a4b0");
    VF c = { 3, 1, 2, 1, -1, 1, 1 }; if (_make_decode_ready(&c) != -137 || c.initlink != 2) FAIL("00c8a4b0");
    VF d = { 3, 1, 2, 0, -1, 5, 5 };
    if (_make_decode_ready(&d) != 0 || d.ready_state != 4 || d.bittrack != 0 || d.samptrack != 0 || d.initlink != 2) FAIL("00c8a4b0");
    std::printf("00c8a4b0_TEST PASS\n"); return 0;
}