// Test for 0x00C41840 CLibpkObject1D8::SetFlag80000: toggles bit 0x80000 and raises dirty 0x40000 only on a change.
#include <cstdio>
struct CObj { unsigned int StateFlags, DirtyFlags;
    void SetFlag80000(bool on) {
        if (on) { unsigned int f = StateFlags; if (f & 0x80000) return; StateFlags = f | 0x80000; }
        else { unsigned int f = StateFlags; if (!(f & 0x80000)) return; StateFlags = f & ~0x80000u; }
        DirtyFlags |= 0x40000; } };
int main()
{
    CObj o; o.StateFlags = 1; o.DirtyFlags = 0;
    o.SetFlag80000(false); bool ok = o.DirtyFlags == 0 && o.StateFlags == 1;
    o.SetFlag80000(true); ok = ok && o.StateFlags == 0x80001 && o.DirtyFlags == 0x40000;
    o.DirtyFlags = 0; o.SetFlag80000(false); ok = ok && o.StateFlags == 1 && o.DirtyFlags == 0x40000;
    if (!ok) { std::printf("00c41840_TEST FAIL\n"); return 1; }
    std::printf("00c41840_TEST PASS\n");
    return 0;
}