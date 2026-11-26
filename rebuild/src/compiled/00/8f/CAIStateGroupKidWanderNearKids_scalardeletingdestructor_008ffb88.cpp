// CAIStateGroup_KidWanderNearKids::`scalar deleting destructor'(unsigned int flags) -- the compiler-generated deleting destructor: the implicit ~CAIStateGroup_KidWanderNearKids
//   (which ends in ~CAIStateGroupBase 0x008FD288), then operator delete when bit 0 of flags is set; returns this
// Leaf: ??_GCAIStateGroup_KidWanderNearKids@@UAEPAXI@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x008FFB88, 28 bytes, thiscall, `ret 4`; vftable 0x01289798 slot 0.
// Identity (NF474, r467f unit): slot 0 of retail vftable 0x01289798 (RTTI .?AVCAIStateGroup_KidWanderNearKids@@); ??_G is VC7.1's scalar deleting destructor (no vector
//   form in retail, GOTCHAS NF424); retail has no out-of-line ~CAIStateGroup_KidWanderNearKids, the implicit destructor is expanded here. Was ?CAIStateGroup_KidWanderNearKids_ScalarDeletingDestructor@@YIPAXPAXHI@Z
//   (a free __fastcall stand-in).
// TU: VC7.1 emits ??_GCAIStateGroup_KidWanderNearKids@@UAEPAXI@Z as a pick-any COMDAT beside the vtable, only in a TU whose code stores the vptr. 0x008FD98C
//   ??0CAIStateGroup_KidWanderNearKids@@QAE@XZ stores vftable 0x01289798 (NF474 r467f port, size-mode TU, r467c), so this function is produced by that
//   translation unit, included below with no code of its own (the nf251 0x00B2AB10 / Option S thin-row pattern, NF422 / nf424 ctor-owned units).
#include "../src/compiled/00/8f/CAIStateGroupKidWanderNearKids_CAIStateGroupKidWanderNearKids_008fd98c.cpp"
