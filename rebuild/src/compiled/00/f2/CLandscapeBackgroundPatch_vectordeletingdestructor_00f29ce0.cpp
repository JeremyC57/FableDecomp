// CRegionScriptDef::`scalar deleting destructor'(unsigned int flags) -- the compiler-generated deleting destructor: ~CRegionScriptDef, then operator
//   delete when bit 0 of flags is set; returns this
// Leaf: ??_GCRegionScriptDef@@UAEPAXI@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00F29CE0, 30 bytes, thiscall, `ret 4`; vftable 0x012FB630 slot 0.
// Identity (NF424 round 2, S0): (1) the vftable's own slots (functions no other vftable references) carry landed / PDB names
//   of this class (V(18:00f29c60,19:00f29f60,20:00f29c50)); (2) PDB name sets P(G:egor,E:fwin,D:fwin).
//   Was the shared stand-in ?CLandscapeBackgroundPatch_vector_deleting_destructor@@YIPAXPAVCLandscapeBackgroundPatch@@HI@Z.
// TU: VC7.1 emits ??_GCRegionScriptDef@@UAEPAXI@Z as a pick-any COMDAT beside the vtable, only in a TU whose code stores the vptr. 0x00F29C10
//   ??0CRegionScriptDef@@QAE@XZ stores vftable 0x012FB630, so this function is produced by that translation unit, included below with no code of its own
//   (the nf251 0x00B2AB10 / Option S thin-row pattern, NF422 ctor-owned units).
#include "../src/compiled/00/f2/CRegionScriptDef_CRegionScriptDef_00f29c10.cpp"
