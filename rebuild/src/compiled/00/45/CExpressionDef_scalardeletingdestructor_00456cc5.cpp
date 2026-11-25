// CExpressionDef::`scalar deleting destructor'(unsigned int flags) -- the compiler-generated deleting destructor: ~CExpressionDef, then operator
//   delete when bit 0 of flags is set; returns this
// Leaf: ??_GCExpressionDef@@UAEPAXI@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00456CC5, 28 bytes, thiscall, `ret 4`; vftable 0x01233D1C slot 0.
// Identity (NF424, S1 header port): retail RTTI .?AV of the vftable plus the FableWin / Ego_r name sets (round-1 histogram).
//   Was ?CExpressionDef_scalar_deleting_destructor@@YIPAXPAVCExpressionDef@@HI@Z.
// TU: VC7.1 emits ??_GCExpressionDef@@UAEPAXI@Z as a pick-any COMDAT beside the vtable, only in a TU whose code stores the vptr. 0x00456964
//   ??0CExpressionDef@@QAE@XZ stores vftable 0x01233D1C, so this function is produced by that translation unit, included below with no code of its own
//   (the nf251 0x00B2AB10 / Option S thin-row pattern, NF422 ctor-owned units).
#include "../src/compiled/00/45/CExpressionDef_CExpressionDef_00456964.cpp"
