// CGameScriptInterfaceBase::`scalar deleting destructor'(unsigned int flags) -- the compiler-generated deleting destructor: ~CGameScriptInterfaceBase, then operator
//   delete when bit 0 of flags is set; returns this
// Leaf: ??_GCGameScriptInterfaceBase@@UAEPAXI@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x006E7180, 30 bytes, thiscall, `ret 4`; vftable 0x0126009C slot 0.
// Identity (NF424, S1 header port): retail RTTI .?AV of the vftable plus the FableWin / Ego_r name sets (round-1 histogram).
//   Was ?ScalarDeletingDestructor@CGameScriptInterfaceBase@@QAEPAXI@Z.
// TU: VC7.1 emits ??_GCGameScriptInterfaceBase@@UAEPAXI@Z as a pick-any COMDAT beside the vtable, only in a TU whose code stores the vptr. 0x006E7150
//   ??0CGameScriptInterfaceBase@@QAE@XZ stores vftable 0x0126009C, so this function is produced by that translation unit, included below with no code of its own
//   (the nf251 0x00B2AB10 / Option S thin-row pattern, NF422 ctor-owned units).
#include "../src/compiled/00/6e/CGameScriptInterfaceBase_CGameScriptInterfaceBase_006e7150.cpp"
