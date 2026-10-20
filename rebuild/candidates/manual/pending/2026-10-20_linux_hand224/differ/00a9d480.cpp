// CMatrix3x4::PostScale(const C3DVector&) -- scale the three rows of the rotation part by X, Y and Z (SSE path when available, x87 otherwise)
// Grade: DIFFER(ours v retail) -- asm_bake family: the SSE path is inline assembly (this copied to a frame word and reloaded into edx, movaps/shufps/mulps on an and-esp-16 frame); only the x87 fallback is source here
// Leaf: PostScale
// The retail SSE path (taken when the CPU-has-SSE byte 0x013D2880 is set) is inline assembly: `this` copied to a frame word
// and reloaded into edx, a 16-byte-aligned scale vector built on an `and esp,-16` frame, movaps/shufps/mulps on xmm0..xmm5
// with the translation column saved around it. It cannot be reproduced without __asm (asm_bake family), so this source
// carries only the x87 fallback.
struct C3DVector { float X, Y, Z; };
extern unsigned char g_bProcessorHasSSE;           // 0x013D2880
struct CMatrix3x4 {
    float m[3][4];                                 // rows of (x, y, z, translation), 16-byte aligned
    void PostScale(const C3DVector& scale);
};

void CMatrix3x4::PostScale(const C3DVector& scale)
{
    m[0][0] = scale.X * m[0][0];
    m[0][1] = m[0][1] * scale.X;
    m[0][2] = scale.X * m[0][2];
    m[1][0] = scale.Y * m[1][0];
    m[1][1] = scale.Y * m[1][1];
    m[1][2] = scale.Y * m[1][2];
    m[2][0] = scale.Z * m[2][0];
    m[2][1] = scale.Z * m[2][1];
    m[2][2] = scale.Z * m[2][2];
}
