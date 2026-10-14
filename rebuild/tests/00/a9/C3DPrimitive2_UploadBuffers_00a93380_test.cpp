#include <cstdio>
static int g_path = 0;
struct P {
    virtual void v00() {}
    long mat; long reps; unsigned char pad[0x3c]; unsigned long anim; unsigned char pad2[0x1c]; unsigned long flags;
    bool Upload() { if (anim > 0) return A(flags); if (reps != 0) return R(flags, reps); return S(flags); }
    bool A(unsigned long) { g_path = 1; return true; }
    bool R(unsigned long, unsigned long) { g_path = 2; return false; }
    bool S(unsigned long) { g_path = 3; return true; }
};
int main()
{
    P p; p.anim = 1; p.reps = 0; p.flags = 0;
    if (!p.Upload() || g_path != 1) { std::printf("00a93380_TEST FAIL\n"); return 1; }
    p.anim = 0; p.reps = 2; if (p.Upload() || g_path != 2) { std::printf("00a93380_TEST FAIL\n"); return 1; }
    p.reps = 0; if (!p.Upload() || g_path != 3) { std::printf("00a93380_TEST FAIL\n"); return 1; }
    std::printf("00a93380_TEST PASS\n");
    return 0;
}