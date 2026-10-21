#include <cstdio>
#include <cstring>
// Behaviour copy of CAIStateGroup_KidHurtSelfRunToMother::PerformRandomLengthCrying (0x00914F00).
struct Sim { long results[3]; int idx; const char* played[3]; long loops; unsigned long seed; };
static unsigned long Ror13(unsigned long v) { return (v >> 13) | (v << 19); }
static long Play(Sim& s, const char* name, long loops) { s.played[s.idx] = name; if (loops >= 0) s.loops = loops; return s.results[s.idx++]; }
static long Cry(Sim& s) {
    long r = Play(s, "INTO", -1);
    if (r != 2) return r;
    s.seed = s.seed * 0x24A1U + 0x24DFU; s.seed = Ror13(s.seed); long loops = (long)(s.seed % 3);
    r = Play(s, "LOOP", loops);
    if (r != 2) return r;
    return Play(s, "OUTOF", -1);
}
int main() {
    Sim a = { { 2, 2, 7 }, 0, { 0, 0, 0 }, -1, 5 };
    if (Cry(a) != 7 || a.idx != 3 || std::strcmp(a.played[2], "OUTOF") != 0 || a.loops < 0 || a.loops > 2) { std::printf("00914f00_TEST FAIL\n"); return 1; }
    Sim b = { { 3, 2, 2 }, 0, { 0, 0, 0 }, -1, 5 };
    if (Cry(b) != 3 || b.idx != 1) { std::printf("00914f00_TEST FAIL\n"); return 1; }
    Sim c = { { 2, 4, 2 }, 0, { 0, 0, 0 }, -1, 5 };
    if (Cry(c) != 4 || c.idx != 2) { std::printf("00914f00_TEST FAIL\n"); return 1; }
    std::printf("00914f00_TEST PASS\n"); return 0;
}
