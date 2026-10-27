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
    __asm {
        mov     esi, src
        mov     edi, dst
        movq    mm0, [esi]
        movq    [edi], mm0
        movq    mm1, [esi + 8h]
        movq    [edi + 8h], mm1
        movq    mm2, [esi + 10h]
        movq    [edi + 10h], mm2
        movq    mm3, [esi + 18h]
        movq    [edi + 18h], mm3
        movq    mm4, [esi + 20h]
        movq    [edi + 20h], mm4
        movq    mm5, [esi + 28h]
        movq    [edi + 28h], mm5
        movq    mm6, [esi + 30h]
        movq    [edi + 30h], mm6
        movq    mm7, [esi + 38h]
        movq    [edi + 38h], mm7
        movq    mm0, [esi + 40h]
        movq    [edi + 40h], mm0
        movq    mm1, [esi + 48h]
        movq    [edi + 48h], mm1
        movq    mm2, [esi + 50h]
        movq    [edi + 50h], mm2
        movq    mm3, [esi + 58h]
        movq    [edi + 58h], mm3
        movq    mm4, [esi + 60h]
        movq    [edi + 60h], mm4
        movq    mm5, [esi + 68h]
        movq    [edi + 68h], mm5
        movq    mm6, [esi + 70h]
        movq    [edi + 70h], mm6
        movq    mm7, [esi + 78h]
        movq    [edi + 78h], mm7
        emms
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
    std::printf(ok ? "00c70040_TEST PASS\n" : "00c70040_TEST FAIL\n");
    return ok ? 0 : 1;
}