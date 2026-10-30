// Behaviour test for vorbisfile.c _ov_initset (0x00C8C1D0): pull packets until INITSET, skipping holes; a hard error is returned.
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
struct VF { int ready_state; int results[6]; int n; int pcm; };
static int fetch(VF* vf) { int r = vf->results[vf->n++]; if (r == 1) vf->ready_state = 4; if (r == 2) { vf->pcm = 1; r = 1; } return r; }

static int _ov_initset(VF* vf)
{
    while (1) {
        if (vf->ready_state == 4) break;
        int ret = fetch(vf);
        if (ret < 0 && ret != -3) return ret;
    }
    return 0;
}
int main()
{
    VF a = { 2, { -3, 0, 1 }, 0, 0 }; if (_ov_initset(&a) != 0 || a.n != 3) FAIL("00c8c1d0");
    VF b = { 2, { -3, -2 }, 0, 0 }; if (_ov_initset(&b) != -2) FAIL("00c8c1d0");
    VF c = { 4, { -2 }, 0, 0 }; if (_ov_initset(&c) != 0 || c.n != 0) FAIL("00c8c1d0");
    std::printf("00c8c1d0_TEST PASS\n"); return 0;
}