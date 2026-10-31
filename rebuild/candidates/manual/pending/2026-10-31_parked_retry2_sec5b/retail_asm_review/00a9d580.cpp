// CMatrix3x4::PostScale(const C3DVector& scale) -- scale the three columns of the 3x4 matrix by (x, y, z), keeping the translation column; SSE path when the CPU supports it
// Grade: DIFFER(221v221) 7 diff lines (asm oracle, not landable under the brief) -- the __asm block (mov edx,self ; fixed xmm0-3) gives retail's length; residue: two translation-copy stores scheduled earlier and the x87 path's commutative operand order at m[0][2] / m[2][0] (base intrinsics source matched the x87 path)
// Retail-asm: 0x00A9D5BE..0x00A9D5EC `mov [esp],ecx ; ... ; mov edx,[esp] ; movaps xmm3,[esp+10h] ; movaps xmm0,[edx] ; movaps xmm1,[edx+10h] ; movaps xmm2,[edx+20h] ; mulps x3 ; movaps [edx],xmm0 ...` -- this homed to a frame slot and reloaded into edx (an __asm naming a C local), fixed xmm0..xmm3.
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
        CMatrix3x4* self = this;
        CAlignedVector4 s;
        s.X = scale.X;
        s.Y = scale.Y;
        s.Z = scale.Z;
        float t0 = m[0][3];
        float t1 = m[1][3];
        float t2 = m[2][3];
        s.W = 0;
        __asm {
            mov     edx, self
            movaps  xmm3, s
            movaps  xmm0, [edx]
            movaps  xmm1, [edx+10h]
            movaps  xmm2, [edx+20h]
            mulps   xmm0, xmm3
            mulps   xmm1, xmm3
            mulps   xmm2, xmm3
            movaps  [edx], xmm0
            movaps  [edx+10h], xmm1
            movaps  [edx+20h], xmm2
        }
        m[0][3] = t0;
        m[1][3] = t1;
        m[2][3] = t2;
        return;
    }
    m[0][0] *= scale.X; m[0][1] *= scale.Y; m[0][2] *= scale.Z;
    m[1][0] *= scale.X; m[1][1] *= scale.Y; m[1][2] *= scale.Z;
    m[2][0] *= scale.X; m[2][1] *= scale.Y; m[2][2] *= scale.Z;
}
