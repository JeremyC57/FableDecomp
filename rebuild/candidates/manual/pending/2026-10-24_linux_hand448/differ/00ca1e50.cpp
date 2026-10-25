// libvorbis::vorbis_dBquant(const float* x) -- libvorbis floor1.c: quantise a dB value to the 10-bit floor-1 scale (x*7.3142857+1023.5, clamped to 0..1023)
// Grade: DIFFER(43v43) -- lane gap: every instruction matches except `fld dword ptr [eax]` v ours `[ecx]`: retail takes x in eax (LTCG custom register convention, `fld dword ptr [eax]`); a C++ declaration gives ecx or a stack argument
// Leaf: @vorbis_dBquant@4
// Retail: Fable.exe (TLC, VC7.1) @ 0x00CA1E50, 43 bytes (+5 int3). libvorbis lib/floor1.c static helper; callee _ftol2
//   0x00BFEA70 (the float-to-int conversion); constants 7.3142857f @ 0x012BF35C, 1023.5f @ 0x012BF358.
// Label: manifest "Clamp_To10BitValue" is a descriptive placeholder; the real name is the floor1.c static `vorbis_dBquant`.
extern "C" int __fastcall vorbis_dBquant(const float* x)
{
    int i = *x * 7.3142857f + 1023.5f;
    if (i > 1023) return (1023);
    if (i < 0) return (0);
    return i;
}
