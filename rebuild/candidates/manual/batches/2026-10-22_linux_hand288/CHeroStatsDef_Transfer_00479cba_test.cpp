// behaviour test for CHeroStatsDef::Transfer (0x00479CBA): 43 typed transfers in member order, Vandalism before Picklock
#include <cstdio>
struct CAppVec { void* a[3]; };
struct CTrackVec { void* a[3]; };
struct CLongVector { long* a[3]; };
struct CFloatVector { float* a[3]; };
static const void* g_log[64]; static int g_n = 0;
struct CPersistContext {
    void Transfer(long& v) { g_log[g_n++] = &v; }
    void Transfer(float& v) { g_log[g_n++] = &v; }
    void Transfer(CAppVec& v) { g_log[g_n++] = &v; }
    void Transfer(CLongVector& v) { g_log[g_n++] = &v; }
    void Transfer(CFloatVector& v) { g_log[g_n++] = &v; }
    void Transfer(CTrackVec& v) { g_log[g_n++] = &v; }
};
struct CHeroStatsDef {
    long a0; float f1, f2, f3; long a4, a5, a6, a7, a8, a9, a10; float f11, f12; CAppVec v13; float f14, f15, f16, f17;
    long a18, a19, a20, a21; float f22, f23, f24, f25; CLongVector v26, v27, v28; long a29, a30; float f31; long a32, a33, a34;
    long Picklock, Vandalism; CFloatVector v37; CTrackVec v38; CFloatVector v39; CLongVector v40; CFloatVector v41, v42;
    void Transfer(CPersistContext& c)
    {
        c.Transfer(a0); c.Transfer(f1); c.Transfer(f2); c.Transfer(f3); c.Transfer(a4); c.Transfer(a5); c.Transfer(a6); c.Transfer(a7);
        c.Transfer(a8); c.Transfer(a9); c.Transfer(a10); c.Transfer(f11); c.Transfer(f12); c.Transfer(v13); c.Transfer(f14); c.Transfer(f15);
        c.Transfer(f16); c.Transfer(f17); c.Transfer(a18); c.Transfer(a19); c.Transfer(a20); c.Transfer(a21); c.Transfer(f22); c.Transfer(f23);
        c.Transfer(f24); c.Transfer(f25); c.Transfer(v26); c.Transfer(v27); c.Transfer(v28); c.Transfer(a29); c.Transfer(a30); c.Transfer(f31);
        c.Transfer(a32); c.Transfer(a33); c.Transfer(a34); c.Transfer(Vandalism); c.Transfer(Picklock); c.Transfer(v37); c.Transfer(v38);
        c.Transfer(v39); c.Transfer(v40); c.Transfer(v41); c.Transfer(v42);
    }
};
int main()
{
    static CHeroStatsDef d; CPersistContext c; d.Transfer(c);
    if (g_n != 43 || g_log[35] != &d.Vandalism || g_log[36] != &d.Picklock || g_log[42] != &d.v42) { std::printf("00479cba_TEST FAIL\n"); return 1; }
    for (int i = 1; i < 35; ++i)
        if ((const char*)g_log[i] <= (const char*)g_log[i - 1]) { std::printf("00479cba_TEST FAIL\n"); return 1; }
    std::printf("00479cba_TEST PASS\n");
    return 0;
}
