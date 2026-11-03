// NVectorMath::GFInterpolateVectors(C4DVector* out, unsigned long outStride, const C4DVector* from, unsigned long fromStride, unsigned long count, float t) -- in-place lerp of `count` 4D vectors toward a second stream by one weight: out = out * (1 - t) + from * t, both streams advancing by their own byte stride; a hand SSE kernel when the CPU has SSE, else the C4DVector operators on the x87
// Leaf: ?GFInterpolateVectors@NVectorMath@@YAXPAVC4DVector@@KPBV2@KKM@Z
// Flags: /Zi
// Retail-asm: 0x00A5B8A6..0x00A5B8DC `mov eax,[ebx+8] ; mov edx,[ebx+10h] ; mov ecx,[ebx+18h] ; movaps xmm0,[ebp-30h] ; movaps xmm1,[ebp-40h] ; shufps xmm0,xmm0,0 ; shufps xmm1,xmm1,0 ; test ecx,ecx ; je ; movaps xmm2,[eax] ; movaps xmm3,[edx] ; mulps xmm2,xmm0 ; mulps xmm3,xmm1 ; addps xmm2,xmm3 ; movaps [eax],xmm2 ; add eax,[ebx+0Ch] ; add edx,[ebx+14h] ; dec ecx ; jne` -- the
//   hand SSE kernel of the NVectorMath family (same style as the landed retail_asm siblings GFInterpolateVectors
//   (K1KPBMKK) 0x00A5BEF0, GFFinishPartialSlerp 0x00A5C2B0 and GFClear4DVectors 0x00A5C300): the two pointers and the
//   count loaded into fixed registers straight from the argument slots, the two 16-aligned weight locals broadcast with
//   `shufps 0`, the strides added from memory every pass, the count counted down in ecx behind a `test ecx,ecx ; je`.
//   VC7.1 cannot emit this loop from C++ (no xmmintrin spelling gives the fixed registers and the memory strides).
//   The C++ around it is compiler-emitted: the SSE flag test, the two 16-byte aligned float[4] locals with element 0
//   assigned (`tt[0] = t` is the integer copy `mov eax,[ebx+1Ch] ; mov [ebp-40h],eax`, `oneMinusT[0] = 1.0f - t` the
//   `fld 1.0 ; fsub [ebx+1Ch] ; fstp [ebp-30h]`; a scalar aligned float gives a 0x58 frame with the locals on top,
//   the 16-byte arrays the retail 0x48 frame with the fallback's Interpolate temporaries overlapping them) and the
//   whole x87 fallback.
// Portable (2026-11-03 asm twins): the asm kernel is this function's own block (retail has no separate helper for it), so it stays here in two-path form: under FABLE_PORTABLE the #ifndef drops the SSE branch and the row runs retail's own non-SSE C++ path (Strategy B; equal to the SSE kernel up to float rounding order -- retail ran this path on CPUs without SSE).
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00A5B870, 801 bytes (+15 int3), __cdecl, plain `ret`, six stack
//   arguments read through ebx (the realigned frame). Ego_r.pdb names the overload
//   ?GFInterpolateVectors@NVectorMath@@YAXPAVC4DVector@@KPBV2@KKM@Z (NVectorMath namespace; the catalog label and the
//   reloc binding drop the namespace); donor order GFInitVectorMath, the (KKM), (K1KKM) and (K1KPBMKK) overloads,
//   GFFinishPartialSlerp = retail 0x00A5B850, 0x00A5B870, 0x00A5BBA0, 0x00A5BEF0, 0x00A5C2B0. No callees.
//   Globals: the SSE-available byte g_SSEAvailable 0x013D2880, the 1.0f constant 0x0122DED8.
// Shape:
//   - `/Zi` (debug info, as the retail TU was built) is what makes VC7.1 copy the return address into the realigned
//     frame (`sub esp,8 ; and esp,-16 ; add esp,4 ; push ebp ; mov ebp,[ebx+4] ; mov [esp+4],ebp ; mov ebp,esp`), as in
//     the landed sibling 0x00A5BEF0.
//   - x87 path: the inline C4DVector::Interpolate (FableWin.pdb: const C4DVector Interpolate(const C4DVector&, float)
//     const, out of line at 0x00A5B7D0 in this TU) needs `__forceinline` to inline here; the weight is the `t`
//     argument slot [ebx+1Ch] read by every fmul. VC7.1 unrolls the loop by four (`cmp count,4 ; jl`,
//     (count-4)/4+1 passes, then a remainder loop).
// Layout: C4DVector X +0, Y +4, Z +8, W +0xc (16 bytes, the SSE rows load it with movaps).
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
                          unsigned long count, float t)
{
#ifndef FABLE_PORTABLE   // portable build: the retail non-SSE C++ path below (the twin of the SSE asm kernel)
    if (g_SSEAvailable) {
        __declspec(align(16)) float tt[4];
        __declspec(align(16)) float oneMinusT[4];
        tt[0] = t;
        oneMinusT[0] = 1.0f - t;
        __asm {
            mov     eax, out
            mov     edx, from
            mov     ecx, count
            movaps  xmm0, xmmword ptr oneMinusT
            movaps  xmm1, xmmword ptr tt
            shufps  xmm0, xmm0, 0
            shufps  xmm1, xmm1, 0
            test    ecx, ecx
            je      done
        next:
            movaps  xmm2, [eax]
            movaps  xmm3, [edx]
            mulps   xmm2, xmm0
            mulps   xmm3, xmm1
            addps   xmm2, xmm3
            movaps  [eax], xmm2
            add     eax, outStride
            add     edx, fromStride
            dec     ecx
            jne     next
        done:
        }
    } else
#endif
    {
        C4DVector* pOut = out;
        const C4DVector* pFrom = from;
        for (unsigned long i = 0; i < count; ++i) {
            *pOut = pOut->Interpolate(*pFrom, t);
            pOut = (C4DVector*)((char*)pOut + outStride);
            pFrom = (const C4DVector*)((const char*)pFrom + fromStride);
        }
    }
}
}   // namespace NVectorMath