// C3DPrimitive2::`scalar deleting destructor'(unsigned int flags) -- the compiler-generated deleting destructor: ~C3DPrimitive2, then operator
//   delete when bit 0 of flags is set; returns this
// Leaf: ??_GC3DPrimitive2@@UAEPAXI@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00A98430, 30 bytes, thiscall, `ret 4`; vftable 0x0129DF94 slot 0.
// Identity (NF424, S2 G2): retail RTTI .?AV of the vftable plus the FableWin / Ego_r name sets (round-1 histogram).
//   Was ?C3DPrimitive2_scalar_deleting_destructor@@YIPAXPAVC3DPrimitive2@@HI@Z.
// TU: VC7.1 emits ??_GC3DPrimitive2@@UAEPAXI@Z as a pick-any COMDAT beside the vtable, only in a TU whose code stores the vptr. 0x00A90790
//   ??1C3DPrimitive2@@UAE@XZ stores vftable 0x0129DF94, so this function is produced by that translation unit, included below with no code of its own
//   (the nf251 0x00B2AB10 / Option S thin-row pattern, NF422 ctor-owned units).
#include "../src/compiled/00/a9/C3DPrimitive2_C3DPrimitive2_00a90790.cpp"
