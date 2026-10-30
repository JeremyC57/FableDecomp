// Behaviour test for vorbisfile.c _prefetch_all_headers (0x00C8A1D0): per link, the beginning PCM offset (granulepos minus the accumulated block overlap, floored at 0) and the PCM length (last granulepos minus the start).
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
static i64 begin_pcm(const long* blocks, int n, i64 granulepos)
{
    i64 accumulated = 0; long lastblock = -1;
    for (int k = 0; k < n; k++) {
        long thisblock = blocks[k];
        if (lastblock != -1) accumulated += (lastblock + thisblock) >> 2;
        lastblock = thisblock;
    }
    accumulated = granulepos - accumulated;
    if (accumulated < 0) accumulated = 0;
    return accumulated;
}
int main()
{
    long b[4] = { 256, 2048, 2048, 256 };
    /* overlaps: (256+2048)/4 = 576, (2048+2048)/4 = 1024, (2048+256)/4 = 576 -> 2176 */
    if (begin_pcm(b, 4, 5000) != 2824) FAIL("00c8a1d0");
    if (begin_pcm(b, 4, 1000) != 0) FAIL("00c8a1d0");
    i64 pcmlengths[2] = { 2824, 0 };
    pcmlengths[1] = 100000 - pcmlengths[0];
    if (pcmlengths[1] != 97176) FAIL("00c8a1d0");
    std::printf("00c8a1d0_TEST PASS\n"); return 0;
}