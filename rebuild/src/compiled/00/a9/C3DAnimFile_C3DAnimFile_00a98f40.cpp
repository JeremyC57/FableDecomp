// Leaf: ??0C3DAnimFile@@AAE@ABV?$CCountedPointer@VC3DAnimFileRootChunk@@@@@Z
// C3DAnimFile::C3DAnimFile(const CCountedPointer<C3DAnimFileRootChunk>& root) -- chunked file over a typed root chunk (private)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00A98F40, length 96 bytes, thiscall `ret 4`. Vtable 0x0129E034.
// Name: Ego_r.pdb 0x00B980A0 (index 14) and FableWin.pdb 0x03204CD0 (lib_3d_anim_file.obj) spell the PRIVATE constructor (AAE); the loader C3DAnimFileLoader is a friend (it calls it).
// The derived-typed counted pointer is converted to the base-typed one CChunkedFile's constructor 0x00AA7DB0 takes (an 8-byte temporary,
//   count bumped), released after the base constructor, before this class's vtable is stored.
// NF422: was the owner of the two-member NF419 unit; now a thin row of the five-member unit owned by 0x00A98E60.
// Option S unit (owner ruling 2026-11-24; NF419 b7, re-owned and widened by NF422): this function's one body lives in its retail TU-mate's
//   row 0x00A98E60 (owner of the retail lib_3d_anim_file.obj unit). The owner is included below with no code of its own; that object's
//   copy of this function grades against this address.
#include "../src/compiled/00/a9/C3DAnimFile_C3DAnimFile_00a98e60.cpp"
