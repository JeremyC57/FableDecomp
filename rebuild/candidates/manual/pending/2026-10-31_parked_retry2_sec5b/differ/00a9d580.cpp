// CMatrix3x4::PostScale(const C3DVector& scale) -- scale the three columns of the 3x4 matrix by (x, y, z), keeping the translation column; SSE path when the CPU supports it
// Grade: DIFFER(199v221) 38 diff lines (unchanged; sec5b intrinsic re-order probe 40) -- retail homes this at [esp] and reloads it into edx for fixed-register movaps xmm0..xmm3 (inline asm naming C locals); intrinsics never reload. The asm oracle reaches 221v221 / 7 lines: retail_asm_review/00a9d580.cpp.
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00A9D580, length 221 bytes (`ret 4`, 16-byte aligned frame).
// Callees: none; the SSE-available flag byte @ 0x013D2880.
// Layout: this TU's CMatrix3x4 is three 16-byte rows (x, y, z, translation) so the SSE path can `movaps` each row.
// Shape: the SSE path builds an aligned (x, y, z, 0) scale, saves the translation column, multiplies the rows, restores the column;
// the x87 path multiplies the nine elements in place.
#include <xmmintrin.h>
struct C3DVector { float X, Y, Z; };
extern bool g_SSEAvailable;                     // 0x013D2880
struct __declspec(align(16)) CAlignedVector4 { float X, Y, Z, W; };
struct CMatrix3x4 {
    float m[3][4];
    void PostScale(const C3DVector& scale);
};

void CMatrix3x4::PostScale(const C3DVector& scale)
{
    if (g_SSEAvailable) {
        CAlignedVector4 s;
        s.X = scale.X; s.Y = scale.Y; s.Z = scale.Z;
        float t0 = m[0][3], t1 = m[1][3], t2 = m[2][3];
        s.W = 0;
        __m128 v = _mm_load_ps(&s.X);
        _mm_store_ps(m[0], _mm_mul_ps(_mm_load_ps(m[0]), v));
        _mm_store_ps(m[1], _mm_mul_ps(_mm_load_ps(m[1]), v));
        _mm_store_ps(m[2], _mm_mul_ps(_mm_load_ps(m[2]), v));
        m[0][3] = t0; m[1][3] = t1; m[2][3] = t2;
        return;
    }
    m[0][0] *= scale.X; m[0][1] *= scale.Y; m[0][2] *= scale.Z;
    m[1][0] *= scale.X; m[1][1] *= scale.Y; m[1][2] *= scale.Z;
    m[2][0] *= scale.X; m[2][1] *= scale.Y; m[2][2] *= scale.Z;
}
