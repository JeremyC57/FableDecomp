// CTextureManager::`scalar deleting destructor'(unsigned int flags) -- the compiler-generated deleting destructor: ~CTextureManager, then operator
//   delete when bit 0 of flags is set; returns this
// Leaf: ??_GCTextureManager@@UAEPAXI@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00A6C000, 30 bytes, thiscall, `ret 4`; vftable 0x0129DC5C slot 0.
// Identity (NF424, S2 G2): retail RTTI .?AV of the vftable plus the FableWin / Ego_r name sets (round-1 histogram).
//   Was ?CTextureManager_scalar_deleting_destructor@@YIPAXPAVCTextureManager@@HI@Z.
// TU: VC7.1 emits ??_GCTextureManager@@UAEPAXI@Z as a pick-any COMDAT beside the vtable, only in a TU whose code stores the vptr. 0x00A6A440
//   ??1CTextureManager@@UAE@XZ stores vftable 0x0129DC5C, so this function is produced by that translation unit, included below with no code of its own
//   (the nf251 0x00B2AB10 / Option S thin-row pattern, NF422 ctor-owned units).
#include "../src/compiled/00/a6/CTextureManager_CTextureManager_00a6a440.cpp"
