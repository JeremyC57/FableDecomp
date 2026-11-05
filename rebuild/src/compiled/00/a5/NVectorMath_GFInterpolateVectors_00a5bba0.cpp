// NVectorMath::GFInterpolateVectors(C4DVector* out, unsigned long outStride, const C4DVector* from, unsigned long fromStride, const C4DVector* to, unsigned long toStride, unsigned long count, float weight) -- strided linear interpolation of count 4-vectors by one weight: SSE hand-asm kernel when the CPU has SSE, else the x87 C4DVector::Interpolate loop
// Leaf: ?GFInterpolateVectors@NVectorMath@@YAXPAVC4DVector@@KPBV2@K1KKM@Z
// Flags: /Zi
// Retail-asm: 0x00A5BBD6..0x00A5BC12 `mov eax,[ebx+8] ; mov esi,[ebx+10h] ; mov edx,[ebx+18h] ; mov ecx,[ebx+20h] ; movaps xmm0,[ebp-30h] ; movaps xmm1,[ebp-40h] ; shufps xmm0,xmm0,0 ; shufps xmm1,xmm1,0 ; test ecx,ecx ; je ; movaps xmm2,[esi] ; movaps xmm3,[edx] ; mulps xmm2,xmm0 ; mulps xmm3,xmm1 ; addps xmm2,xmm3 ; movaps [eax],xmm2 ; add eax,[ebx+0Ch] ; add esi,[ebx+14h] ; add edx,[ebx+1Ch] ; dec ecx ; jne`:
// Helper (2026-11-05 asm migration): the retail inline-asm block is the shared engine/NVectorMathSSE.h (byte-matching path + FABLE_PORTABLE twin); the row has
//   no asm of its own. C++/intrinsic attempts: the batch ATTEMPTS.tsv, family SSE_NVECTORMATH.
//   the hand SSE kernel of the NVectorMath family, exactly as the landed retail_asm sibling GFInterpolateVectors
//   (K1KPBMKK) 0x00A5BEF0 and GFFinishPartialSlerp 0x00A5C2B0 / GFClear4DVectors 0x00A5C300: the three pointers and the
//   count loaded into fixed registers straight from the argument slots, the strides added from memory every pass, the
//   count counted down in ecx behind `test ecx,ecx ; je`. Intrinsics give the `and esp,-16` frame and a re-scheduled
//   loop instead (the 0x00A5BEF0 probe log), so the block is the original's inline asm. The C++ around it is
//   compiler-emitted: the SSE flag test, the two 16-aligned weight locals (weight copied as a dword into [ebp-40h],
//   1 - weight through the x87 into [ebp-30h]) and the whole x87 fallback.
// Retail: Fable.exe (TLC, VC7.1) @ 0x00A5BBA0, 840 bytes, __cdecl, plain `ret`, eight stack arguments read through ebx
//   (the /Zi realigned frame: `push ebx ; mov ebx,esp ; sub esp,8 ; and esp,-16 ; add esp,4 ; push ebp ; mov ebp,[ebx+4] ;
//   mov [esp+4],ebp ; mov ebp,esp`).
// Identity (proven): Ego_r.pdb donor order of the NVectorMath TU (GFInitVectorMath, GFInterpolateVectors (KKM), (K1KKM),
//   (K1KPBMKK), GFFinishPartialSlerp = retail 0x00A5B850, 0x00A5B870, 0x00A5BBA0, 0x00A5BEF0, 0x00A5C2B0); this body
//   takes three strided streams and one weight ([ebx+24h] both as the SSE splat and as the x87 `fmul [ebx+24h]`), the
//   (K1KKM) signature; callers 0x00AE12B2 / 0x00AE1671 are the two CParticleProgram::PrepareForRendering bodies
//   (0x00AE1260 / 0x00AE1600) interpolating particle streams by one blend weight. No callees. Global: the SSE-available
//   byte g_SSEAvailable 0x013D2880 (as the landed siblings).
// Shape: the landed 0x00A5BEF0 model with one weight: the x87 fallback walks local copies of the three stream pointers
//   and calls the __forceinline C4DVector::Interpolate (FableWin.pdb: const C4DVector Interpolate(const C4DVector&, float)
//   const, out of line at 0x00A5B7D0 in this TU); VC7.1 unrolls it four times with a remainder loop.
#include "engine/NVectorMathSSE.h"
class C4DVector {
public:
    float X, Y, Z, W;
    C4DVector() {}
    C4DVector(float x, float y, float z, float w) : X(x), Y(y), Z(z), W(w) {}
    const C4DVector operator-(const C4DVector& o) const { return C4DVector(X - o.X, Y - o.Y, Z - o.Z, W - o.W); }
    const C4DVector operator+(const C4DVector& o) const { return C4DVector(X + o.X, Y + o.Y, Z + o.Z, W + o.W); }
    const C4DVector operator*(float s) const { return C4DVector(X * s, Y * s, Z * s, W * s); }
    __forceinline const C4DVector Interpolate(const C4DVector& to, float t) const { return (to - *this) * t + *this; }
};
extern bool g_SSEAvailable;                                          // 0x013D2880
namespace NVectorMath {
void GFInterpolateVectors(C4DVector* out, unsigned long outStride, const C4DVector* from, unsigned long fromStride,
                          const C4DVector* to, unsigned long toStride, unsigned long count, float weight)
{
    if (g_SSEAvailable) {
        __declspec(align(16)) C4DVector w;
        __declspec(align(16)) C4DVector oneMinusW;
        w.X = weight;
        oneMinusW.X = 1.0f - weight;
        GF_SSE_INTERPOLATE_VECTORS_FROM_TO(out, from, to, count, oneMinusW, w, outStride, fromStride, toStride);
    } else
    {
        C4DVector* pOut = out;
        const C4DVector* pFrom = from;
        const C4DVector* pTo = to;
        for (unsigned long i = 0; i < count; ++i) {
            *pOut = pFrom->Interpolate(*pTo, weight);
            pOut = (C4DVector*)((char*)pOut + outStride);
            pFrom = (const C4DVector*)((const char*)pFrom + fromStride);
            pTo = (const C4DVector*)((const char*)pTo + toStride);
        }
    }
}
}   // namespace NVectorMath