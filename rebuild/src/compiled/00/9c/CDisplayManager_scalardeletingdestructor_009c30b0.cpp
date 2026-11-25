// CDisplayManager::`scalar deleting destructor'(unsigned int flags) -- the compiler-generated deleting destructor: ~CDisplayManager, then operator
//   delete when bit 0 of flags is set; returns this
// Leaf: ??_GCDisplayManager@@UAEPAXI@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x009C30B0, 30 bytes, thiscall, `ret 4`; vftable 0x0129B3F8 slot 0.
// Identity (NF424, S2 G2): retail RTTI .?AV of the vftable plus the FableWin / Ego_r name sets (round-1 histogram).
//   Was ?ScalarDeletingDestructor@CDisplayManager@@QAEPAXI@Z.
// TU: VC7.1 emits ??_GCDisplayManager@@UAEPAXI@Z as a pick-any COMDAT beside the vtable, only in a TU whose code stores the vptr. 0x009C0880
//   ??0CDisplayManager@@QAE@AAVCSystemManager@@@Z stores vftable 0x0129B3F8, so this function is produced by that translation unit, included below with no code of its own
//   (the nf251 0x00B2AB10 / Option S thin-row pattern, NF422 ctor-owned units).
#include "../src/compiled/00/9c/CDisplayManager_CDisplayManager_009c0880.cpp"
