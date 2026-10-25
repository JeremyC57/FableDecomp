// NVectorMath::GFClear4DVectors(C4DVector* out, unsigned long count, const C4DVector& value) -- fill an array of 4D vectors with one value: an SSE store loop when the CPU has SSE, else a copy of the value handed to the STLport fill_n
// Grade: DIFFER(95v89) -- retail_asm candidate: the SSE arm is hand asm (`mov eax,[ebp+10h] ; mov edx,[ebp+8] ; movups xmm0,[eax] ; mov ecx,[ebp+0Ch]` then a `movaps [edx],xmm0 ; add edx,10h ; dec ecx ; jne` loop) inside a plain ebp frame whose C++ fallback addresses its 16-byte copy as [ebp-10h]; the intrinsic reference body adds `and esp,-16` and esp-relative locals
// Leaf: ?GFClear4DVectors@NVectorMath@@
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00A5C300, 89 bytes (+7 int3), `__cdecl`, plain `ret` (PDB
//   ?GFClear4DVectors@NVectorMath@@YAXPAVC4DVector@@KABV2@@Z).
// Callees: fill_n<C4DVector*, unsigned long, C4DVector> @ 0x00A5C510 (__fastcall: first in ecx, n in edx, the value by
//   reference; catalogued as the propagated `vector<unsigned __int64>::_Ufill`). Global: the SSE flag @ 0x013D2880 set by
//   the landed GFInitVectorMath 0x00A5B850 (cpuid).
// Shape: `if (g_HasSSE) { unaligned load, aligned store loop } else { C4DVector v = value; fill_n(out, count, v); }` --
//   the 16-byte copy is the frame local, the fallback call passes its address.
#include <xmmintrin.h>

class C4DVector {
public:
    float X, Y, Z, W;
};

extern unsigned char g_HasSSE;                          // 0x013D2880

namespace std {
template <class OutputIter, class Size, class T>
OutputIter __fastcall fill_n(OutputIter first, Size n, const T& value);   // 0x00A5C510
}

namespace NVectorMath {

void GFClear4DVectors(C4DVector* out, unsigned long count, const C4DVector& value)
{
    if (g_HasSSE) {
        __m128 v = _mm_loadu_ps(&value.X);
        for (; count != 0; --count, ++out)
            _mm_store_ps(&out->X, v);
    }
    else {
        C4DVector v = value;
        std::fill_n(out, count, v);
    }
}

}   // namespace NVectorMath
