// Behaviour test for vorbisfile.c ov_halfrate (0x00C8A8B0): OV_EINVAL without info or seeking; clears the decoder; on a refused link switches back and fails.
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
struct VF { int has_vi; int seekable; int ready_state; int links; int refuse_at; int cleared; int flags[4]; };
static int synthesis_halfrate(VF* vf, int i, int flag) { if (flag && i == vf->refuse_at) return -1; vf->flags[i] = flag; return 0; }
static int ov_halfrate(VF* vf, int flag)
{
    if (!vf->has_vi) return -131;
    if (!vf->seekable) return -131;
    if (vf->ready_state >= 3) { vf->cleared++; vf->ready_state = 2; }
    for (int i = 0; i < vf->links; i++)
        if (synthesis_halfrate(vf, i, flag)) { ov_halfrate(vf, 0); return -131; }
    return 0;
}
int main()
{
    VF a = { 0, 1, 4, 2, -1, 0, { 0 } }; if (ov_halfrate(&a, 1) != -131) FAIL("00c8a8b0");
    VF b = { 1, 1, 4, 2, -1, 0, { 0 } }; if (ov_halfrate(&b, 1) != 0 || b.cleared != 1 || b.flags[1] != 1) FAIL("00c8a8b0");
    VF c = { 1, 1, 2, 3, 1, 0, { 0 } }; if (ov_halfrate(&c, 1) != -131 || c.flags[0] != 0 || c.cleared != 0) FAIL("00c8a8b0");
    std::printf("00c8a8b0_TEST PASS\n"); return 0;
}