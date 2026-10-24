// NVectorMath::GFFinishPartialSlerp(CQuaternion* out, unsigned long outStride, const CQuaternion* in, unsigned long inStride, const CQuaternion* weights, unsigned long weightStride, unsigned long count) -- SSE blend over strided quaternion arrays: out = out * w.x + in * w.y for every element, each weight quaternion carrying the two blend factors
// Grade: DIFFER(98v67) -- retail_asm candidate: retail is hand SSE (`movaps xmm1,xmm0 ; shufps xmm0,xmm0,0 ; shufps xmm1,xmm1,55h`, the strides added straight from [ebp+N] each pass, the count in ecx with `dec ecx ; jne`, an ebp frame with no `and esp,-16`); the intrinsic reference body gets an aligned frame, spills the count and keeps the strides in registers
// Leaf: ?GFFinishPartialSlerp@NVectorMath@@
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00A5C2B0, 67 bytes (+13 int3), `__cdecl`, plain `ret` (PDB
//   ?GFFinishPartialSlerp@NVectorMath@@YAXPAVCQuaternion@@KPBV2@K1KK@Z). No callees.
// Shape: SSE intrinsics (VC7.1 builds the `push ebp ; mov ebp,esp` frame for the __m128 locals and re-reads the stride
//   arguments from it): aligned loads of the weight, the output and the input, `shufps` broadcasts of the weight's
//   x and y lanes, two `mulps`, an `addps` and an aligned store back to the output; the pointers advance by their byte
//   strides; the count runs down to zero (`dec ecx ; jne`).
#include <xmmintrin.h>

class CQuaternion {
public:
    float X, Y, Z, W;
};

namespace NVectorMath {

void GFFinishPartialSlerp(CQuaternion* out, unsigned long outStride, const CQuaternion* in, unsigned long inStride,
                          const CQuaternion* weights, unsigned long weightStride, unsigned long count)
{
    for (; count != 0; --count) {
        __m128 w = _mm_load_ps(&weights->X);
        __m128 o = _mm_load_ps(&out->X);
        __m128 i = _mm_load_ps(&in->X);
        weights = (const CQuaternion*)((const char*)weights + weightStride);
        in = (const CQuaternion*)((const char*)in + inStride);
        __m128 wx = _mm_shuffle_ps(w, w, 0x00);
        __m128 wy = _mm_shuffle_ps(w, w, 0x55);
        _mm_store_ps(&out->X, _mm_add_ps(_mm_mul_ps(o, wx), _mm_mul_ps(i, wy)));
        out = (CQuaternion*)((char*)out + outStride);
    }
}

}   // namespace NVectorMath
