// CTCParamsBase::`scalar deleting destructor'(unsigned int flags) -- the compiler-generated deleting destructor: ~CTCParamsBase, then operator
//   delete when bit 0 of flags is set; returns this
// Leaf: ??_GCTCParamsBase@@UAEPAXI@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x0069EED0, 30 bytes, thiscall, `ret 4`; vftable 0x0125C810 slot 0.
// Identity (NF424, S1 local model): retail RTTI .?AV of the vftable plus the FableWin / Ego_r name sets (round-1 histogram).
//   Was ?ScalarDeletingDestructor@CTCParamsBase@@QAEPAXI@Z.
// TU: VC7.1 emits ??_GCTCParamsBase@@UAEPAXI@Z as a pick-any COMDAT beside the vtable, only in a TU whose code stores the vptr. 0x0069EE90
//   ??0CTCParamsBase@@QAE@XZ stores vftable 0x0125C810, so this function is produced by that translation unit, included below with no code of its own
//   (the nf251 0x00B2AB10 / Option S thin-row pattern, NF422 ctor-owned units).
#include "../src/compiled/00/69/CTCParamsBase_CTCParamsBase_0069ee90.cpp"
