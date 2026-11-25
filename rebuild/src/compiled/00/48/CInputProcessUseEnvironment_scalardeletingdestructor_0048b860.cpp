// CInputProcessUseEnvironment::`scalar deleting destructor'(unsigned int flags) -- the compiler-generated deleting destructor: ~CInputProcessUseEnvironment, then operator
//   delete when bit 0 of flags is set; returns this
// Leaf: ??_GCInputProcessUseEnvironment@@UAEPAXI@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x0048B860, 30 bytes, thiscall, `ret 4`; vftable 0x01237A90 slot 0.
// Identity (NF424, S1 header port): retail RTTI .?AV of the vftable plus the FableWin / Ego_r name sets (round-1 histogram).
//   Was ?CInputProcessUseEnvironment_scalar_deleting_destructor@@YIPAXPAVCInputProcessUseEnvironment@@HI@Z.
// TU: VC7.1 emits ??_GCInputProcessUseEnvironment@@UAEPAXI@Z as a pick-any COMDAT beside the vtable, only in a TU whose code stores the vptr. 0x00488D10
//   ?InitInterfaces@CPlayer@@AAEXXZ stores vftable 0x01237A90, so this function is produced by that translation unit, included below with no code of its own
//   (the nf251 0x00B2AB10 / Option S thin-row pattern, NF422 ctor-owned units).
#include "../src/compiled/00/48/CPlayer_InitInterfaces_00488d10.cpp"
