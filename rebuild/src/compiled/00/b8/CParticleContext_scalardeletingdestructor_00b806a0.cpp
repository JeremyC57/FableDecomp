// CParticleContext::`scalar deleting destructor'(unsigned int flags) -- the compiler-generated deleting destructor: ~CParticleContext, then operator
//   delete when bit 0 of flags is set; returns this
// Leaf: ??_GCParticleContext@@UAEPAXI@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00B806A0, 30 bytes, thiscall, `ret 4`; vftable 0x012A38E0 slot 0.
// Identity (NF424, S1 header port): retail RTTI .?AV of the vftable plus the FableWin / Ego_r name sets (round-1 histogram).
//   Was ?CParticleContext_scalar_deleting_destructor@@YIPAXPAVCParticleContext@@HI@Z.
// TU: VC7.1 emits ??_GCParticleContext@@UAEPAXI@Z as a pick-any COMDAT beside the vtable, only in a TU whose code stores the vptr. 0x00B80660
//   ??0CParticleContext@@QAE@XZ stores vftable 0x012A38E0, so this function is produced by that translation unit, included below with no code of its own
//   (the nf251 0x00B2AB10 / Option S thin-row pattern, NF422 ctor-owned units).
#include "../src/compiled/00/b8/CParticleContext_CParticleContext_00b80660.cpp"
