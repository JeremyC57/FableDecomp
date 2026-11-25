// CInputManager::`scalar deleting destructor'(unsigned int flags) -- the compiler-generated deleting destructor: ~CInputManager, then operator
//   delete when bit 0 of flags is set; returns this
// Leaf: ??_GCInputManager@@UAEPAXI@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x009F6FC0, 30 bytes, thiscall, `ret 4`; vftable 0x0129C79C slot 0.
// Identity (NF424, S2 G2): retail RTTI .?AV of the vftable plus the FableWin / Ego_r name sets (round-1 histogram).
//   Was ?CInputManager_scalar_deleting_destructor@@YIPAXPAVCInputManager@@HI@Z.
// TU: VC7.1 emits ??_GCInputManager@@UAEPAXI@Z as a pick-any COMDAT beside the vtable, only in a TU whose code stores the vptr. 0x009F5BD0
//   ??0CInputManager@@QAE@ABVCInputManagerInit@@_N@Z stores vftable 0x0129C79C, so this function is produced by that translation unit, included below with no code of its own
//   (the nf251 0x00B2AB10 / Option S thin-row pattern, NF422 ctor-owned units).
#include "../src/compiled/00/9f/CInputManager_CInputManager_009f5bd0.cpp"
