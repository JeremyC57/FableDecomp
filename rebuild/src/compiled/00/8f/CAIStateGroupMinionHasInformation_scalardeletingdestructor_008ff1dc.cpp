// CAIStateGroup_MinionHasInformation::`scalar deleting destructor'(unsigned int flags) -- the compiler-generated deleting destructor: the implicit ~CAIStateGroup_MinionHasInformation
//   (which ends in ~CAIStateGroupBase 0x008FD288), then operator delete when bit 0 of flags is set; returns this
// Leaf: ??_GCAIStateGroup_MinionHasInformation@@UAEPAXI@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x008FF1DC, 28 bytes, thiscall, `ret 4`; vftable 0x012890C8 slot 0.
// Identity (NF474, r467f unit): slot 0 of retail vftable 0x012890C8 (RTTI .?AVCAIStateGroup_MinionHasInformation@@); ??_G is VC7.1's scalar deleting destructor (no vector
//   form in retail, GOTCHAS NF424); retail has no out-of-line ~CAIStateGroup_MinionHasInformation, the implicit destructor is expanded here. Was ?CAIStateGroup_MinionHasInformation_ScalarDeletingDestructor@@YIPAXPAXHI@Z
//   (a free __fastcall stand-in).
// TU: VC7.1 emits ??_GCAIStateGroup_MinionHasInformation@@UAEPAXI@Z as a pick-any COMDAT beside the vtable, only in a TU whose code stores the vptr. 0x008FD532
//   ??0CAIStateGroup_MinionHasInformation@@QAE@XZ stores vftable 0x012890C8 (NF474 r467f port, size-mode TU, r467c), so this function is produced by that
//   translation unit, included below with no code of its own (the nf251 0x00B2AB10 / Option S thin-row pattern, NF422 / nf424 ctor-owned units).
#include "../src/compiled/00/8f/CAIStateGroupMinionHasInformation_CAIStateGroupMinionHasInformation_008fd532.cpp"
