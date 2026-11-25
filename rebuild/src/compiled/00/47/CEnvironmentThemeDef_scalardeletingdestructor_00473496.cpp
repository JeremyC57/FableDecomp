// CEnvironmentThemeDef::`scalar deleting destructor'(unsigned int flags) -- the compiler-generated deleting destructor: ~CEnvironmentThemeDef, then operator
//   delete when bit 0 of flags is set; returns this
// Leaf: ??_GCEnvironmentThemeDef@@UAEPAXI@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00473496, 28 bytes, thiscall, `ret 4`; vftable 0x01236244 slot 0.
// Identity (NF424, S1 header port): retail RTTI .?AV of the vftable plus the FableWin / Ego_r name sets (round-1 histogram).
//   Was ?CEnvironmentThemeDef_ScalarDeletingDestructor@@YIPAXPAXHI@Z.
// TU: VC7.1 emits ??_GCEnvironmentThemeDef@@UAEPAXI@Z as a pick-any COMDAT beside the vtable, only in a TU whose code stores the vptr. 0x0047318D
//   ??0CEnvironmentThemeDef@@QAE@ABV0@@Z stores vftable 0x01236244, so this function is produced by that translation unit, included below with no code of its own
//   (the nf251 0x00B2AB10 / Option S thin-row pattern, NF422 ctor-owned units).
#include "../src/compiled/00/47/CEnvironmentThemeDef_CEnvironmentThemeDef_0047318d.cpp"
