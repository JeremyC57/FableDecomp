// Grade: DIFFER(15v15) -- IMPOSSIBLE: retail rounds with a bare dword `fistp dword ptr [esp]` into a pushed slot; plain `(long)x` under /QIfist gives `fistp qword ptr [esp+4]` (no `push ecx`), without /QIfist a __ftol2 call. The original is libvorbis os.h vorbis_ftoi, whose MSVC branch is an inline-assembly block: not reproducible as compiled C++ (asm_bake family, GOTCHAS 493).
// vorbis_ftoi(double f) -- libvorbis float-to-int rounding helper (os.h), current FPU rounding mode
// Retail 0x00CA3B10, 15 bytes: push ecx ; fld qword [esp+8] ; fistp dword [esp] ; mov eax,[esp] ;
// pop ecx ; ret 8. Callers 0x00CA3E6D / 0x00CA3EC3 / 0x00CA3F16 are libvorbis' table
// interpolators (vorbis_fromdBlook / vorbis_invsqlook / vorbis_coslook family, manifest
// "Math_Interpolate*Lookup"); the exe carries "Xiph.Org libVorbis I 20030909". The library was
// linked with link-time code generation, hence the callee-pops `ret 8` on a static inline.
// Flags: /QIfist

extern "C" long __stdcall vorbis_ftoi(double f)
{
    return (long)f;
}
