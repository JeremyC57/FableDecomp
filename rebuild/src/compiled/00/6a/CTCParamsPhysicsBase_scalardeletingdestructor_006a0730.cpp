// CTCParamsPhysicsBase::`scalar deleting destructor'(unsigned int flags) -- the compiler-generated deleting destructor: ~CTCParamsPhysicsBase, then operator
//   delete when bit 0 of flags is set; returns this
// Leaf: ??_GCTCParamsPhysicsBase@@UAEPAXI@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x006A0730, 30 bytes, thiscall, `ret 4`; vftable 0x0125C818 slot 0.
// Identity (NF424, S1 header port): retail RTTI .?AV of the vftable plus the FableWin / Ego_r name sets (round-1 histogram).
//   Was ?CTCParamsPhysicsBase_scalar_deleting_destructor@@YIPAXPAVCTCParamsPhysicsBase@@HI@Z.
// TU: VC7.1 emits ??_GCTCParamsPhysicsBase@@UAEPAXI@Z as a pick-any COMDAT beside the vtable, only in a TU whose code stores the vptr. 0x0069F2B0
//   ?FinishAction@CCreatureAction_UseObject@@UAEXXZ stores vftable 0x0125C818, so this function is produced by that translation unit, included below with no code of its own
//   (the nf251 0x00B2AB10 / Option S thin-row pattern, NF422 ctor-owned units).
#include "../src/compiled/00/69/CCreatureActionUseObject_FinishAction_0069f2b0.cpp"
