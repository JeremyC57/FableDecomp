// CArenaCreatureDef::`scalar deleting destructor'(unsigned int flags) -- the compiler-generated deleting destructor: ~CArenaCreatureDef, then operator
//   delete when bit 0 of flags is set; returns this
// Leaf: ??_GCArenaCreatureDef@@UAEPAXI@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00F25E20, 30 bytes, thiscall, `ret 4`; vftable 0x012FB358 slot 0.
// Identity (NF424 round 2, S0): (1) the vftable's own slots (functions no other vftable references) carry landed / PDB names
//   of this class (V(18:00f25e70)); (2) PDB name sets P(E:egor+fwin,D:egor+fwin).
//   Was the shared stand-in ?CLandscapeBackgroundPatch_vector_deleting_destructor@@YIPAXPAVCLandscapeBackgroundPatch@@HI@Z.
// TU: VC7.1 emits ??_GCArenaCreatureDef@@UAEPAXI@Z as a pick-any COMDAT beside the vtable, only in a TU whose code stores the vptr. 0x00F25DD0
//   ??0CArenaCreatureDef@@QAE@ABV0@@Z stores vftable 0x012FB358, so this function is produced by that translation unit, included below with no code of its own
//   (the nf251 0x00B2AB10 / Option S thin-row pattern, NF422 ctor-owned units).
#include "../src/compiled/00/f2/CArenaCreatureDef_CArenaCreatureDef_00f25dd0.cpp"
