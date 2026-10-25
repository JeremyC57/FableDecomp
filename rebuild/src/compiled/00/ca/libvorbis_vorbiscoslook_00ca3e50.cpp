// libvorbis::vorbis_coslook(float a) -- libvorbis lookup.c (compiled into lsp.c): table-interpolated cos(a) over 0..PI (COS_LOOKUP_SZ 128)
// Leaf: @vorbis_coslook@4
// Retail: Fable.exe (TLC, VC7.1) @ 0x00CA3E50, 71 bytes (+9 int3). libvorbis lib/lookup.c, #included by lsp.c under
//   FLOAT_LOOKUP (libpk's statically linked Xiph libvorbis, "Xiph.Org libVorbis I 20030909"); `__fastcall` with the float on
//   the stack (`ret 4`), called by vorbis_lsp_to_curve 0x00CA45A0 / 0x00CA4603.
// Callee: vorbis_ftoi 0x00CA3B10 (os.h, the inline-asm `fistp` rounding helper kept out of line; landed as
//   `extern "C" long __stdcall vorbis_ftoi(double)`, callee-pops its 8-byte double).
// Data: COS_LOOKUP @ 0x013AC260 (129 floats, lookup_data.h); constants .31830989*128 (double) @ 0x012BF378, .5 (double)
//   @ 0x012316F0.
// Shape: the shipped source; `d` lives in a frame double, `i` is stored into the dead argument slot for the `fild`, and the
//   difference COS_LOOKUP[i+1]-COS_LOOKUP[i] is formed first.
// Label: manifest "Math_InterpolateFloatTableLookup" is a descriptive placeholder; the real name is the libvorbis vorbis_coslook.
#define COS_LOOKUP_SZ 128
extern "C" float COS_LOOKUP[COS_LOOKUP_SZ + 1];      // 0x013AC260
extern "C" long __stdcall vorbis_ftoi(double f);     // 0x00CA3B10

extern "C" float __fastcall vorbis_coslook(float a)
{
    double d = a * (.31830989 * (float)COS_LOOKUP_SZ);
    int i = vorbis_ftoi(d - .5);

    return COS_LOOKUP[i] + (d - i) * (COS_LOOKUP[i + 1] - COS_LOOKUP[i]);
}