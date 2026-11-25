// Leaf: ??0C3DMeshFile@@AAE@ABV?$CCountedPointer@VC3DMeshFileRootChunk@@@@@Z
// C3DMeshFile::C3DMeshFile(const CCountedPointer<C3DMeshFileRootChunk>& root) -- chunked file over a typed root chunk (private)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00A9AE60, length 96 bytes, thiscall `ret 4`. Vtable 0x0129E0D8.
// Name: Ego_r.pdb 0x00C0BF60 (index 10) and FableWin.pdb 0x033D5540 (lib_3d_mesh_file.obj) spell the PRIVATE constructor (AAE); the loader C3DMeshFileLoader is a friend (it calls it).
// The derived-typed counted pointer is converted to the base-typed one CChunkedFile's constructor 0x00AA7DB0 takes (an 8-byte temporary,
//   count bumped), released after the base constructor, before this class's vtable is stored.
// NF422: was the owner of the two-member NF419 unit; now a thin row of the five-member unit owned by 0x00A9AD80.
// Option S unit (owner ruling 2026-11-24; NF419 b7, re-owned and widened by NF422): this function's one body lives in its retail TU-mate's
//   row 0x00A9AD80 (owner of the retail lib_3d_mesh_file.obj unit). The owner is included below with no code of its own; that object's
//   copy of this function grades against this address.
#include "../src/compiled/00/a9/C3DMeshFile_C3DMeshFile_00a9ad80.cpp"
