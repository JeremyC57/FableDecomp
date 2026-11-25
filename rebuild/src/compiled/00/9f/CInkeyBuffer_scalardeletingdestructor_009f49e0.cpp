// CInkeyBuffer::`scalar deleting destructor'(unsigned int flags) -- the compiler-generated deleting destructor: ~CInkeyBuffer, then operator
//   delete when bit 0 of flags is set; returns this
// Leaf: ??_GCInkeyBuffer@@UAEPAXI@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x009F49E0, 30 bytes, thiscall, `ret 4`; vftable 0x0129C784 slot 0.
// Identity (NF424, S1 local model): retail RTTI .?AV of the vftable plus the FableWin / Ego_r name sets (round-1 histogram).
//   Was ?CInkeyBuffer_scalar_deleting_destructor@@YIPAXPAUCInkeyBuffer@@HI@Z.
// TU: VC7.1 emits ??_GCInkeyBuffer@@UAEPAXI@Z as a pick-any COMDAT beside the vtable, only in a TU whose code stores the vptr. 0x01224250
//   _$E1 stores vftable 0x0129C784, so this function is produced by that translation unit, included below with no code of its own
//   (the nf251 0x00B2AB10 / Option S thin-row pattern, NF422 ctor-owned units).
#include "../src/compiled/01/22/StaticInitialization_InitializeCUnprovenGlobal013CB2F8_01224250.cpp"
