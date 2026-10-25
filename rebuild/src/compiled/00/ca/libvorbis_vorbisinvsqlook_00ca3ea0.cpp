// libvorbis::vorbis_invsqlook(float a) -- libvorbis lookup.c (compiled into lsp.c): table-interpolated 1/sqrt(a) for .5 <= a < 1 (INVSQ_LOOKUP_SZ 32)
// Leaf: @vorbis_invsqlook@4
// Retail: Fable.exe (TLC, VC7.1) @ 0x00CA3EA0, 77 bytes (+3 int3). libvorbis lib/lookup.c, #included by lsp.c under
//   FLOAT_LOOKUP (libpk's statically linked Xiph libvorbis, "Xiph.Org libVorbis I 20030909"); `__fastcall` with the float on
//   the stack (`ret 4`), called by vorbis_lsp_to_curve 0x00CA46B2.
// Callee: vorbis_ftoi 0x00CA3B10 (os.h inline-asm rounding helper; landed as `extern "C" long __stdcall vorbis_ftoi(double)`).
// Data: INVSQ_LOOKUP @ 0x013AC468 (lookup_data.h); constants 2.f*32 @ 0x0126973C and 32.f @ 0x01230894 (float), .5 (double,
//   `d-.5f` promotes) @ 0x012316F0.
// Shape: the shipped source; `a*64.f-32.f` is float arithmetic stored to the double `d` with `fst qword`.
// Label: manifest "Math_InterpolateAngleLookup" is a descriptive placeholder; the real name is the libvorbis vorbis_invsqlook.
#define INVSQ_LOOKUP_SZ 32
extern "C" float INVSQ_LOOKUP[INVSQ_LOOKUP_SZ + 1];  // 0x013AC468
extern "C" long __stdcall vorbis_ftoi(double f);     // 0x00CA3B10

extern "C" float __fastcall vorbis_invsqlook(float a)
{
    double d = a * (2.f * (float)INVSQ_LOOKUP_SZ) - (float)INVSQ_LOOKUP_SZ;
    int i = vorbis_ftoi(d - .5f);
    return INVSQ_LOOKUP[i] + (d - i) * (INVSQ_LOOKUP[i + 1] - INVSQ_LOOKUP[i]);
}