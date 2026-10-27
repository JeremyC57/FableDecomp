// Test for 0x00C417F0 CLibpkObject1D8::SetFlag40000: toggles bit 0x40000; dirty mask depends on word +0xC bit 0x2000.
#include <cstdio>
struct CObj { unsigned int Word0C, StateFlags, DirtyFlags;
    void SetFlag40000(bool on) {
        if (on) { unsigned int f = StateFlags; if (f & 0x40000) return; StateFlags = f | 0x40000; }
        else { unsigned int f = StateFlags; if (!(f & 0x40000)) return; StateFlags = f & ~0x40000u; }
        unsigned int mask = 0xBF8000; if (!(Word0C & 0x2000)) mask = 0x1FF8000; DirtyFlags |= mask; } };
int main()
{
    CObj o; o.Word0C = 0; o.StateFlags = 0; o.DirtyFlags = 0;
    o.SetFlag40000(false); bool ok = o.DirtyFlags == 0;
    o.SetFlag40000(true); ok = ok && o.StateFlags == 0x40000 && o.DirtyFlags == 0x1FF8000;
    o.DirtyFlags = 0; o.SetFlag40000(true); ok = ok && o.DirtyFlags == 0;
    o.Word0C = 0x2000; o.SetFlag40000(false); ok = ok && o.StateFlags == 0 && o.DirtyFlags == 0xBF8000;
    if (!ok) { std::printf("00c417f0_TEST FAIL\n"); return 1; }
    std::printf("00c417f0_TEST PASS\n");
    return 0;
}