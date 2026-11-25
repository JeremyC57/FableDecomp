// CShaderBankEntry::`scalar deleting destructor'(unsigned int flags) -- the compiler-generated deleting destructor: ~CShaderBankEntry, then operator
//   delete when bit 0 of flags is set; returns this
// Leaf: ??_GCShaderBankEntry@@UAEPAXI@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00A628B0, 30 bytes, thiscall, `ret 4`; vftable 0x0129D698 slot 0.
// Identity (NF424, S2 G2): retail RTTI .?AV of the vftable plus the FableWin / Ego_r name sets (round-1 histogram).
//   Was ?CShaderBankEntry_scalar_deleting_destructor@@YIPAXPAVCShaderBankEntry@@HI@Z.
// TU: VC7.1 emits ??_GCShaderBankEntry@@UAEPAXI@Z as a pick-any COMDAT beside the vtable, only in a TU whose code stores the vptr. 0x00A62120
//   ??0CShaderBankEntry@@QAE@KKPAVCShaderDataBank@@@Z stores vftable 0x0129D698, so this function is produced by that translation unit, included below with no code of its own
//   (the nf251 0x00B2AB10 / Option S thin-row pattern, NF422 ctor-owned units).
#include "../src/compiled/00/a6/CShaderBankEntry_CShaderBankEntry_00a62120.cpp"
