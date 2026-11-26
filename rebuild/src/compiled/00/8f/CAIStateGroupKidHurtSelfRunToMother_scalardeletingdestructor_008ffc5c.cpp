// CAIStateGroup_KidHurtSelfRunToMother::`scalar deleting destructor'(unsigned int flags) -- the compiler-generated deleting destructor: the implicit ~CAIStateGroup_KidHurtSelfRunToMother
//   (which ends in ~CAIStateGroupBase 0x008FD288), then operator delete when bit 0 of flags is set; returns this
// Leaf: ??_GCAIStateGroup_KidHurtSelfRunToMother@@UAEPAXI@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x008FFC5C, 28 bytes, thiscall, `ret 4`; vftable 0x01289824 slot 0.
// Identity (NF474, r467f unit): slot 0 of retail vftable 0x01289824 (RTTI .?AVCAIStateGroup_KidHurtSelfRunToMother@@); ??_G is VC7.1's scalar deleting destructor (no vector
//   form in retail, GOTCHAS NF424); retail has no out-of-line ~CAIStateGroup_KidHurtSelfRunToMother, the implicit destructor is expanded here. Was ?CAIStateGroup_KidHurtSelfRunToMother_ScalarDeletingDestructor@@YIPAXPAXHI@Z
//   (a free __fastcall stand-in).
// TU: VC7.1 emits ??_GCAIStateGroup_KidHurtSelfRunToMother@@UAEPAXI@Z as a pick-any COMDAT beside the vtable, only in a TU whose code stores the vptr. 0x008FD9DA
//   ??0CAIStateGroup_KidHurtSelfRunToMother@@QAE@XZ stores vftable 0x01289824 (NF474 r467f port, size-mode TU, r467c), so this function is produced by that
//   translation unit, included below with no code of its own (the nf251 0x00B2AB10 / Option S thin-row pattern, NF422 / nf424 ctor-owned units).
#include "../src/compiled/00/8f/CAIStateGroupKidHurtSelfRunToMother_CAIStateGroupKidHurtSelfRunToMother_008fd9da.cpp"
