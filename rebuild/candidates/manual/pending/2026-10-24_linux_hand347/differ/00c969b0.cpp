// _global::FFT_InitFactorsUnlessTrivial_00c969b0(long n, long* table, long* factors) -- FFT setup step: unless the transform length is 1, factor n and fill the twiddle/factor tables through 0x00C96800
// Grade: DIFFER(ours v retail) -- retail takes n in EAX, table in EDX, factors in ECX (an LTCG custom register convention); VC7.1 C++ has no eax-argument convention
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00c969b0, 17 bytes (+ int3 padding):
//   cmp eax,1 ; je ret ; push ecx ; lea ecx,[edx+eax*4] ; push ecx ; push eax ; call 0x00C96800 ; ret
// Callee: 0x00C96800 (manifest FFT_InitializeTwiddleFactors; it divides n by 4, 2, 3, 5.. like
// KISS FFT's kf_factor and returns with `ret 0Ch`, callee-clean).
// Only reference: the tail `jmp` at 0x00C9BC80 of the FFT state constructor 0x00C9BC50, which stores
// n, calloc(3n, 4) and calloc(32, 4) and jumps here with n/table/factors left in eax/edx/ecx.
// Identity: the catalog label `_Cons_val<allocator<CCharString>,CCharString,const char(&)[5]>` is
// propagated over this body and wrong.  GOTCHAS (b84): a target that itself receives register
// arguments is not gradable by ltcg_grade.py either (the target is exported, forcing the standard
// convention).  This __fastcall stand-in is the nearest plain-C++ form.
void __stdcall FFT_Factor_00c96800(long n, long* table, long* factors);   // 0x00C96800

void __fastcall FFT_InitFactorsUnlessTrivial_00c969b0(long* factors, long* table, long n)
{
    if (n != 1)
        FFT_Factor_00c96800(n, table + n, factors);
}
