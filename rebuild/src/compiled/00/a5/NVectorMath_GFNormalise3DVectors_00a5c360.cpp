// 00a5c360 NVectorMath::GFNormalise3DVectors
// Evidence: FableWin lib_vector_math.obj marks this five-argument overload as containing inline assembly.
// Leaf: ?GFNormalise3DVectors@NVectorMath@@YAXPBVC4DVector@@KPAV2@KK@Z
// Flags: /Zi
// Retail-asm-helper: GF_SSE_NORMALISE_3D_VECTORS
// Retail-asm: 0x00A5C3B6..0x00A5C46F, aligned pair loop and count-preserving odd tail.
// Inferred: /Zi retains the realigned return-address-copy prologue of this TU.
#define FABLE_RETAIL_C3DVECTOR
#define FABLE_RETAIL_VECTOR_NORMALISE_PASS29
#define FABLE_RETAIL_C3DVECTOR_NORMALISE_SQUARE_HELPER
#define FABLE_RETAIL_C3DVECTOR_MAGNITUDE_SQRTF
#include "engine/C4DVector.h"
#include "engine/NVectorMath.h"
#include "engine/NVectorMathSSE.h"
namespace NVectorMath {
void GFNormalise3DVectors(const C4DVector* src, unsigned long src_stride, C4DVector* dest, unsigned long dest_stride, unsigned long count)
{
    if (GEnableSIMD) {
        float md = 0.01f;
#if defined(_MSC_VER)
        __declspec(align(16))
#endif
        C4DVector min_divisor(md, md, md, md);
        unsigned long src_stride_2 = src_stride * 2;
        unsigned long dest_stride_2 = dest_stride * 2;
        GF_SSE_NORMALISE_3D_VECTORS(src, dest, count, src_stride_2, dest_stride_2, min_divisor)
    } else {
        for (unsigned long c1 = 0; c1 < count; ++c1) {
            C4DVector vect(src[c1].ToC3DVector(), 0.0f);
            float dist = GFRoot(GFSqr(vect.X) + GFSqr(vect.Y) + GFSqr(vect.Z) + GFSqr(vect.W));
            vect /= dist;
            dest[c1] = vect;
        }
    }
}
}