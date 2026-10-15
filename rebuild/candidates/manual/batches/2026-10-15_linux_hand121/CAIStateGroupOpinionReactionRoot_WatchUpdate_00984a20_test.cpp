#include <cstdio>
// OpinionReactionRoot::WatchUpdate: look at the thing for 30 + seed%45 frames; 1 when done (4) or timed out, else 0
static unsigned long g_seed = 3; static long g_frame = 100; static long g_results[8]; static int g_calls = 0;
static unsigned long ROR13(unsigned long v) { return (v >> 13) | (v << 19); }
static unsigned long RandomFrames(const unsigned long& range) { unsigned long* s = &g_seed; unsigned long n = *s * 0x24A1U + 0x24DFU; *s = n; unsigned long r = ROR13(n); *s = r; return r % range; }
static long Look() { ++g_frame; return g_results[(g_calls++) & 7]; }
struct Group {
    long WatchUpdate()
    {
        long endFrame = g_frame + RandomFrames(45) + 30;
        do { long result = Look(); if (result != 2) { if (result != 4) return 0; break; } } while (g_frame < endFrame);
        return 1;
    }
};
int main()
{
    Group g; unsigned long expect = ROR13(3u * 0x24A1U + 0x24DFU) % 45;
    g_results[0] = 2; g_results[1] = 4; if (g.WatchUpdate() != 1 || g_calls != 2) { std::printf("00984a20_TEST FAIL\n"); return 1; }
    g_calls = 0; g_results[0] = 2; g_results[1] = 3; if (g.WatchUpdate() != 0 || g_calls != 2) { std::printf("00984a20_TEST FAIL\n"); return 1; }
    g_calls = 0; g_frame = 100; g_seed = 3; for (int i = 0; i < 8; ++i) g_results[i] = 2;
    long before = g_frame; long r = g.WatchUpdate();
    if (r != 1 || g_frame - before != (long)(expect + 30) || g_calls != (int)(expect + 30)) { std::printf("00984a20_TEST FAIL\n"); return 1; }
    std::printf("00984a20_TEST PASS\n"); return 0;
}
