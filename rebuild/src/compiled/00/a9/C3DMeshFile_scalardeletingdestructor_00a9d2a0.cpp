// Leaf: ??_GC3DMeshFile@@UAEPAXI@Z
// C3DMeshFile::`scalar deleting destructor'(unsigned int flags) -- the implicit ~C3DMeshFile expanded to its one call,
//   CChunkedFile::~CChunkedFile 0x00AA7DF0 (no vtable store), then operator delete 0x00BFE9BC when flags & 1; returns this.
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00A9D2A0, length 30 bytes, thiscall `ret 4`. Vftable 0x0129E0D8 slot 0.
// Name: Ego_r.pdb 0x00C0E0D0 (lib_3d_mesh_file.obj index 43) and FableWin.pdb 0x033D54E0 (lib_3d_mesh_file.obj) both name it
//   ??_GC3DMeshFile@@UAEPAXI@Z, proc "C3DMeshFile::`scalar deleting destructor'"; the body tests only bit 0.
// Option S unit (owner ruling 2026-11-24; NF419): this function's one body lives in its retail TU-mate's row 0x00A9AE60. The owner is
//   included below with no code of its own, as the EH-funclet rows include their parent; that object's compiler-generated ??_G copy
//   grades against this address. Before the unit this row defined a free __fastcall stand-in
//   ?C3DMeshFile_scalar_deleting_destructor@@YIPAXPAVC3DMeshFile@@HI@Z that called a stand-in ?C3DMeshFile_dtor@@YIXPAVC3DMeshFile@@@Z.
#include "../src/compiled/00/a9/C3DMeshFile_C3DMeshFile_00a9ae60.cpp"
