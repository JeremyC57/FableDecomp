// CAIStateGroup_TestTestTest::`scalar deleting destructor'(unsigned int flags) -- the compiler-generated deleting destructor: the implicit ~CAIStateGroup_TestTestTest
//   (which ends in ~CAIStateGroupBase 0x008FD288), then operator delete when bit 0 of flags is set; returns this
// Leaf: ??_GCAIStateGroup_TestTestTest@@UAEPAXI@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00900ED0, 28 bytes, thiscall, `ret 4`; vftable 0x0128A4DC slot 0.
// Identity (NF474, r467f unit): slot 0 of retail vftable 0x0128A4DC (RTTI .?AVCAIStateGroup_TestTestTest@@); ??_G is VC7.1's scalar deleting destructor (no vector
//   form in retail, GOTCHAS NF424); retail has no out-of-line ~CAIStateGroup_TestTestTest, the implicit destructor is expanded here. Was ?CAIStateGroup_TestTestTest_ScalarDeletingDestructor@@YIPAXPAXHI@Z
//   (a free __fastcall stand-in).
// TU: VC7.1 emits ??_GCAIStateGroup_TestTestTest@@UAEPAXI@Z as a pick-any COMDAT beside the vtable, only in a TU whose code stores the vptr. 0x008FE29B
//   ??0CAIStateGroup_TestTestTest@@QAE@XZ stores vftable 0x0128A4DC (NF474 r467f port, size-mode TU, r467c), so this function is produced by that
//   translation unit, included below with no code of its own (the nf251 0x00B2AB10 / Option S thin-row pattern, NF422 / nf424 ctor-owned units).
#include "../src/compiled/00/8f/CAIStateGroupTestTestTest_CAIStateGroupTestTestTest_008fe29b.cpp"
