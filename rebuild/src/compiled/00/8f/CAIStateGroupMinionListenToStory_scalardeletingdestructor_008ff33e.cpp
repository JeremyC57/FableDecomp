// CAIStateGroup_MinionListenToStory::`scalar deleting destructor'(unsigned int flags) -- the compiler-generated deleting destructor: the implicit ~CAIStateGroup_MinionListenToStory
//   (which ends in ~CAIStateGroupBase 0x008FD288), then operator delete when bit 0 of flags is set; returns this
// Leaf: ??_GCAIStateGroup_MinionListenToStory@@UAEPAXI@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x008FF33E, 28 bytes, thiscall, `ret 4`; vftable 0x01289180 slot 0.
// Identity (NF474, r467f unit): slot 0 of retail vftable 0x01289180 (RTTI .?AVCAIStateGroup_MinionListenToStory@@); ??_G is VC7.1's scalar deleting destructor (no vector
//   form in retail, GOTCHAS NF424); retail has no out-of-line ~CAIStateGroup_MinionListenToStory, the implicit destructor is expanded here. Was ?CAIStateGroup_MinionListenToStory_ScalarDeletingDestructor@@YIPAXPAXHI@Z
//   (a free __fastcall stand-in).
// TU: VC7.1 emits ??_GCAIStateGroup_MinionListenToStory@@UAEPAXI@Z as a pick-any COMDAT beside the vtable, only in a TU whose code stores the vptr. 0x008FD5CE
//   ??0CAIStateGroup_MinionListenToStory@@QAE@XZ stores vftable 0x01289180 (NF474 r467f port, size-mode TU, r467c), so this function is produced by that
//   translation unit, included below with no code of its own (the nf251 0x00B2AB10 / Option S thin-row pattern, NF422 / nf424 ctor-owned units).
#include "../src/compiled/00/8f/CAIStateGroupMinionListenToStory_CAIStateGroupMinionListenToStory_008fd5ce.cpp"
