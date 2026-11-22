// CWaterPatchMesh::`scalar deleting destructor'(unsigned int flags) -- the destructor (inlined), then the class operator delete (FreeVirtual 0x00BD9B90 through the landscape memory manager) on flag bit 0
// Leaf: ??_GCWaterPatchMesh@@UAEPAXI@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00BEB4D0, 100 bytes, thiscall, `ret 4`.
// Identity (NF272, 2026-11-22; NF269 NAME_FIXES): slot 0 of vtable 0x012A8134 (rebuild/manifest/rtti-vtables.tsv CWaterPatchMesh); Ego_r.pdb 00aa3780
//   ??_GCWaterPatchMesh@@UAEPAXI@Z; the body is ~CWaterPatchMesh 0x00BEAAA0's then FreeVirtual(this) on flag 1 (the class delete, FableWin.pdb
//   ??3CWaterPatchMesh@@SIXPAX@Z). Catalog already CWaterPatchMesh::`scalar deleting destructor'; the July row defined the stand-in
//   ?ScalarDeletingDestructor@CEngineScreenEffectOutlineGlow@@QAEPAV1@I@Z on candidates/outline_glow_compiled.h.
// TU: VC7.1 emits ??_GCWaterPatchMesh as a pick-any COMDAT beside the vtable, only in a TU whose code stores the vptr; retail's copy has the
//   destructor inlined, so it comes from the destructor's TU. The destructor has its own row (0x00BEAAA0), so this function is produced by that
//   translation unit, included below with no code of its own (the nf251 0x00B2AB10 pattern, as the EH funclet rows include their parent).
#include "../src/compiled/00/be/CWaterPatchMesh_CWaterPatchMesh_00beaaa0.cpp"
