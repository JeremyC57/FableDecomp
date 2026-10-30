// Behaviour test for vorbisfile.c _fetch_and_process_packet (0x00C8A540): the PCM offset of a packet (granulepos less the earlier links' start, floored, less the buffered samples, plus the earlier links' lengths).
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
static i64 pcm_offset(i64 granulepos, int seekable, int link, const i64* pcmlengths, int samples)
{
    if (seekable && link > 0) granulepos -= pcmlengths[link * 2];
    if (granulepos < 0) granulepos = 0;
    granulepos -= samples;
    for (int i = 0; i < link; i++) granulepos += pcmlengths[i * 2 + 1];
    return granulepos;
}
int main()
{
    i64 pl[4] = { 0, 44100, 500, 22050 };
    if (pcm_offset(10000, 1, 1, pl, 1000) != 10000 - 500 - 1000 + 44100) FAIL("00c8a540");
    if (pcm_offset(100, 1, 1, pl, 0) != 44100) FAIL("00c8a540");      /* floored at 0 before the sums */
    if (pcm_offset(5000, 0, 0, pl, 200) != 4800) FAIL("00c8a540");
    std::printf("00c8a540_TEST PASS\n"); return 0;
}