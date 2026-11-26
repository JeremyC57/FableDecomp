// CInputProcessInventoryBase::`scalar deleting destructor'(unsigned int flags) -- the compiler-generated deleting destructor: ~CInputProcessInventoryBase, then operator
//   delete when bit 0 of flags is set; returns this
// Leaf: ??_GCInputProcessInventoryBase@@UAEPAXI@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x0048B4E0, 30 bytes, thiscall, `ret 4`; vftable 0x012376CC slot 0.
// Identity (NF477 thin-row port, nf424 pattern; NF424 S1 for the 60 siblings): retail RTTI .?AV of the vftable plus the FableWin / Ego_r name sets (round-1 histogram).
//   Was ?CInputProcessInventoryBase_scalar_deleting_destructor@@YIPAXPAUCInputProcessInventoryBase@@HI@Z.
// NF477 (2026-11-26): the misspelt free stand-in respelt to the compiler-generated ??_G; the owner TU's copy is byte- and relocation-equal to
//   retail at HEAD (ceq). TU: VC7.1 emits ??_GCInputProcessInventoryBase@@UAEPAXI@Z as a pick-any COMDAT beside the vtable, only in a TU whose code stores the vptr. 0x00488D10
//   ?InitInterfaces@CPlayer@@AAEXXZ emits vftable 0x012376CC (the inline CInputProcessInventoryBase constructor of the ten inventory screens
//   names it; the store itself is dead, each screen then stores its own vtable), so this function is produced by that translation unit, included below with no code of its own
//   (the nf251 0x00B2AB10 / Option S thin-row pattern, NF422 ctor-owned units).
#include "../src/compiled/00/48/CPlayer_InitInterfaces_00488d10.cpp"
