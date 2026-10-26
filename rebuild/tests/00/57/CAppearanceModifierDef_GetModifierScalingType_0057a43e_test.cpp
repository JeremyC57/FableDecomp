#include <cstdio>
class CAppearanceModifierDef { public:
    unsigned char _pad0[0x40]; long CoversBodyAreaFlags; unsigned char _pad1[0xc]; long SuitPart;
    long GetModifierScalingType() const { if (SuitPart == 0) { switch (CoversBodyAreaFlags) { case 0x10: return 1; case 0x80: return 2; case 0x100: return 3; } } return 0; }
};
int main()
{
    CAppearanceModifierDef d; d.SuitPart = 0;
    d.CoversBodyAreaFlags = 0x10;  if (d.GetModifierScalingType() != 1) { std::printf("0057a43e_TEST FAIL\n"); return 1; }
    d.CoversBodyAreaFlags = 0x80;  if (d.GetModifierScalingType() != 2) { std::printf("0057a43e_TEST FAIL\n"); return 1; }
    d.CoversBodyAreaFlags = 0x100; if (d.GetModifierScalingType() != 3) { std::printf("0057a43e_TEST FAIL\n"); return 1; }
    d.CoversBodyAreaFlags = 0x20;  if (d.GetModifierScalingType() != 0) { std::printf("0057a43e_TEST FAIL\n"); return 1; }
    d.SuitPart = 2; d.CoversBodyAreaFlags = 0x10; if (d.GetModifierScalingType() != 0) { std::printf("0057a43e_TEST FAIL\n"); return 1; }
    std::printf("0057a43e_TEST PASS\n");
    return 0;
}