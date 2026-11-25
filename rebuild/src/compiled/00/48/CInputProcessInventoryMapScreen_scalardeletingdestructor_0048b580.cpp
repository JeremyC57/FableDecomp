// CInputProcessInventoryMapScreen::`scalar deleting destructor'(unsigned int flags) -- the compiler-generated deleting destructor: ~CInputProcessInventoryMapScreen, then operator
//   delete when bit 0 of flags is set; returns this
// Leaf: ??_GCInputProcessInventoryMapScreen@@UAEPAXI@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x0048B580, 30 bytes, thiscall, `ret 4`; vftable 0x012377A8 slot 0.
// Identity (NF424, S1 header port): retail RTTI .?AV of the vftable plus the FableWin / Ego_r name sets (round-1 histogram).
//   Was ?ScalarDeletingDestructor@CInputProcessInventoryMapScreen@@QAEPAXI@Z.
// TU: VC7.1 emits ??_GCInputProcessInventoryMapScreen@@UAEPAXI@Z as a pick-any COMDAT beside the vtable, only in a TU whose code stores the vptr. 0x00488D10
//   ?InitInterfaces@CPlayer@@AAEXXZ stores vftable 0x012377A8, so this function is produced by that translation unit, included below with no code of its own
//   (the nf251 0x00B2AB10 / Option S thin-row pattern, NF422 ctor-owned units).
#include "../src/compiled/00/48/CPlayer_InitInterfaces_00488d10.cpp"
