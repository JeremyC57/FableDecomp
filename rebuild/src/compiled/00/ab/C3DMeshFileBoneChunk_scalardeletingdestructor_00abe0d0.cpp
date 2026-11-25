// C3DMeshFileBoneChunk::`scalar deleting destructor'(unsigned int flags) -- the compiler-generated deleting destructor: ~C3DMeshFileBoneChunk, then operator
//   delete when bit 0 of flags is set; returns this
// Leaf: ??_GC3DMeshFileBoneChunk@@UAEPAXI@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00ABE0D0, 40 bytes, thiscall, `ret 4`; vftable 0x0129EB9C slot 0.
// Identity (NF424, S3 G3): retail RTTI .?AV of the vftable plus the FableWin / Ego_r name sets (round-1 histogram).
//   Was ?C3DMeshFileBoneChunk_CompositeScalarDeletingDestructor@@YIPAXPAXHI@Z.
// TU: VC7.1 emits ??_GC3DMeshFileBoneChunk@@UAEPAXI@Z as a pick-any COMDAT beside the vtable, only in a TU whose code stores the vptr. 0x00AB9D50
//   ??0C3DMeshFileBoneChunk@@QAE@JABVCMatrix3x4@@@Z stores vftable 0x0129EB9C, so this function is produced by that translation unit, included below with no code of its own
//   (the nf251 0x00B2AB10 / Option S thin-row pattern, NF422 ctor-owned units).
#include "../src/compiled/00/ab/C3DMeshFileBoneChunk_C3DMeshFileBoneChunk_00ab9d50.cpp"
