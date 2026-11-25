// CMeshBankStateBlock::`scalar deleting destructor'(unsigned int flags) -- the compiler-generated deleting destructor: ~CMeshBankStateBlock, then operator
//   delete when bit 0 of flags is set; returns this
// Leaf: ??_GCMeshBankStateBlock@@UAEPAXI@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00A2D770, 30 bytes, thiscall, `ret 4`; vftable 0x0129CE7C slot 0.
// Identity (NF424, S2 G2): retail RTTI .?AV of the vftable plus the FableWin / Ego_r name sets (round-1 histogram).
//   Was ?ScalarDeletingDestructor@CMeshBankStateBlock@@QAEPAXI@Z.
// TU: VC7.1 emits ??_GCMeshBankStateBlock@@UAEPAXI@Z as a pick-any COMDAT beside the vtable, only in a TU whose code stores the vptr. 0x00A2D650
//   ??0CMeshBankStateBlock@@QAE@XZ stores vftable 0x0129CE7C, so this function is produced by that translation unit, included below with no code of its own
//   (the nf251 0x00B2AB10 / Option S thin-row pattern, NF422 ctor-owned units).
#include "../src/compiled/00/a2/CMeshBankStateBlock_CMeshBankStateBlock_00a2d650.cpp"
