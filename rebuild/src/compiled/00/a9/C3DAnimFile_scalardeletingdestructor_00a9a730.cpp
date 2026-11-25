// Leaf: ??_GC3DAnimFile@@UAEPAXI@Z
// C3DAnimFile::`scalar deleting destructor'(unsigned int flags) -- the implicit ~C3DAnimFile expanded to its one call,
//   CChunkedFile::~CChunkedFile 0x00AA7DF0 (no vtable store), then operator delete 0x00BFE9BC when flags & 1; returns this.
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00A9A730, length 30 bytes, thiscall `ret 4`. Vftable 0x0129E034 slot 0.
// Name: Ego_r.pdb 0x00C0E0D0 (index 36, public the identical-folded ??_GC3DMeshFile@@UAEPAXI@Z) and FableWin.pdb 0x03204C70 (public the ??_E alias): proc "C3DAnimFile::`scalar deleting destructor'" in lib_3d_anim_file.obj; the body tests only bit 0, so ??_G.
// Option S unit (owner ruling 2026-11-24; NF419 b7, re-owned and widened by NF422): this function's one body lives in its retail TU-mate's
//   row 0x00A98E60 (owner of the retail lib_3d_anim_file.obj unit). The owner is included below with no code of its own; that object's
//   copy of this function grades against this address.
#include "../src/compiled/00/a9/C3DAnimFile_C3DAnimFile_00a98e60.cpp"
