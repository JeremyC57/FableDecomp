// CEnginePrimitiveManagerDecal::`scalar deleting destructor'(unsigned int flags) -- the compiler-generated deleting destructor: ~CEnginePrimitiveManagerDecal, then operator
//   delete when bit 0 of flags is set; returns this
// Leaf: ??_GCEnginePrimitiveManagerDecal@@UAEPAXI@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00BBBBA0, 30 bytes, thiscall, `ret 4`; vftable 0x012A5B6C slot 0.
// Identity (NF424, S1x identity): retail RTTI .?AV of the vftable plus the FableWin / Ego_r name sets (round-1 histogram).
//   Was ?CEnginePrimitiveManagerDecal_scalar_deleting_destructor@@YIPAXPAVCEnginePrimitiveManagerDecal@@HI@Z.
// TU: VC7.1 emits ??_GCEnginePrimitiveManagerDecal@@UAEPAXI@Z as a pick-any COMDAT beside the vtable, only in a TU whose code stores the vptr. 0x00BBA480
//   ??0CEnginePrimitiveManagerDecal@@QAE@AAV?$CArray@PAVCEngineScene@@@@@Z stores vftable 0x012A5B6C, so this function is produced by that translation unit, included below with no code of its own
//   (the nf251 0x00B2AB10 / Option S thin-row pattern, NF422 ctor-owned units).
#include "../src/compiled/00/bb/CEnginePrimitiveManagerDecal_CEnginePrimitiveManagerDecal_00bba480.cpp"
