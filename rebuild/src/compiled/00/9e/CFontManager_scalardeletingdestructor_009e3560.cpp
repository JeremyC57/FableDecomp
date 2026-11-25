// CFontManager::`scalar deleting destructor'(unsigned int flags) -- the compiler-generated deleting destructor: ~CFontManager, then operator
//   delete when bit 0 of flags is set; returns this
// Leaf: ??_GCFontManager@@UAEPAXI@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x009E3560, 30 bytes, thiscall, `ret 4`; vftable 0x0129BA38 slot 0.
// Identity (NF424, S2 G2): retail RTTI .?AV of the vftable plus the FableWin / Ego_r name sets (round-1 histogram).
//   Was ?CFontManager_scalar_deleting_destructor@@YIPAXPAVCFontManager@@HI@Z.
// TU: VC7.1 emits ??_GCFontManager@@UAEPAXI@Z as a pick-any COMDAT beside the vtable, only in a TU whose code stores the vptr. 0x009E2E30
//   ??0CFontManager@@QAE@ABVCCharString@@0@Z stores vftable 0x0129BA38, so this function is produced by that translation unit, included below with no code of its own
//   (the nf251 0x00B2AB10 / Option S thin-row pattern, NF422 ctor-owned units).
#include "../src/compiled/00/9e/CFontManager_CFontManager_009e2e30.cpp"
