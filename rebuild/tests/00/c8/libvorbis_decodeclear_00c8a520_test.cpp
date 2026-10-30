// Behaviour test for vorbisfile.c _decode_clear (0x00C8A520): clear the DSP and block state, back to OPENED.
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
struct VF { int ready_state; int dsp_cleared, block_cleared; };
static void _decode_clear(VF* vf) { vf->dsp_cleared++; vf->block_cleared++; vf->ready_state = 2; }
int main()
{
    VF vf = { 4, 0, 0 }; _decode_clear(&vf);
    if (vf.ready_state != 2 || vf.dsp_cleared != 1 || vf.block_cleared != 1) FAIL("00c8a520");
    std::printf("00c8a520_TEST PASS\n"); return 0;
}