#if !defined(_MSC_VER) && !defined(FABLE_PORTABLE)
#define FABLE_PORTABLE   // a non-MSVC build takes the helpers' plain C++ twins
#endif
#include "libpk/MixKernelsSIMD.h"
#include <cstdio>
// behaviour test of CPreTransposedBoneMatrix::Set (0x00c70040): a 128-byte aligned copy (sixteen MMX movq load/store pairs and emms)
#ifdef _MSC_VER
#define ALIGN16 __declspec(align(16))
#else
#define ALIGN16 __attribute__((aligned(16)))
#endif
class CMatrix3x4 { public: float M[32]; };
struct CPreTransposedBoneMatrix {
public:
    float M[32];
    static void Set(CPreTransposedBoneMatrix* dst, const CMatrix3x4* src);
};
#ifdef _MSC_VER
void CPreTransposedBoneMatrix::Set(CPreTransposedBoneMatrix* dst, const CMatrix3x4* src)
{
    LIBPK_MMX_COPY128(src, dst);
}
#else
void CPreTransposedBoneMatrix::Set(CPreTransposedBoneMatrix* dst, const CMatrix3x4* src)
{
    for (int i = 0; i < 32; ++i) dst->M[i] = src->M[i];
}
#endif
int main()
{
    ALIGN16 CMatrix3x4 src;
    ALIGN16 CPreTransposedBoneMatrix dst;
    ALIGN16 CPreTransposedBoneMatrix guard;
    for (int i = 0; i < 32; ++i) { src.M[i] = 0.5f * i - 3.0f; dst.M[i] = -1.0f; guard.M[i] = 42.0f; }
    CPreTransposedBoneMatrix::Set(&dst, &src);
    bool ok = true;
    for (int i = 0; i < 32; ++i) ok = ok && dst.M[i] == 0.5f * i - 3.0f && src.M[i] == 0.5f * i - 3.0f && guard.M[i] == 42.0f;
    std::printf(ok ? "00c70040_TEST PASS\n" : "00c70040_TEST FAIL\n");
    return ok ? 0 : 1;
}