// CCompressorZlib::`scalar deleting destructor'(unsigned int flags) -- the compiler-generated deleting destructor: ~CCompressorZlib, then operator
//   delete when bit 0 of flags is set; returns this
// Leaf: ??_GCCompressorZlib@@UAEPAXI@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x0052AE40, 30 bytes, thiscall, `ret 4`; vftable 0x01245974 slot 0.
// Identity (NF424, S2 G2): retail RTTI .?AV of the vftable plus the FableWin / Ego_r name sets (round-1 histogram).
//   Was ?ScalarDeletingDestructor@CCompressorZlib@@QAEPAXI@Z.
// TU: VC7.1 emits ??_GCCompressorZlib@@UAEPAXI@Z as a pick-any COMDAT beside the vtable, only in a TU whose code stores the vptr. 0x0052ADA0
//   ??0CCompressorZlib@@QAE@XZ stores vftable 0x01245974, so this function is produced by that translation unit, included below with no code of its own
//   (the nf251 0x00B2AB10 / Option S thin-row pattern, NF422 ctor-owned units).
#include "../src/compiled/00/52/CCompressorZlib_CCompressorZlib_0052ada0.cpp"
