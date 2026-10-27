// Test for 0x00C415E0 CLibpkObject1D8::SetFloat154: a change stores and sets dirty bit 0x800000.
#include <cstdio>
struct CObj { float Float154; unsigned int DirtyFlags;
    void SetFloat154(float v) { if (Float154 != v) { Float154 = v; DirtyFlags |= 0x800000; } } };
int main()
{
    CObj o; o.Float154 = 1.0f; o.DirtyFlags = 0;
    o.SetFloat154(1.0f); bool ok = o.DirtyFlags == 0;
    o.SetFloat154(0.25f); ok = ok && o.Float154 == 0.25f && o.DirtyFlags == 0x800000;
    if (!ok) { std::printf("00c415e0_TEST FAIL\n"); return 1; }
    std::printf("00c415e0_TEST PASS\n");
    return 0;
}