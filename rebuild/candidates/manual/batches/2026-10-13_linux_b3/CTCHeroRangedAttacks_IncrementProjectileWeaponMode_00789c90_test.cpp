#include <cstdio>

static int g_reset = 0;
struct Def { unsigned char pad[0x128]; long* First; long* Last; };
struct R { unsigned char pad[0x10]; Def* def; unsigned char p2[0x30]; long Index;
    void Reset() { ++g_reset; }
    void Inc() { Def* d = def; ++Index; if (Index == d->Last - d->First) Reset(); } };
int main()
{
    long modes[3]; Def d; d.First = modes; d.Last = modes + 3; R r; r.def = &d; r.Index = 1;
    r.Inc(); if (r.Index != 2 || g_reset != 0) { std::printf("00789c90_TEST FAIL\n"); return 1; }
    r.Inc(); if (r.Index != 3 || g_reset != 1) { std::printf("00789c90_TEST FAIL\n"); return 1; }
    std::printf("00789c90_TEST PASS\n");
    return 0;
}
