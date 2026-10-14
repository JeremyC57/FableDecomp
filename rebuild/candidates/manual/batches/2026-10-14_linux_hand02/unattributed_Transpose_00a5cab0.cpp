// CMatrix4x4Aligned::Transpose() -- in-place transpose through a 16-byte-aligned stack copy (and esp,-16 frame), row loop with the four column reads unrolled
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00A5CAB0, length 80 bytes.
// Callees: (none)
// Propagated manifest label rejected: the body copies 16 floats of `this` to an aligned temporary and writes
// m[i][j] = tmp[j][i]; the __declspec(align(16)) matrix is what forces the aligned frame. Written as the
// nested i/j loops: VC7.1 fully unrolls the inner constant loop and keeps the outer one (a single loop
// with the four stores written out is unrolled completely, 143 bytes).
//
#include "rebuild_abi.h"
struct __declspec(align(16)) CMatrix4x4Aligned {
    float m[4][4];
    void Transpose();
};
void CMatrix4x4Aligned::Transpose()
{
    CMatrix4x4Aligned tmp = *this;
    for (int i = 0; i < 4; ++i)
        for (int j = 0; j < 4; ++j)
            m[i][j] = tmp.m[j][i];
}
