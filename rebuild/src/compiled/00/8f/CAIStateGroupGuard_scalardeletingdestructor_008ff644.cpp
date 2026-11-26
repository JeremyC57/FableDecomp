// CAIStateGroup_Guard::`scalar deleting destructor'(unsigned int flags) -- the compiler-generated deleting destructor: the implicit ~CAIStateGroup_Guard
//   (which ends in ~CAIStateGroupBase 0x008FD288), then operator delete when bit 0 of flags is set; returns this
// Leaf: ??_GCAIStateGroup_Guard@@UAEPAXI@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x008FF644, 28 bytes, thiscall, `ret 4`; vftable 0x0128936C slot 0.
// Identity (NF469, r467a step 2): slot 0 of retail vftable 0x0128936C (RTTI .?AVCAIStateGroup_Guard@@); ??_G is VC7.1's scalar deleting destructor (no vector
//   form in retail, GOTCHAS NF424). Ego_r.pdb / FableWin.pdb type records flag ~CAIStateGroup_Guard `public virtual compiler-generated`.
//   Was ?CAIStateGroup_Guard_ScalarDeletingDestructor@@YIPAXPAXHI@Z (a free __fastcall stand-in).
// TU: VC7.1 emits ??_GCAIStateGroup_Guard@@UAEPAXI@Z as a pick-any COMDAT beside the vtable, only in a TU whose code stores the vptr. 0x008FD6E8
//   ??0CAIStateGroup_Guard@@QAE@XZ stores vftable 0x0128936C (NF467 r459a port, size-mode TU, r467c), so this function is produced by that translation unit,
//   included below with no code of its own (the nf251 0x00B2AB10 / Option S thin-row pattern, NF422 / nf424 ctor-owned units).
#include "../src/compiled/00/8f/CAIStateGroupGuard_CAIStateGroupGuard_008fd6e8.cpp"
