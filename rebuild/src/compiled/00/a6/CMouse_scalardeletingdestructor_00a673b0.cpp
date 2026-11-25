// CMouse::`scalar deleting destructor'(unsigned int flags) -- the compiler-generated deleting destructor: ~CMouse, then operator
//   delete when bit 0 of flags is set; returns this
// Leaf: ??_GCMouse@@UAEPAXI@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00A673B0, 30 bytes, thiscall, `ret 4`; vftable 0x0129DBC4 slot 0.
// Identity (NF424, S2 G2): retail RTTI .?AV of the vftable plus the FableWin / Ego_r name sets (round-1 histogram).
//   Was ?ScalarDeletingDestructor@CMouse@@QAEPAXI@Z.
// TU: VC7.1 emits ??_GCMouse@@UAEPAXI@Z as a pick-any COMDAT beside the vtable, only in a TU whose code stores the vptr. 0x00A66F20
//   ??0CMouse@@QAE@AAVCInputManager@@_N@Z stores vftable 0x0129DBC4, so this function is produced by that translation unit, included below with no code of its own
//   (the nf251 0x00B2AB10 / Option S thin-row pattern, NF422 ctor-owned units).
#include "../src/compiled/00/a6/CMouse_CMouse_00a66f20.cpp"
