// Pow10_00c40e60(float x) -- libpk 10^x as 2^(x*log2(10)): split the exponent into its rounded integer and fractional parts, 2^frac by f2xm1, 2^whole by fscale
// Grade: DIFFER(109v75) -- retail_asm candidate: retail inline x87 asm (frndint, fld1/fscale, f2xm1, fwait after each fst) that VC7.1 cannot emit from C++; this pass forbids __asm
// Leaf: ?Pow10_00c40e60@@YAMM@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C40E60, 75 bytes (+ int3 padding), __cdecl, plain `ret`.
// Identity (descriptive): Pow10_00c40e60(float); evidence: 75 bytes byte-identical to the other five rows (no folding: one per translation unit), `fmul` by log2(10) = 0x40549A78, frndint / fld1 ; fscale / f2xm1; no call, jump or data reference reaches it (an unreferenced inline-helper COMDAT; its neighbours 0x00C40E50 (std::std::pow) and 0x00C40EB0 (unlabelled) are libpk code)
// Label: the manifest's "Pow2_Float" is a descriptive catalog name (family J032: six copies of one inline x87 helper, one per libpk TU).
// Callees: none.
// Retail-asm candidate: the body is retail inline assembly -- `frndint`, `fscale`, `f2xm1` and the `wait`s after each `fst`
//   have no C++ source form under VC7.1 (no intrinsic emits them), the operands are spilled through named stack temporaries
//   ([esp] = 1.0f, [esp+4] = log2(10) = 0x40549A78, [esp+8] = the rounded exponent, [esp+0xC] / [esp+0x10] the asm outputs).
//   This pass forbids __asm, so the source below is the plain C++ reference of the same computation (it cannot match).
#include <math.h>

float Pow10_00c40e60(float x)
{
    const float log2Of10 = 3.32192802f;                     // 0x40549A78
    const float one = 1.0f;
    float whole = (float)floor(log2Of10 * x + 0.5f);        // frndint (round to nearest)
    float scale = (float)ldexp(one, (int)whole);            // fld1 ; fscale
    float frac = (float)(pow(2.0, (double)(log2Of10 * x - whole)) - 1.0);   // f2xm1 over |frac| <= 0.5
    return (frac + one) * scale;
}
