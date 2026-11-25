// CCreatureAction_Curiosity::`scalar deleting destructor'(unsigned int flags) -- the compiler-generated deleting destructor: ~CCreatureAction_Curiosity, then operator
//   delete when bit 0 of flags is set; returns this
// Leaf: ??_GCCreatureAction_Curiosity@@UAEPAXI@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00911200, 30 bytes, thiscall, `ret 4`; vftable 0x0128F86C slot 0.
// Identity (NF424, S1 header port): retail RTTI .?AV of the vftable plus the FableWin / Ego_r name sets (round-1 histogram).
//   Was ?ScalarDeletingDestructor@CCreatureAction_Curiosity@@QAEPAXI@Z.
// TU: VC7.1 emits ??_GCCreatureAction_Curiosity@@UAEPAXI@Z as a pick-any COMDAT beside the vtable, only in a TU whose code stores the vptr. 0x00911180
//   ??0CCreatureAction_Curiosity@@QAE@AAVCThingCreatureBase@@@Z stores vftable 0x0128F86C, so this function is produced by that translation unit, included below with no code of its own
//   (the nf251 0x00B2AB10 / Option S thin-row pattern, NF422 ctor-owned units).
#include "../src/compiled/00/91/CCreatureActionCuriosity_CCreatureActionCuriosity_00911180.cpp"
