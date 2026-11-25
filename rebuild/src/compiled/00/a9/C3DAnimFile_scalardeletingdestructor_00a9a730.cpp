// Leaf: ??_GC3DAnimFile@@UAEPAXI@Z
// C3DAnimFile::`scalar deleting destructor'(unsigned int flags) -- the implicit ~C3DAnimFile expanded to its one call,
//   CChunkedFile::~CChunkedFile 0x00AA7DF0 (no vtable store), then operator delete 0x00BFE9BC when flags & 1; returns this.
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00A9A730, length 30 bytes, thiscall `ret 4`. Vftable 0x0129E034 slot 0.
// Name: both PDBs have this body as the proc "C3DAnimFile::`scalar deleting destructor'" in lib_3d_anim_file.obj (Ego_r.pdb index 36,
//   0x00C0E0D0, whose public is the identical-folded ??_GC3DMeshFile@@UAEPAXI@Z; FableWin.pdb 0x03204C70, public the ??_E alias
//   ??_EC3DAnimFile@@UAEPAXI@Z). `scalar deleting destructor' is ??_G, and the body tests only bit 0 (no vector path), so the spelling
//   is ??_GC3DAnimFile@@UAEPAXI@Z (the vtable slot names ??_E, the weak alias VC7.1 defaults to ??_G).
// Option S unit (owner ruling 2026-11-24; NF419): this function's one body lives in its retail TU-mate's row 0x00A98F40. The owner is
//   included below with no code of its own, as the EH-funclet rows include their parent; that object's compiler-generated ??_G copy
//   grades against this address. Before the unit this row defined a free stand-in ?ScalarDeletingDestructor@C3DAnimFile@@QAEPAXI@Z
//   that called a stand-in ?Dtor@C3DAnimFile@@QAEXXZ.
#include "../src/compiled/00/a9/C3DAnimFile_C3DAnimFile_00a98f40.cpp"
