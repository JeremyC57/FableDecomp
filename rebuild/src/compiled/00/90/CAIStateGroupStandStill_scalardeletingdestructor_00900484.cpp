// CAIStateGroup_StandStill::`scalar deleting destructor'(unsigned int flags) -- the compiler-generated deleting destructor: the implicit ~CAIStateGroup_StandStill
//   (which ends in ~CAIStateGroupBase 0x008FD288), then operator delete when bit 0 of flags is set; returns this
// Leaf: ??_GCAIStateGroup_StandStill@@UAEPAXI@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00900484, 28 bytes, thiscall, `ret 4`; vftable 0x01289D88 slot 0.
// Identity (NF469, r467a step 2): slot 0 of retail vftable 0x01289D88 (RTTI .?AVCAIStateGroup_StandStill@@); ??_G is VC7.1's scalar deleting destructor (no vector
//   form in retail, GOTCHAS NF424). Ego_r.pdb / FableWin.pdb type records flag ~CAIStateGroup_StandStill `public virtual compiler-generated`.
//   Was ?CAIStateGroup_StandStill_ScalarDeletingDestructor@@YIPAXPAXHI@Z (a free __fastcall stand-in).
// TU: VC7.1 emits ??_GCAIStateGroup_StandStill@@UAEPAXI@Z as a pick-any COMDAT beside the vtable, only in a TU whose code stores the vptr. 0x008FDE1B
//   ??0CAIStateGroup_StandStill@@QAE@XZ stores vftable 0x01289D88 (NF467 r459a port, size-mode TU, r467c), so this function is produced by that translation unit,
//   included below with no code of its own (the nf251 0x00B2AB10 / Option S thin-row pattern, NF422 / nf424 ctor-owned units).
#include "../src/compiled/00/8f/CAIStateGroupStandStill_CAIStateGroupStandStill_008fde1b.cpp"
