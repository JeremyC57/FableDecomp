// CCreatureAction_SummonerStrike::`scalar deleting destructor'(unsigned int flags) -- the compiler-generated deleting destructor: ~CCreatureAction_SummonerStrike, then operator
//   delete when bit 0 of flags is set; returns this
// Leaf: ??_GCCreatureAction_SummonerStrike@@UAEPAXI@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x006F81D0, 30 bytes, thiscall, `ret 4`; vftable 0x0126435C slot 0.
// Identity (NF424, S1 header port): retail RTTI .?AV of the vftable plus the FableWin / Ego_r name sets (round-1 histogram).
//   Was ?CCreatureAction_SummonerStrike_scalar_deleting_destructor@@YIPAXPAVCCreatureAction_SummonerStrike@@HI@Z.
// TU: VC7.1 emits ??_GCCreatureAction_SummonerStrike@@UAEPAXI@Z as a pick-any COMDAT beside the vtable, only in a TU whose code stores the vptr. 0x006F5C10
//   ?StatRegisterClass@CCreatureActionBase@@SIXXZ stores vftable 0x0126435C, so this function is produced by that translation unit, included below with no code of its own
//   (the nf251 0x00B2AB10 / Option S thin-row pattern, NF422 ctor-owned units).
#include "../src/compiled/00/6f/CCreatureActionBase_StatRegisterClass_006f5c10.cpp"
