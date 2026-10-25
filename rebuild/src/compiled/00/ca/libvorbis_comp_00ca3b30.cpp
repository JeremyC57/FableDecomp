// libvorbis::comp(const void* a, const void* b) -- libvorbis lsp.c: qsort comparator sorting floats in descending order ((a<b)-(a>b))
// Leaf: _comp
// Retail: Fable.exe (TLC, VC7.1) @ 0x00CA3B30, 59 bytes. libvorbis lib/lsp.c static comparator (libpk's statically linked
//   Xiph libvorbis, "Xiph.Org libVorbis I 20030909"); a qsort callback, so plain `__cdecl` (its address is pushed at
//   0x00CA44E7 / 0x00CA44F5 by vorbis_lpc_to_lsp's two root sorts).
// Shape: the shipped source; each comparison reloads both floats (`fld [ecx] ; fcomp [edx]`), the first result lands in esi
//   and the second picks the `mov ecx,1` / `xor ecx,ecx` arm of the subtraction (two tail-duplicated exits).
// Label: manifest "Float_CompareStrict" is a descriptive placeholder; the real name is the lsp.c static `comp`.
extern "C" int __cdecl comp(const void* a, const void* b)
{
    return (*(float*)a < *(float*)b) - (*(float*)a > *(float*)b);
}