// CEnginePrimitiveRenderer::`scalar deleting destructor'(unsigned int flags) -- the compiler-generated deleting destructor: ~CEnginePrimitiveRenderer, then operator
//   delete when bit 0 of flags is set; returns this
// Leaf: ??_GCEnginePrimitiveRenderer@@UAEPAXI@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00B38D10, 30 bytes, thiscall, `ret 4`; vftable 0x012A1380 slot 0.
// Identity (NF424, S2 G2): retail RTTI .?AV of the vftable plus the FableWin / Ego_r name sets (round-1 histogram).
//   Was ?Override_00b38d10@CMulti_00b38d10@@UAEXXZ.
// TU: VC7.1 emits ??_GCEnginePrimitiveRenderer@@UAEPAXI@Z as a pick-any COMDAT beside the vtable, only in a TU whose code stores the vptr. 0x00B33B50
//   ??0CEnginePrimitiveRenderer@@QAE@XZ stores vftable 0x012A1380, so this function is produced by that translation unit, included below with no code of its own
//   (the nf251 0x00B2AB10 / Option S thin-row pattern, NF422 ctor-owned units).
#include "../src/compiled/00/b3/CEnginePrimitiveRenderer_CEnginePrimitiveRenderer_00b33b50.cpp"
