// CShaderDirectionalLight::`scalar deleting destructor'(unsigned int flags) -- the compiler-generated deleting destructor: ~CShaderDirectionalLight, then operator
//   delete when bit 0 of flags is set; returns this
// Leaf: ??_GCShaderDirectionalLight@@UAEPAXI@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00B499C0, 30 bytes, thiscall, `ret 4`; vftable 0x012A221C slot 0.
// Identity (NF424, S1 header port): retail RTTI .?AV of the vftable plus the FableWin / Ego_r name sets (round-1 histogram).
//   Was ?CShaderDirectionalLight_scalar_deleting_destructor@@YIPAXPAUCShaderDirectionalLight@@HI@Z.
// TU: VC7.1 emits ??_GCShaderDirectionalLight@@UAEPAXI@Z as a pick-any COMDAT beside the vtable, only in a TU whose code stores the vptr. 0x00B49950
//   ??0CShaderDirectionalLight@@QAE@ABVC3DVector@@ABVCRGBFloatColour@@1@Z stores vftable 0x012A221C, so this function is produced by that translation unit, included below with no code of its own
//   (the nf251 0x00B2AB10 / Option S thin-row pattern, NF422 ctor-owned units).
#include "../src/compiled/00/b4/CShaderDirectionalLight_CShaderDirectionalLight_00b49950.cpp"
