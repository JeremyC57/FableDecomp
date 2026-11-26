// CAIStateGroup_MinionMotherConsoleChild::`scalar deleting destructor'(unsigned int flags) -- the compiler-generated deleting destructor: the implicit ~CAIStateGroup_MinionMotherConsoleChild
//   (which ends in ~CAIStateGroupBase 0x008FD288), then operator delete when bit 0 of flags is set; returns this
// Leaf: ??_GCAIStateGroup_MinionMotherConsoleChild@@UAEPAXI@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00900E02, 28 bytes, thiscall, `ret 4`; vftable 0x0128A3DC slot 0.
// Identity (NF469, r467a step 2): slot 0 of retail vftable 0x0128A3DC (RTTI .?AVCAIStateGroup_MinionMotherConsoleChild@@); ??_G is VC7.1's scalar deleting destructor (no vector
//   form in retail, GOTCHAS NF424). Ego_r.pdb / FableWin.pdb type records flag ~CAIStateGroup_MinionMotherConsoleChild `public virtual compiler-generated`.
//   Was ?CAIStateGroup_MinionMotherConsoleChild_ScalarDeletingDestructor@@YIPAXPAXHI@Z (a free __fastcall stand-in).
// TU: VC7.1 emits ??_GCAIStateGroup_MinionMotherConsoleChild@@UAEPAXI@Z as a pick-any COMDAT beside the vtable, only in a TU whose code stores the vptr. 0x008FE227
//   ??0CAIStateGroup_MinionMotherConsoleChild@@QAE@XZ stores vftable 0x0128A3DC (NF467 r459a port, size-mode TU, r467c), so this function is produced by that translation unit,
//   included below with no code of its own (the nf251 0x00B2AB10 / Option S thin-row pattern, NF422 / nf424 ctor-owned units).
#include "../src/compiled/00/8f/CAIStateGroupMinionMotherConsoleChild_CAIStateGroupMinionMotherConsoleChild_008fe227.cpp"
