// libvorbis::vorbis_fromdBlook(float a) -- libvorbis lookup.c (compiled into lsp.c): table lookup of 10^(a/20) over -140dB..0dB (1.f above 0dB, 0.f below the table)
// Leaf: @vorbis_fromdBlook@4
// Retail: Fable.exe (TLC, VC7.1) @ 0x00CA3F00, 81 bytes (+15 int3). libvorbis lib/lookup.c, #included by lsp.c under
//   FLOAT_LOOKUP (libpk's statically linked Xiph libvorbis, "Xiph.Org libVorbis I 20030909"); `__fastcall` with the float on
//   the stack (`ret 4`), called by vorbis_lsp_to_curve 0x00CA46C7.
// Callee: vorbis_ftoi 0x00CA3B10 (os.h inline-asm rounding helper; landed as `extern "C" long __stdcall vorbis_ftoi(double)`).
// Data: FROMdB_LOOKUP @ 0x013AC5F8 (35 floats), FROMdB2_LOOKUP @ 0x013AC688 (32 floats); constants -8.f @ 0x0125A298,
//   .5f @ 0x0122F59C, 1.f @ 0x0122DED8, 0.f @ 0x0122DEDC.
// Shape: the shipped source; the nested ?: keeps the three `ret 4` exits, and FROMdB_LOOKUP[i>>5]*FROMdB2_LOOKUP[i&31]
//   loads the right operand first (`fld [i&31 table] ; fmul [i>>5 table]`).
// Label: manifest "Math_InterpolatePowerLookup" is a descriptive placeholder; the real name is the libvorbis vorbis_fromdBlook.
#define FROMdB_LOOKUP_SZ 35
#define FROMdB2_LOOKUP_SZ 32
#define FROMdB_SHIFT 5
#define FROMdB2_SHIFT 3
#define FROMdB2_MASK 31
extern "C" float FROMdB_LOOKUP[FROMdB_LOOKUP_SZ];    // 0x013AC5F8
extern "C" float FROMdB2_LOOKUP[FROMdB2_LOOKUP_SZ];  // 0x013AC688
extern "C" long __stdcall vorbis_ftoi(double f);     // 0x00CA3B10

extern "C" float __fastcall vorbis_fromdBlook(float a)
{
    int i = vorbis_ftoi(a * ((float)(-(1 << FROMdB2_SHIFT))) - .5f);
    return (i < 0) ? 1.f :
        ((i >= (FROMdB_LOOKUP_SZ << FROMdB_SHIFT)) ? 0.f :
         FROMdB_LOOKUP[i >> FROMdB_SHIFT] * FROMdB2_LOOKUP[i & FROMdB2_MASK]);
}