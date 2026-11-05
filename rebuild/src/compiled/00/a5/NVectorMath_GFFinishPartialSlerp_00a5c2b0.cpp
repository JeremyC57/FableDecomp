// NVectorMath::GFFinishPartialSlerp(CQuaternion* out, unsigned long outStride, const CQuaternion* in, unsigned long inStride, const CQuaternion* weights, unsigned long weightStride, unsigned long count) -- finish a batch of partial slerps: out = out * weight.x + in * weight.y for `count` quaternions, each stream advancing by its byte stride
// Leaf: ?GFFinishPartialSlerp@NVectorMath@@YAXPAVCQuaternion@@KPBV2@K1KK@Z
// Retail-asm: 0x00A5C2B4..0x00A5C2EE `mov eax,[ebp+8] ; mov edx,[ebp+10h] ; mov esi,[ebp+18h] ; mov ecx,[ebp+20h] ; test ecx,ecx ; je ; movaps xmm0,[esi] ; movaps xmm2,[eax] ; movaps xmm3,[edx] ; add esi,[ebp+1Ch] ; add edx,[ebp+14h] ; movaps xmm1,xmm0 ; shufps xmm0,xmm0,0 ; shufps xmm1,xmm1,55h ; mulps xmm2,xmm0 ; mulps xmm3,xmm1 ; addps xmm2,xmm3 ; movaps [eax],xmm2 ; add eax,[ebp+0Ch] ; dec ecx ; jne` -- a hand SSE kernel (22 instructions, a je and the loop jne: rasm1 family (f)): fixed registers, the strides added straight from the argument slots, the count counted down in ecx. The xmmintrin spelling (rasm1 probe probe/intr/00a5c2b0.cpp) gives an `and esp,-16` aligned frame, register copies and `movaps x,y ; shufps x,y` pairs instead, so the block is the original's inline asm. The function is the block: the prologue/epilogue (ebp frame, `push esi` for the register the block names) are compiler-emitted.
// Helper (2026-11-05 asm migration): the retail inline-asm block is the shared engine/NVectorMathSSE.h (byte-matching path + FABLE_PORTABLE twin); the row has
//   no asm of its own. C++/intrinsic attempts: the batch ATTEMPTS.tsv, family SSE_NVECTORMATH.
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00A5C2B0, 67 bytes (+13 int3), __cdecl, plain `ret` (Ego_r.pdb
//   ?GFFinishPartialSlerp@NVectorMath@@YAXPAVCQuaternion@@KPBV2@K1KK@Z). No callees.
#include "engine/NVectorMathSSE.h"
class CQuaternion {
public:
    float X, Y, Z, W;
};
namespace NVectorMath {
#pragma optimize("y", off)
void GFFinishPartialSlerp(CQuaternion* out, unsigned long outStride, const CQuaternion* in, unsigned long inStride,
                          const CQuaternion* weights, unsigned long weightStride, unsigned long count)
{
    GF_SSE_FINISH_PARTIAL_SLERP(out, in, weights, count, weightStride, inStride, outStride);
}
}   // namespace NVectorMath