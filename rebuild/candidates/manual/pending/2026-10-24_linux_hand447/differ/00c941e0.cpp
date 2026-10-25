// libvorbis::couple_lossless(float A, float B, float* qA, float* qB) -- lossless square-polar M/A coupling of one quantised line pair
// Grade: DIFFER(202v202) -- LTCG register argument: retail passes qA in EDX and qB in ECX (the reverse of `__fastcall`), every other byte matches; declaring the pointers in swapped order `(float A, float B, float* qB, float* qA)` grades RELOCATION_MATCH (probe/00c941e0_swapped.cpp) but misstates the libvorbis signature
// Retail: Fable.exe @ 0x00C941E0, 202 bytes (`ret 8`, A/B on the stack). libvorbis 1.0.1 lib/psy.c (static, called from _vp_couple). Data: 0.0f @ 0x0122DEDC, the doubles 1.9999 @ 0x012BF318 and -2.0 @ 0x0129C4A0 (VC folds `-fabs(x)*2.f` into `x * -2.0`). Grades: 2 (natural order, swapped order).
// Lane gap: LTCG custom calling convention (VC7.1 /GL); the body is the shipped libvorbis 1.0.1 source.
// Leaf: couple_lossless
extern "C" double __cdecl fabs(double x);
#pragma intrinsic(fabs)

extern "C" void __fastcall couple_lossless(float A, float B, float* qA, float* qB)
{
    int test1 = fabs(*qA) > fabs(*qB);
    test1 -= fabs(*qA) < fabs(*qB);

    if (!test1) test1 = ((fabs(A) > fabs(B)) << 1) - 1;
    if (test1 == 1) {
        *qB = (*qA > 0.f ? *qA - *qB : *qB - *qA);
    } else {
        float temp = *qB;
        *qB = (*qB > 0.f ? *qA - *qB : *qB - *qA);
        *qA = temp;
    }

    if (*qB > fabs(*qA) * 1.9999f) {
        *qB = -fabs(*qA) * 2.f;
        *qA = -*qA;
    }
}
