// libvorbis::unitnorm(float x) -- return +1.0f or -1.0f with the sign of x (bit surgery on the float)
// Leaf: unitnorm
// Retail: Fable.exe (TLC, VC7.1) @ 0x00C92640, 25 bytes. libvorbis 1.0.1 lib/scales.h (a static helper each
//   including TU gets; this copy is psy.c's, used by _vp_couple), compiled as C with /Gr: `__fastcall` with the
//   float on the stack (`ret 4`). No callees.
// Shape: the shipped pointer form -- the word is rewritten in the argument slot and reloaded (`mov [esp+4],eax ;
//   fld [esp+4]`); VC folds `(i & 0x80000000) | 0x3f800000` into `and 0BF800000h ; or 3F800000h`. Given
//   external linkage here so the unreferenced static is still emitted.
typedef unsigned int ogg_uint32_t;

extern "C" float __fastcall unitnorm(float x)
{
    ogg_uint32_t* ix = (ogg_uint32_t*)&x;
    *ix = (*ix & 0x80000000UL) | (0x3f800000UL);
    return x;
}