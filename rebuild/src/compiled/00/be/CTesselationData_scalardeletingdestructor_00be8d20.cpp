// CTesselationData::`scalar deleting destructor'(unsigned int flags) -- the compiler-generated deleting destructor: ~CTesselationData, then operator
//   delete when bit 0 of flags is set; returns this
// Leaf: ??_GCTesselationData@@UAEPAXI@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00BE8D20, 30 bytes, thiscall, `ret 4`; vftable 0x012A800C slot 0.
// Identity (NF424, S1 header port): retail RTTI .?AV of the vftable plus the FableWin / Ego_r name sets (round-1 histogram).
//   Was ?ScalarDeletingDestructor@CTesselationData@@QAEPAXI@Z.
// TU: VC7.1 emits ??_GCTesselationData@@UAEPAXI@Z as a pick-any COMDAT beside the vtable, only in a TU whose code stores the vptr. 0x00BE59C0
//   ??0CTesselationData@@QAE@PAVCEngineMap@@JJGG@Z stores vftable 0x012A800C, so this function is produced by that translation unit, included below with no code of its own
//   (the nf251 0x00B2AB10 / Option S thin-row pattern, NF422 ctor-owned units).
#include "../src/compiled/00/be/CTesselationData_CTesselationData_00be59c0.cpp"
