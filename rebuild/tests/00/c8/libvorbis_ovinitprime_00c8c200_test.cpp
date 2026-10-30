// Behaviour test for vorbisfile.c _ov_initprime (0x00C8C200): pull packets until INITSET with PCM buffered, skipping holes; a hard error is returned.
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

static int _ov_initprime(VF* vf)
{
    while (1) {
        if (vf->ready_state == 4) if (vf->pcm) break;
        int ret = fetch(vf);
        if (ret < 0 && ret != -3) return ret;
    }
    return 0;
}
int main()
{
    VF a = { 2, { 1, -3, 2 }, 0, 0 }; if (_ov_initprime(&a) != 0 || a.n != 3) FAIL("00c8c200");
    VF b = { 4, { -137 }, 0, 0 }; if (_ov_initprime(&b) != -137) FAIL("00c8c200");
    std::printf("00c8c200_TEST PASS\n"); return 0;
}