// CVector4SSE_00c88930::CVector4SSE_00c88930(float x, float y, float z, float w) -- pack four floats into the 16-byte SSE member
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C88930, length 67 bytes.
// Callees: none. The manifest label "CDateAndTime::CDateAndTime" is propagated; CDateAndTime has no SSE
// members, so the class is named by address and shape.
// Shape: `_mm_set_ps(x, y, z, w)` builds the vector in a 16-byte aligned frame temporary (`and esp,-16 ;
// sub esp,10h`), one `movss` per argument from the highest element (x at +0Ch) down to w at +0, then a
// `movaps` load/store pair writes the member; `this` is kept in eax because the constructor returns it.
#include <xmmintrin.h>

struct CVector4SSE_00c88930 {
    __m128 m_v;                        // +0x00 x in element 3 .. w in element 0
    CVector4SSE_00c88930(float x, float y, float z, float w);
};

CVector4SSE_00c88930::CVector4SSE_00c88930(float x, float y, float z, float w)
{
    m_v = _mm_set_ps(x, y, z, w);
}