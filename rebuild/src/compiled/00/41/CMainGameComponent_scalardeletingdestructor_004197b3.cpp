// CMainGameComponent::`scalar deleting destructor'(unsigned int) -- run the destructor, then free the object when flag bit 0 is set
// Leaf: ??_GCMainGameComponent@@UAEPAXI@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x004197B3, 28 bytes, thiscall `ret 4` (size-optimised: `pop ecx` after operator delete); slot 0 of
//   vtable 0x0122F180, its only reference (no E8 / E9 caller).
// Identity (NF278, 2026-11-22): Ego_r.pdb 0x00418362 + FableWin.pdb 0x018A1307 ??_GCMainGameComponent@@UAEPAXI@Z; callees
//   CMainGameComponent::~CMainGameComponent 0x00418B7A (??1CMainGameComponent@@UAE@XZ, its only caller) and operator delete 0x00BFE9BC.
//   Both landed vtable emitters (0x00418DCA ctor, 0x00418B7A dtor) bind slot 0 as ??_ECMainGameComponent@@UAEPAXI@Z, the weak alias VC7.1
//   emits for this ??_G, and define this ??_G beside it.
// VC7.1 emits ??_GCMainGameComponent as a pick-any COMDAT beside the class's vtables, only in a TU whose code stores the vptr (the
// constructor or destructor), and both have their own rows. So this function is produced by the destructor's translation unit
// (0x00418B7A, whose only caller is this function), included below with no code of its own, as 0x00B2AB10 ??_GCEngine is folded under 0x00B260B0
// (landing nf251 ruling 1). The destructor's TU, not the constructor's: every pick-any copy the dtor object defines is EQ to its owner
// or has none, while the ctor object carries one NEQ copy (~CCountedPointer<CGraphicDataBank> v 0x004194B5, 44 v 44: NF275 debt) that a
// fold there would define a second time. Was a row-local CMainGameComponent model defining the stand-in
// ?ScalarDeletingDestructor@CMainGameComponent@@QAEPAXI@Z and binding the destructor as ?Dtor@CMainGameComponent@@QAEXXZ.
#include "../src/compiled/00/41/CMainGameComponent_CMainGameComponent_00418b7a.cpp"
