// libvorbis::apsort(const void* a, const void* b) -- qsort comparator over float pointers: descending by absolute value
// Leaf: apsort
// Retail: Fable.exe (TLC, VC7.1) @ 0x00C945B0, 70 bytes. libvorbis 1.0.1 lib/psy.c (static, handed to qsort by
//   _vp_noise_normalize_sort 0x00C94750 and _vp_quantize_couple_sort), `__cdecl` (a qsort callback; plain `ret`).
//   No callees; fabs is the x87 intrinsic.
// Shape: the shipped source; both magnitudes stay on the x87 stack, `f1 < f2` is `fld st1 ; fcomp st1 ; test
//   ah,5` and `f1 > f2` reuses them after `fxch`; the subtraction is duplicated into both arms of the second
//   compare.
extern "C" double __cdecl fabs(double x);
#pragma intrinsic(fabs)

extern "C" int __cdecl apsort(const void* a, const void* b)
{
    float f1 = fabs(**(float**)a);
    float f2 = fabs(**(float**)b);
    return (f1 < f2) - (f1 > f2);
}