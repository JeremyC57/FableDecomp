// NScript::CQ_AwakeningTheOracleScript::`scalar deleting destructor'(unsigned int flags) -- the compiler-generated deleting destructor: ~CQ_AwakeningTheOracleScript, then operator
//   delete when bit 0 of flags is set; returns this
// Leaf: ??_GCQ_AwakeningTheOracleScript@NScript@@UAEPAXI@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00D005C0, 30 bytes, thiscall, `ret 4`; vftable 0x012C78B0 slot 0.
// Identity (NF424 round 2, S0): (1) the vftable's own slots (functions no other vftable references) carry landed / PDB names
//   of this class (V(1:00cfaeb0,2:00cfb080,3:00cfadd0,4:00cfaf70)); (2) PDB name sets P(G:fwin,E:egor,D:egor+fwin).
//   Was the shared stand-in ?CLandscapeBackgroundPatch_vector_deleting_destructor@@YIPAXPAVCLandscapeBackgroundPatch@@HI@Z.
// TU: VC7.1 emits ??_GCQ_AwakeningTheOracleScript@NScript@@UAEPAXI@Z as a pick-any COMDAT beside the vtable, only in a TU whose code stores the vptr. 0x00CFAD30
//   ??0CQ_AwakeningTheOracleScript@NScript@@QAE@PAVCScriptDataBase@@AAVCGameScriptInterfaceBase@@@Z stores vftable 0x012C78B0, so this function is produced by that translation unit, included below with no code of its own
//   (the nf251 0x00B2AB10 / Option S thin-row pattern, NF422 ctor-owned units).
#include "../src/compiled/00/cf/NScriptCQAwakeningTheOracleScript_CQAwakeningTheOracleScript_00cfad30.cpp"
