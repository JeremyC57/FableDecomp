// CAFile::`scalar deleting destructor'(unsigned int flags) -- the compiler-generated deleting destructor: ~CAFile, then operator
//   delete when bit 0 of flags is set; returns this
// Leaf: ??_GCAFile@@UAEPAXI@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x004192B0, 30 bytes, thiscall, `ret 4`; vftable 0x0122ECCC slot 0.
// Identity (NF424, S1 header port): retail RTTI .?AV of the vftable plus the FableWin / Ego_r name sets (round-1 histogram).
//   Was ?ScalarDeletingDestructor@CAFile@@QAEPAXI@Z.
// TU: VC7.1 emits ??_GCAFile@@UAEPAXI@Z as a pick-any COMDAT beside the vtable, only in a TU whose code stores the vptr. 0x00415C10
//   ??0CAFile@@QAE@XZ stores vftable 0x0122ECCC, so this function is produced by that translation unit, included below with no code of its own
//   (the nf251 0x00B2AB10 / Option S thin-row pattern, NF422 ctor-owned units).
#include "../src/compiled/00/41/CAFile_CAFile_00415c10.cpp"
