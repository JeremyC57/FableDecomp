#include <cstdio>
struct CSprite_00b1b5e0 { unsigned char _pad00[0x48]; float m_height; long GetHeight() const; };
__forceinline void FloatToLong(long* result, float value) { __asm fld value __asm mov edx, result __asm fistp dword ptr [edx] }
long CSprite_00b1b5e0::GetHeight() const { long height; FloatToLong(&height, m_height); return height; }
int main() {
    CSprite_00b1b5e0 s; s.m_height = 12.4f; bool ok = s.GetHeight() == 12;
    s.m_height = 12.6f; ok = ok && s.GetHeight() == 13;        // round-to-nearest (the FPU default), not truncation
    s.m_height = -3.5f; ok = ok && s.GetHeight() == -4;        // ties to even
    std::printf(ok ? "00b1b5e0_TEST PASS\n" : "00b1b5e0_TEST FAIL\n"); return ok ? 0 : 1;
}