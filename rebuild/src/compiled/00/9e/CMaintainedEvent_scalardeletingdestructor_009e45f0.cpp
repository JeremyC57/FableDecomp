// CMaintainedEvent::`scalar deleting destructor'(unsigned int flags) -- the compiler-generated deleting destructor: ~CMaintainedEvent, then operator
//   delete when bit 0 of flags is set; returns this
// Leaf: ??_GCMaintainedEvent@@UAEPAXI@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x009E45F0, 40 bytes, thiscall, `ret 4`; vftable 0x0129C0E0 slot 0.
// Identity (NF424, S3 G3): retail RTTI .?AV of the vftable plus the FableWin / Ego_r name sets (round-1 histogram).
//   Was ?CMaintainedEvent_CompositeScalarDeletingDestructor@@YIPAXPAXHI@Z.
// TU: VC7.1 emits ??_GCMaintainedEvent@@UAEPAXI@Z as a pick-any COMDAT beside the vtable, only in a TU whose code stores the vptr. 0x009E45B0
//   ??0CMaintainedEvent@@QAE@XZ stores vftable 0x0129C0E0, so this function is produced by that translation unit, included below with no code of its own
//   (the nf251 0x00B2AB10 / Option S thin-row pattern, NF422 ctor-owned units).
#include "../src/compiled/00/9e/CMaintainedEvent_CMaintainedEvent_009e45b0.cpp"
