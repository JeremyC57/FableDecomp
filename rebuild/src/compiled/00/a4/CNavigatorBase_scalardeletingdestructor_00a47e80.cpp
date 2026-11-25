// CNavigatorBase::`scalar deleting destructor'(unsigned int flags) -- the compiler-generated deleting destructor: ~CNavigatorBase, then operator
//   delete when bit 0 of flags is set; returns this
// Leaf: ??_GCNavigatorBase@@UAEPAXI@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00A47E80, 30 bytes, thiscall, `ret 4`; vftable 0x0129D4A4 slot 0.
// Identity (NF424, S2 G2): retail RTTI .?AV of the vftable plus the FableWin / Ego_r name sets (round-1 histogram).
//   Was ?ScalarDeletingDestructor@CNavigatorBase@@QAEPAXI@Z.
// TU: VC7.1 emits ??_GCNavigatorBase@@UAEPAXI@Z as a pick-any COMDAT beside the vtable, only in a TU whose code stores the vptr. 0x00A47180
//   ??1CNavigatorBase@@UAE@XZ stores vftable 0x0129D4A4, so this function is produced by that translation unit, included below with no code of its own
//   (the nf251 0x00B2AB10 / Option S thin-row pattern, NF422 ctor-owned units).
#include "../src/compiled/00/a4/CNavigatorBase_CNavigatorBase_00a47180.cpp"
