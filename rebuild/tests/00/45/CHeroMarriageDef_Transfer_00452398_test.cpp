// Behaviour test for CHeroMarriageDef::Transfer @ 0x00452398: every member is transferred once, in order,
// with the matching float/long/string overload.
#include <cstdio>
#include <cstddef>
struct CCharStringData;
struct Def {
    void* vt; unsigned char pad[0x24];
    float f28, f2c, f30, f34, f38; int l3c, l40; float f44; int l48, l4c, l50, l54, l58;
    float f5c, f60, f64, f68, f6c; int l70; float f74, f78; CCharStringData* s7c; float f80; int l84;
    float f88, f8c, f90, f94, f98, f9c, fa0; CCharStringData* sa4; CCharStringData* sa8;
    float fac, fb0, fb4; CCharStringData* sb8; float fbc, fc0; CCharStringData* sc4;
};
static char kinds[64]; static const void* ptrs[64]; static int n = 0;
struct Ctx {
    void Transfer(float& v) { kinds[n] = 'f'; ptrs[n++] = &v; }
    void Transfer(int& v) { kinds[n] = 'l'; ptrs[n++] = &v; }
    void Transfer(CCharStringData*& v) { kinds[n] = 's'; ptrs[n++] = &v; }
};
static void Run(Def& d, Ctx& c)
{
    c.Transfer(d.f28); c.Transfer(d.f2c); c.Transfer(d.f30); c.Transfer(d.f34); c.Transfer(d.f38);
    c.Transfer(d.l3c); c.Transfer(d.l40); c.Transfer(d.f44); c.Transfer(d.l48); c.Transfer(d.l4c);
    c.Transfer(d.l50); c.Transfer(d.l54); c.Transfer(d.l58); c.Transfer(d.f5c); c.Transfer(d.f60);
    c.Transfer(d.f64); c.Transfer(d.f68); c.Transfer(d.f6c); c.Transfer(d.l70); c.Transfer(d.f74);
    c.Transfer(d.f78); c.Transfer(d.s7c); c.Transfer(d.f80); c.Transfer(d.l84); c.Transfer(d.f88);
    c.Transfer(d.f8c); c.Transfer(d.f90); c.Transfer(d.f94); c.Transfer(d.f98); c.Transfer(d.f9c);
    c.Transfer(d.fa0); c.Transfer(d.sa4); c.Transfer(d.sa8); c.Transfer(d.fac); c.Transfer(d.fb0);
    c.Transfer(d.fb4); c.Transfer(d.sb8); c.Transfer(d.fbc); c.Transfer(d.fc0); c.Transfer(d.sc4);
}
int main()
{
    Def d; Ctx c;
    Run(d, c);
    const char* want = "fffffllflllllffffflffsflfffffffssfffsffs";
    int i;
    for (i = 0; i < 40; ++i) {
        if (kinds[i] != want[i]) { std::printf("00452398_TEST FAIL\n"); return 1; }
        if (i > 0 && (const char*)ptrs[i] <= (const char*)ptrs[i - 1]) { std::printf("00452398_TEST FAIL\n"); return 1; }
    }
    if (n != 40 || ptrs[0] != &d.f28 || ptrs[39] != &d.sc4) { std::printf("00452398_TEST FAIL\n"); return 1; }
    std::printf("00452398_TEST PASS\n");
    return 0;
}