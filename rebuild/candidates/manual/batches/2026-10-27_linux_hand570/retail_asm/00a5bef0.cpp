// NVectorMath::GFInterpolateVectors(C4DVector* out, unsigned long outStride, const C4DVector* from, unsigned long fromStride, const C4DVector* to, unsigned long toStride, const float* weights, unsigned long weightStride, unsigned long count) -- per-element lerp of `count` 4D vectors: out = from + (to - from) * weight, every stream advancing by its own byte stride; a hand SSE kernel when the CPU has SSE, else the C4DVector operators on the x87
// Leaf: ?GFInterpolateVectors@NVectorMath@@YAXPAVC4DVector@@KPBV2@K1KPBMKK@Z
// Flags: /Zi
// Retail-asm: 0x00A5BF30..0x00A5BF74 `mov edi,[ebx+8] ; mov esi,[ebx+10h] ; mov edx,[ebx+18h] ; mov eax,[ebx+20h] ; mov ecx,[ebx+28h] ; movaps xmm4,[ebp-40h] ; test ecx,ecx ; je ; movss xmm1,[eax] ; shufps xmm1,xmm1,0 ; movaps xmm0,xmm4 ; subps xmm0,xmm1 ; movaps xmm2,[esi] ; movaps xmm3,[edx] ; mulps xmm2,xmm0 ; mulps xmm3,xmm1 ; addps xmm2,xmm3 ; movaps [edi],xmm2 ; add edi,[ebx+0Ch] ; add esi,[ebx+14h] ; add edx,[ebx+1Ch] ; add eax,[ebx+24h] ; dec ecx ; jne` -- the
//   hand SSE kernel of the NVectorMath family (same style as the landed retail_asm siblings GFFinishPartialSlerp 0x00A5C2B0
//   and GFClear4DVectors 0x00A5C300): the five pointers/count loaded into fixed registers straight from the argument
//   slots, the strides added from memory every pass, the count counted down in ecx behind a `test ecx,ecx ; je`.
//   The xmmintrin spelling (probe: `_mm_set_ps1` + `_mm_load_ps1` + mul/add/sub) gives a `push ebp ; mov ebp,esp ;
//   and esp,-16` frame, a `movss`+`shufps` constant and a re-scheduled loop instead, so the block is the original's
//   `__asm`. The C++ around it is compiler-emitted: the SSE flag test, the 16-aligned `one` vector (four immediate
//   stores of 1.0f into [ebp-40h], the reason for the ebx/ebp realigned frame), and the whole x87 fallback.
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00A5BEF0, 946 bytes (+14 int3), __cdecl, plain `ret`, nine stack
//   arguments read through ebx (the realigned frame). Ego_r.pdb names the overload
//   ?GFInterpolateVectors@NVectorMath@@YAXPAVC4DVector@@KPBV2@K1KPBMKK@Z; donor order GFInitVectorMath, the (KKM),
//   (K1KKM) and (K1KPBMKK) overloads, GFFinishPartialSlerp = retail 0x00A5B850, 0x00A5B870, 0x00A5BBA0, 0x00A5BEF0,
//   0x00A5C2B0. No callees. Global: the SSE-available byte g_SSEAvailable 0x013D2880.
// Shape:
//   - `/Zi` (debug info, as the retail TU was built) is what makes VC7.1 copy the return address into the realigned
//     frame (`sub esp,8 ; and esp,-16 ; add esp,4 ; push ebp ; mov ebp,[ebx+4] ; mov [esp+4],ebp ; mov ebp,esp`);
//     without it the same source emits `push ecx` and no copy (the "16-aligned frame copying the return address" that
//     chunk 511 parked as a lane gap).
//   - x87 path: the inline C4DVector::Interpolate (FableWin.pdb: const C4DVector Interpolate(const C4DVector&, float)
//     const, out of line at 0x00A5B7D0 in this TU) needs `__forceinline` to inline here; the weight argument is
//     evaluated first (right-to-left), so `fld [weight]` precedes the subtraction and stays in st(2).
//   - The fallback walks local copies of the four stream pointers (the weight copy lives in [ebp-4]); walking the
//     parameters themselves writes the weight pointer back to its argument slot. VC7.1 unrolls the loop by four
//     (`cmp count,4 ; jl`, (count-4)/4+1 passes, then a remainder loop).
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
                          const C4DVector* to, unsigned long toStride, const float* weights, unsigned long weightStride,
                          unsigned long count)
{
    if (g_SSEAvailable) {
        __declspec(align(16)) C4DVector one(1.0f, 1.0f, 1.0f, 1.0f);
        __asm {
            mov     edi, out
            mov     esi, from
            mov     edx, to
            mov     eax, weights
            mov     ecx, count
            movaps  xmm4, one
            test    ecx, ecx
            je      done
        next:
            movss   xmm1, [eax]
            shufps  xmm1, xmm1, 0
            movaps  xmm0, xmm4
            subps   xmm0, xmm1
            movaps  xmm2, [esi]
            movaps  xmm3, [edx]
            mulps   xmm2, xmm0
            mulps   xmm3, xmm1
            addps   xmm2, xmm3
            movaps  [edi], xmm2
            add     edi, outStride
            add     esi, fromStride
            add     edx, toStride
            add     eax, weightStride
            dec     ecx
            jne     next
        done:
        }
    } else {
        C4DVector* pOut = out;
        const C4DVector* pFrom = from;
        const C4DVector* pTo = to;
        const float* pWeight = weights;
        for (unsigned long i = 0; i < count; ++i) {
            *pOut = pFrom->Interpolate(*pTo, *pWeight);
            pOut = (C4DVector*)((char*)pOut + outStride);
            pFrom = (const C4DVector*)((const char*)pFrom + fromStride);
            pTo = (const C4DVector*)((const char*)pTo + toStride);
            pWeight = (const float*)((const char*)pWeight + weightStride);
        }
    }
}
}   // namespace NVectorMath
