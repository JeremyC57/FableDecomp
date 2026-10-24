// vorbis_ftoi(double f) -- libvorbis float-to-int rounding helper (os.h), current FPU rounding mode
// Leaf: vorbis_ftoi
// Flags: /O2 /Oy /W3
// Retail-asm: 0x00CA3B11 `fld qword ptr [esp+8]` + 0x00CA3B15 `fistp dword ptr [esp]` into a `push ecx` slot: the libvorbis os.h MSVC branch of vorbis_ftoi is exactly this `__asm` block; VC7.1 C++ `(long)f` calls _ftol2 and /QIfist emits `fistp qword` with an 8-byte slot, so the bare dword fistp is the original's inline asm.
// Retail 0x00CA3B10, 15 bytes: push ecx ; fld qword [esp+8] ; fistp dword [esp] ; mov eax,[esp] ; pop ecx ; ret 8.
// Callers 0x00CA3E6D / 0x00CA3EC3 / 0x00CA3F16 are libvorbis' table interpolators (vorbis_fromdBlook / vorbis_invsqlook /
// vorbis_coslook family); the exe carries "Xiph.Org libVorbis I 20030909". The library was linked with link-time code
// generation, hence the callee-pops `ret 8` (__stdcall here) on a static inline.
extern "C" long __stdcall vorbis_ftoi(double f)
{
    long i;
    __asm fld f
    __asm fistp i
    return i;
}
