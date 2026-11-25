// CThingShotDef::`scalar deleting destructor'(unsigned int flags) -- the compiler-generated deleting destructor: ~CThingShotDef, then operator
//   delete when bit 0 of flags is set; returns this
// Leaf: ??_GCThingShotDef@@UAEPAXI@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x004590E6, 28 bytes, thiscall, `ret 4`; vftable 0x01234E2C slot 0.
// Identity (NF424, S1 header port): retail RTTI .?AV of the vftable plus the FableWin / Ego_r name sets (round-1 histogram).
//   Was ?CThingShotDef_ScalarDeletingDestructor@@YIPAXPAXHI@Z.
// TU: VC7.1 emits ??_GCThingShotDef@@UAEPAXI@Z as a pick-any COMDAT beside the vtable, only in a TU whose code stores the vptr. 0x00458D3F
//   ??0CThingShotDef@@QAE@XZ stores vftable 0x01234E2C, so this function is produced by that translation unit, included below with no code of its own
//   (the nf251 0x00B2AB10 / Option S thin-row pattern, NF422 ctor-owned units).
#include "../src/compiled/00/45/CThingShotDef_CThingShotDef_00458d3f.cpp"
