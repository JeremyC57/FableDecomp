// Test for 0x00C3D390 CLibpkStateA4::SetFloat94: a change stores the value, value/330 and dirty bit 0x10000.
#include <cstdio>
struct CState { float Float94, Float98; unsigned int DirtyFlags;
    void SetFloat94(float v) { if (Float94 != v) { Float94 = v; Float98 = v * (1.0f / 330.0f); DirtyFlags |= 0x10000; } } };
int main()
{
    CState s; s.Float94 = 1.0f; s.Float98 = -1.0f; s.DirtyFlags = 1;
    s.SetFloat94(1.0f);
    bool ok = s.Float98 == -1.0f && s.DirtyFlags == 1;
    s.SetFloat94(33.0f);
    float d = s.Float98 - 0.1f; if (d < 0) d = -d;
    ok = ok && s.Float94 == 33.0f && d < 1e-5f && s.DirtyFlags == 0x10001;
    if (!ok) { std::printf("00c3d390_TEST FAIL\n"); return 1; }
    std::printf("00c3d390_TEST PASS\n");
    return 0;
}