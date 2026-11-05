// NVectorMath::GFClear4DVectors(C4DVector* dst, unsigned long count, const C4DVector* value) -- fill `count` aligned 4D vectors with one value: an SSE `movups`/`movaps` store loop when the CPU has SSE, else a copy of the value handed to the scalar fill 0x00A5C510
// Leaf: ?GFClear4DVectors@NVectorMath@@
// Retail-asm: 0x00A5C30F..0x00A5C326 `mov eax,[ebp+10h] ; mov edx,[ebp+8] ; movups xmm0,[eax] ; mov ecx,[ebp+0Ch] ; test ecx,ecx ; je ; movaps [edx],xmm0 ; add edx,10h ; dec ecx ; jne` -- a hand SSE fill loop (10 instructions, a je and the loop jne: rasm1 family (f)) inside the plain ebp frame: fixed registers and an unaligned load of the value next to aligned stores, which the xmmintrin spelling does not produce (rasm1 probe: intrinsics build an `and esp,-16` frame). The SSE flag test, the scalar fallback (the value copied dword by dword into a frame local and passed by address) and the frame are C++.
// Helper (2026-11-05 asm migration): the retail inline-asm block is the shared engine/NVectorMathSSE.h (byte-matching path + FABLE_PORTABLE twin); the row has
//   no asm of its own. C++/intrinsic attempts: the batch ATTEMPTS.tsv, family SSE_NVECTORMATH.
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00A5C300, 89 bytes (+7 int3), __cdecl, plain `ret`. Globals: the SSE-available byte
//   0x013D2880. Callee: the scalar fill 0x00A5C510 (__fastcall: dst in ecx, count in edx, the value pointer pushed).
#include "engine/NVectorMathSSE.h"
class C4DVector {
public:
    float X, Y, Z, W;
    C4DVector() { }
    C4DVector(const C4DVector& other) : X(other.X), Y(other.Y), Z(other.Z), W(other.W) { }
};
extern bool g_SSEAvailable;                                          // 0x013D2880
namespace NVectorMath {
void __fastcall GFClear4DVectorsScalar(C4DVector* dst, unsigned long count, const C4DVector* value);   // 0x00A5C510
#pragma optimize("y", off)
void GFClear4DVectors(C4DVector* dst, unsigned long count, const C4DVector* value)
{
    if (g_SSEAvailable) {
        GF_SSE_CLEAR_4D_VECTORS(value, dst, count);
    } else
    {
        C4DVector copy = *value;
        GFClear4DVectorsScalar(dst, count, &copy);
    }
}
}   // namespace NVectorMath