// C3DMeshFileMaterialExtChunk::`scalar deleting destructor'(unsigned int flags) -- the compiler-generated deleting destructor: ~C3DMeshFileMaterialExtChunk, then operator
//   delete when bit 0 of flags is set; returns this
// Leaf: ??_GC3DMeshFileMaterialExtChunk@@UAEPAXI@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00AC1940, 30 bytes, thiscall, `ret 4`; vftable 0x0129EE90 slot 0.
// Identity (NF424, S1 header port): retail RTTI .?AV of the vftable plus the FableWin / Ego_r name sets (round-1 histogram).
//   Was ?C3DMeshFileMaterialExtChunk_scalar_deleting_destructor@@YIPAXPAVC3DMeshFileMaterialExtChunk@@HI@Z.
// TU: VC7.1 emits ??_GC3DMeshFileMaterialExtChunk@@UAEPAXI@Z as a pick-any COMDAT beside the vtable, only in a TU whose code stores the vptr. 0x00AC03B0
//   ?CreateNewInstance@C3DMeshFileMaterialExtChunk@@UBEPAVCChunkedFileChunk@@XZ stores vftable 0x0129EE90, so this function is produced by that translation unit, included below with no code of its own
//   (the nf251 0x00B2AB10 / Option S thin-row pattern, NF422 ctor-owned units).
#include "../src/compiled/00/ac/C3DMeshFileMaterialExtChunk_CreateNewInstance_00ac03b0.cpp"
