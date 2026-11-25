// CEngineWeatherRenderer::`scalar deleting destructor'(unsigned int flags) -- the compiler-generated deleting destructor: ~CEngineWeatherRenderer, then operator
//   delete when bit 0 of flags is set; returns this
// Leaf: ??_GCEngineWeatherRenderer@@UAEPAXI@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00B56690, 30 bytes, thiscall, `ret 4`; vftable 0x012A2434 slot 0.
// Identity (NF424, S2 G2): retail RTTI .?AV of the vftable plus the FableWin / Ego_r name sets (round-1 histogram).
//   Was ?ScalarDeletingDestructor@CEngineWeatherRenderer@@QAEPAXI@Z.
// TU: VC7.1 emits ??_GCEngineWeatherRenderer@@UAEPAXI@Z as a pick-any COMDAT beside the vtable, only in a TU whose code stores the vptr. 0x00B52250
//   ??0CEngineWeatherRenderer@@QAE@XZ stores vftable 0x012A2434, so this function is produced by that translation unit, included below with no code of its own
//   (the nf251 0x00B2AB10 / Option S thin-row pattern, NF422 ctor-owned units).
#include "../src/compiled/00/b5/CEngineWeatherRenderer_CEngineWeatherRenderer_00b52250.cpp"
