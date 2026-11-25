// CShaderPointLight::`scalar deleting destructor'(unsigned int flags) -- the compiler-generated deleting destructor: ~CShaderPointLight, then operator
//   delete when bit 0 of flags is set; returns this
// Leaf: ??_GCShaderPointLight@@UAEPAXI@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00B499E0, 30 bytes, thiscall, `ret 4`; vftable 0x012A2114 slot 0.
// Identity (NF424, S1 local model): retail RTTI .?AV of the vftable plus the FableWin / Ego_r name sets (round-1 histogram).
//   Was ?ScalarDeletingDestructor@CShaderPointLight@@QAEPAXI@Z.
// TU: VC7.1 emits ??_GCShaderPointLight@@UAEPAXI@Z as a pick-any COMDAT beside the vtable, only in a TU whose code stores the vptr. 0x00B49320
//   ??0CShaderPointLight@@QAE@ABVC3DVector@@ABVCRGBFloatColour@@MABVCShaderLightAttenuation@@@Z stores vftable 0x012A2114, so this function is produced by that translation unit, included below with no code of its own
//   (the nf251 0x00B2AB10 / Option S thin-row pattern, NF422 ctor-owned units).
#include "../src/compiled/00/b4/CShaderPointLight_CShaderPointLight_00b49320.cpp"
