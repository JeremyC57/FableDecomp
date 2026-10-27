#include <cstdio>
// behaviour test of CPreTransposedBoneMatrix::Set (0x00c70a60): a 128-byte aligned copy (eight SSE2 movdqa loads then eight stores)
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
    __asm {
        mov     esi, src
        mov     edi, dst
        movdqa  xmm0, [esi]
        movdqa  xmm1, [esi + 10h]
        movdqa  xmm2, [esi + 20h]
        movdqa  xmm3, [esi + 30h]
        movdqa  xmm4, [esi + 40h]
        movdqa  xmm5, [esi + 50h]
        movdqa  xmm6, [esi + 60h]
        movdqa  xmm7, [esi + 70h]
        movdqa  [edi], xmm0
        movdqa  [edi + 10h], xmm1
        movdqa  [edi + 20h], xmm2
        movdqa  [edi + 30h], xmm3
        movdqa  [edi + 40h], xmm4
        movdqa  [edi + 50h], xmm5
        movdqa  [edi + 60h], xmm6
        movdqa  [edi + 70h], xmm7
    }
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
    std::printf(ok ? "00c70a60_TEST PASS\n" : "00c70a60_TEST FAIL\n");
    return ok ? 0 : 1;
}