// CSoundThemeDef::`scalar deleting destructor'(unsigned int flags) -- the compiler-generated deleting destructor: ~CSoundThemeDef, then operator
//   delete when bit 0 of flags is set; returns this
// Leaf: ??_GCSoundThemeDef@@UAEPAXI@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00455ECA, 38 bytes, thiscall, `ret 4`; vftable 0x01233824 slot 0.
// Identity (NF424, S3 G3): retail RTTI .?AV of the vftable plus the FableWin / Ego_r name sets (round-1 histogram).
//   Was ?CSoundThemeDef_scalar_deleting_destructor@@YIPAXPAVCSoundThemeDef@@HI@Z.
// TU: VC7.1 emits ??_GCSoundThemeDef@@UAEPAXI@Z as a pick-any COMDAT beside the vtable, only in a TU whose code stores the vptr. 0x00455EAC
//   ??0CSoundThemeDef@@QAE@XZ stores vftable 0x01233824, so this function is produced by that translation unit, included below with no code of its own
//   (the nf251 0x00B2AB10 / Option S thin-row pattern, NF422 ctor-owned units).
#include "../src/compiled/00/45/CSoundThemeDef_CSoundThemeDef_00455eac.cpp"
