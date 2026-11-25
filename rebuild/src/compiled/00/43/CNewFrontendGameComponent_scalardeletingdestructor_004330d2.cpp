// CNewFrontendGameComponent::`scalar deleting destructor'(unsigned int flags) -- the compiler-generated deleting destructor: ~CNewFrontendGameComponent, then operator
//   delete when bit 0 of flags is set; returns this
// Leaf: ??_GCNewFrontendGameComponent@@UAEPAXI@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x004330D2, 28 bytes, thiscall, `ret 4`; vftable 0x01230CA0 slot 0.
// Identity (NF424, S2 G2): retail RTTI .?AV of the vftable plus the FableWin / Ego_r name sets (round-1 histogram).
//   Was ?ScalarDeletingDestructor@CNewFrontendGameComponent@@QAEPAXI@Z.
// TU: VC7.1 emits ??_GCNewFrontendGameComponent@@UAEPAXI@Z as a pick-any COMDAT beside the vtable, only in a TU whose code stores the vptr. 0x0042EA8F
//   ??0CNewFrontendGameComponent@@QAE@AAVCGame@@ABVCNewFrontendGameComponentInit@@@Z stores vftable 0x01230CA0, so this function is produced by that translation unit, included below with no code of its own
//   (the nf251 0x00B2AB10 / Option S thin-row pattern, NF422 ctor-owned units).
#include "../src/compiled/00/42/CNewFrontendGameComponent_CNewFrontendGameComponent_0042ea8f.cpp"
