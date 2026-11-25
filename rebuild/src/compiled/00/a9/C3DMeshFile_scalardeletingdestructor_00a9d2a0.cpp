// Leaf: ??_GC3DMeshFile@@UAEPAXI@Z
// C3DMeshFile::`scalar deleting destructor'(unsigned int flags) -- the implicit ~C3DMeshFile expanded to its one call,
//   CChunkedFile::~CChunkedFile 0x00AA7DF0 (no vtable store), then operator delete 0x00BFE9BC when flags & 1; returns this.
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00A9D2A0, length 30 bytes, thiscall `ret 4`. Vftable 0x0129E0D8 slot 0.
// Name: Ego_r.pdb 0x00C0E0D0 (index 43) and FableWin.pdb 0x033D54E0, both this public: proc "C3DMeshFile::`scalar deleting destructor'" in lib_3d_mesh_file.obj; the body tests only bit 0, so ??_G.
// Option S unit (owner ruling 2026-11-24; NF419 b7, re-owned and widened by NF422): this function's one body lives in its retail TU-mate's
//   row 0x00A9AD80 (owner of the retail lib_3d_mesh_file.obj unit). The owner is included below with no code of its own; that object's
//   copy of this function grades against this address.
#include "../src/compiled/00/a9/C3DMeshFile_C3DMeshFile_00a9ad80.cpp"
