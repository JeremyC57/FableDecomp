// libvorbis::icomp(const void* a, const void* b) -- libvorbis floor1.c: qsort comparator over int pointers (the post list sort of floor1_look)
// Leaf: _icomp
// Retail: Fable.exe (TLC, VC7.1) @ 0x00CA1BC0, 21 bytes (+11 int3). libvorbis lib/floor1.c static comparator
//   (libpk's statically linked Xiph libvorbis, "Xiph.Org libVorbis I 20030909").
// Convention: a qsort callback, so it keeps the plain `__cdecl` stack arguments (its address is taken at 0x00CA1C54,
//   floor1_look's `qsort(sortpointer,n,sizeof(*sortpointer),icomp)`) -- unlike the library's `__fastcall` API rows.
// Shape: the shipped source `return(**(int **)a-**(int **)b);`: both double dereferences load before the `sub ecx,edx`.
// Label: manifest "Int_Compare" is a descriptive placeholder; the real name is the floor1.c static `icomp`.
extern "C" int __cdecl icomp(const void* a, const void* b)
{
    return (**(int**)a - **(int**)b);
}