// CIDrawEnvironment::`scalar deleting destructor'(unsigned int flags) -- the compiler-generated deleting destructor: ~CIDrawEnvironment, then operator
//   delete when bit 0 of flags is set; returns this
// Leaf: ??_GCIDrawEnvironment@@UAEPAXI@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x006B2AD0, 30 bytes, thiscall, `ret 4`; vftable 0x0125D4E4 slot 0.
// Identity (NF424, S1 header port): retail RTTI .?AV of the vftable plus the FableWin / Ego_r name sets (round-1 histogram).
//   Was ?ScalarDeletingDestructor@CIDrawEnvironment@@QAEPAXI@Z.
// TU: VC7.1 emits ??_GCIDrawEnvironment@@UAEPAXI@Z as a pick-any COMDAT beside the vtable, only in a TU whose code stores the vptr. 0x006B2A70
//   ??0CIDrawEnvironment@@QAE@XZ stores vftable 0x0125D4E4, so this function is produced by that translation unit, included below with no code of its own
//   (the nf251 0x00B2AB10 / Option S thin-row pattern, NF422 ctor-owned units).
#include "../src/compiled/00/6b/CIDrawEnvironment_CIDrawEnvironment_006b2a70.cpp"
