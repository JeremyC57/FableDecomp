// CWADFile::`scalar deleting destructor'(unsigned int flags) -- the compiler-generated deleting destructor: ~CWADFile, then operator
//   delete when bit 0 of flags is set; returns this
// Leaf: ??_GCWADFile@@UAEPAXI@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00A39DF0, 30 bytes, thiscall, `ret 4`; vftable 0x0129CF8C slot 0.
// Identity (NF424, S1 header port): retail RTTI .?AV of the vftable plus the FableWin / Ego_r name sets (round-1 histogram).
//   Was ?CWADFile_scalar_deleting_destructor@@YIPAXPAVCWADFile@@HI@Z.
// TU: VC7.1 emits ??_GCWADFile@@UAEPAXI@Z as a pick-any COMDAT beside the vtable, only in a TU whose code stores the vptr. 0x00A39B40
//   ??0CWADFile@@QAE@PAVCBankFileAsync@@@Z stores vftable 0x0129CF8C, so this function is produced by that translation unit, included below with no code of its own
//   (the nf251 0x00B2AB10 / Option S thin-row pattern, NF422 ctor-owned units).
#include "../src/compiled/00/a3/CWADFile_CWADFile_00a39b40.cpp"
