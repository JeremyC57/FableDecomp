// CCreatureAction_UndeadRiseUp::`scalar deleting destructor'(unsigned int flags) -- the compiler-generated deleting destructor: ~CCreatureAction_UndeadRiseUp, then operator
//   delete when bit 0 of flags is set; returns this
// Leaf: ??_GCCreatureAction_UndeadRiseUp@@UAEPAXI@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x0074DE10, 30 bytes, thiscall, `ret 4`; vftable 0x01268F44 slot 0.
// Identity (NF424, S1x identity): retail RTTI .?AV of the vftable plus the FableWin / Ego_r name sets (round-1 histogram).
//   Was ?CCreatureAction_UndeadRiseUp_scalar_deleting_destructor@@YIPAXPAVCCreatureAction_UndeadRiseUp@@HI@Z.
// TU: VC7.1 emits ??_GCCreatureAction_UndeadRiseUp@@UAEPAXI@Z as a pick-any COMDAT beside the vtable, only in a TU whose code stores the vptr. 0x0074ACE0
//   ?UpdateUndead@CTCCreatureGenerator@@IAEXXZ stores vftable 0x01268F44, so this function is produced by that translation unit, included below with no code of its own
//   (the nf251 0x00B2AB10 / Option S thin-row pattern, NF422 ctor-owned units).
#include "../src/compiled/00/74/CTCCreatureGenerator_UpdateUndead_0074ace0.cpp"
