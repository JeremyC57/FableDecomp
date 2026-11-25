// CVertexShaderConstantLayout::`scalar deleting destructor'(unsigned int flags) -- the compiler-generated deleting destructor: ~CVertexShaderConstantLayout, then operator
//   delete when bit 0 of flags is set; returns this
// Leaf: ??_GCVertexShaderConstantLayout@@UAEPAXI@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x0098D990, 30 bytes, thiscall, `ret 4`; vftable 0x0129A06C slot 0.
// Identity (NF424, S1x identity): retail RTTI .?AV of the vftable plus the FableWin / Ego_r name sets (round-1 histogram).
//   Was ?ScalarDeletingDestructor@CVertexShaderConstantLayout@@QAEPAXI@Z.
// TU: VC7.1 emits ??_GCVertexShaderConstantLayout@@UAEPAXI@Z as a pick-any COMDAT beside the vtable, only in a TU whose code stores the vptr. 0x0098D490
//   ??0CVertexShaderConstantLayout@@QAE@XZ stores vftable 0x0129A06C, so this function is produced by that translation unit, included below with no code of its own
//   (the nf251 0x00B2AB10 / Option S thin-row pattern, NF422 ctor-owned units).
#include "../src/compiled/00/98/CVertexShaderConstantLayout_CVertexShaderConstantLayout_0098d490.cpp"
