// CKeyboard::`scalar deleting destructor'(unsigned int flags) -- the compiler-generated deleting destructor: ~CKeyboard, then operator
//   delete when bit 0 of flags is set; returns this
// Leaf: ??_GCKeyboard@@UAEPAXI@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00A66AE0, 30 bytes, thiscall, `ret 4`; vftable 0x0129DBA8 slot 0.
// Identity (NF424, S2 G2): retail RTTI .?AV of the vftable plus the FableWin / Ego_r name sets (round-1 histogram).
//   Was ?CKeyboard_scalar_deleting_destructor@@YIPAXPAVCKeyboard@@HI@Z.
// TU: VC7.1 emits ??_GCKeyboard@@UAEPAXI@Z as a pick-any COMDAT beside the vtable, only in a TU whose code stores the vptr. 0x00A66930
//   ??0CKeyboard@@QAE@AAVCInputManager@@_N@Z stores vftable 0x0129DBA8, so this function is produced by that translation unit, included below with no code of its own
//   (the nf251 0x00B2AB10 / Option S thin-row pattern, NF422 ctor-owned units).
#include "../src/compiled/00/a6/CKeyboard_CKeyboard_00a66930.cpp"
