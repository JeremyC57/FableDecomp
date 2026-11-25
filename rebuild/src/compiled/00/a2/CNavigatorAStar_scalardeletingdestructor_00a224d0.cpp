// CNavigatorAStar::`scalar deleting destructor'(unsigned int flags) -- the compiler-generated deleting destructor: ~CNavigatorAStar, then operator
//   delete when bit 0 of flags is set; returns this
// Leaf: ??_GCNavigatorAStar@@UAEPAXI@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00A224D0, 30 bytes, thiscall, `ret 4`; vftable 0x0129CCCC slot 0.
// Identity (NF424, S2 G2): retail RTTI .?AV of the vftable plus the FableWin / Ego_r name sets (round-1 histogram).
//   Was ?ScalarDeletingDestructor@CNavigatorAStar@@QAEPAXI@Z.
// TU: VC7.1 emits ??_GCNavigatorAStar@@UAEPAXI@Z as a pick-any COMDAT beside the vtable, only in a TU whose code stores the vptr. 0x00A1CB40
//   ??1CNavigatorAStar@@UAE@XZ stores vftable 0x0129CCCC, so this function is produced by that translation unit, included below with no code of its own
//   (the nf251 0x00B2AB10 / Option S thin-row pattern, NF422 ctor-owned units).
#include "../src/compiled/00/a1/CNavigatorAStar_CNavigatorAStar_00a1cb40.cpp"
