#include <cstdio>
class C3DVector { public: float X, Y, Z; };
struct CPackedNormalOwner_00be8b50 { unsigned long m_packedNormal; void PackVertexNormal(const C3DVector& normal); unsigned long Pack(const C3DVector& n) { PackVertexNormal(n); return m_packedNormal; } };
__forceinline long ROUND(float value)
{
    long result;
    __asm fld value
    __asm fistp result
    return result;
}
void CPackedNormalOwner_00be8b50::PackVertexNormal(const C3DVector& normal)
{
    long z = ROUND(normal.Z * 511.0f); long y = ROUND(normal.Y * 1023.0f); long x = ROUND(normal.X * 1023.0f);
    m_packedNormal = (((y & 0x7ff) | (z << 11)) << 11) | (x & 0x7ff);
}
int main() {
    CPackedNormalOwner_00be8b50 p; C3DVector n = { 1.0f, 0.0f, 0.0f };
    bool ok = p.Pack(n) == 0x3ffu;                       // x*1023 in the low 11 bits
    C3DVector n2 = { 0.0f, 0.5f, 1.0f };                            // y = 512 (0.5*1023 = 511.5 rounds to even 512), z = 511
    ok = ok && p.Pack(n2) == ((((512u & 0x7ff) | (511u << 11)) << 11) & 0xffffffffu);
    C3DVector n3 = { -1.0f, 0.0f, 0.0f };                           // -1023 & 0x7ff
    ok = ok && p.Pack(n3) == ((unsigned long)(-1023) & 0x7ff);
    std::printf(ok ? "00be8b50_TEST PASS\n" : "00be8b50_TEST FAIL\n"); return ok ? 0 : 1;
}