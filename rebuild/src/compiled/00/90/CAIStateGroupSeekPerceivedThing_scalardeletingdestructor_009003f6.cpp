// CAIStateGroup_SeekPerceivedThing::`scalar deleting destructor'(unsigned int flags) -- the compiler-generated deleting destructor: the implicit ~CAIStateGroup_SeekPerceivedThing
//   (which ends in ~CAIStateGroupBase 0x008FD288), then operator delete when bit 0 of flags is set; returns this
// Leaf: ??_GCAIStateGroup_SeekPerceivedThing@@UAEPAXI@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x009003F6, 28 bytes, thiscall, `ret 4`; vftable 0x01289D24 slot 0.
// Identity (NF474, r467f unit): slot 0 of retail vftable 0x01289D24 (RTTI .?AVCAIStateGroup_SeekPerceivedThing@@); ??_G is VC7.1's scalar deleting destructor (no vector
//   form in retail, GOTCHAS NF424); retail has no out-of-line ~CAIStateGroup_SeekPerceivedThing, the implicit destructor is expanded here. Was ?CAIStateGroup_SeekPerceivedThing_ScalarDeletingDestructor@@YIPAXPAXHI@Z
//   (a free __fastcall stand-in).
// TU: VC7.1 emits ??_GCAIStateGroup_SeekPerceivedThing@@UAEPAXI@Z as a pick-any COMDAT beside the vtable, only in a TU whose code stores the vptr. 0x008FDDE3
//   ??0CAIStateGroup_SeekPerceivedThing@@QAE@XZ stores vftable 0x01289D24 (NF474 r467f port, size-mode TU, r467c), so this function is produced by that
//   translation unit, included below with no code of its own (the nf251 0x00B2AB10 / Option S thin-row pattern, NF422 / nf424 ctor-owned units).
#include "../src/compiled/00/8f/CAIStateGroupSeekPerceivedThing_CAIStateGroupSeekPerceivedThing_008fdde3.cpp"
