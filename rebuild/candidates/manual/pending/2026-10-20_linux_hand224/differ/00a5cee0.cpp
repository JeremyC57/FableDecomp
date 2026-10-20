// CMatrix4x4::AlignedSetInterpolate(const CMatrix4x4&, const CMatrix4x4&, float) -- this = a + (b - a) * t (SSE path when available, x87 otherwise)
// Grade: DIFFER(ours v retail) -- asm_bake family: the SSE path is inline assembly (1-t and t spilled to aligned frame vectors, shufps broadcast, mulps/addps over xmm0..xmm7); only the x87 a+(b-a)*t fallback is source here
// Leaf: AlignedSetInterpolate
// The retail SSE path (CPU-has-SSE byte 0x013D2880) is inline assembly on an `and esp,-16` frame: `1 - t` and `t` spilled to
// aligned frame vectors, shufps-broadcast, then mulps/addps over xmm0..xmm7 with movaps loads/stores of the three matrices
// (asm_bake family). This source carries only the x87 fallback loop.
extern unsigned char g_bProcessorHasSSE;           // 0x013D2880
struct CMatrix4x4 {
    float m[4][4];
    void AlignedSetInterpolate(const CMatrix4x4& a, const CMatrix4x4& b, float t);
};

void CMatrix4x4::AlignedSetInterpolate(const CMatrix4x4& a, const CMatrix4x4& b, float t)
{
    for (long row = 0; row < 4; ++row) {
        for (long column = 0; column < 4; ++column) {
            float from = a.m[row][column];
            m[row][column] = from + (b.m[row][column] - from) * t;
        }
    }
}
