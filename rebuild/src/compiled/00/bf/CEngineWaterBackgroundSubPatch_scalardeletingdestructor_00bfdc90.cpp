// CEngineWaterBackgroundSubPatch::`scalar deleting destructor'(unsigned int flags) -- the virtual destructor (called out of line, 0x00BFD480), then (bit 0) the class operator delete: the block goes back to the landscape memory manager (FreeVirtual 0x00BD9B90)
// Leaf: ??_GCEngineWaterBackgroundSubPatch@@UAEPAXI@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00BFDC90, 38 bytes, thiscall, `ret 4`, returns this.
// Identity (proven): slot 0 of vtable 0x012A8334 (rtti-vtables: CEngineWaterBackgroundSubPatch; RTTI bases CMovableResource); Ego_r.pdb
//   ??_GCEngineWaterBackgroundSubPatch@@UAEPAXI@Z (00a4c340); the destructor it calls, 0x00BFD480, is ??1CEngineWaterBackgroundSubPatch@@UAE@XZ
//   (both PDBs); the class delete is FableWin.pdb ??3CEngineWaterBackgroundSubPatch@@SIXPAX@Z (static __fastcall, inline in retail).
// TU (NF279, 2026-11-22; nf277 ruling 4): VC7.1 emits ??_GCEngineWaterBackgroundSubPatch as a pick-any COMDAT beside the vtable, only in a TU whose
//   code stores the vptr: the destructor's. The destructor has its own row (0x00BFD480), so this function is produced by that translation unit,
//   included below with no code of its own (the nf251 0x00B2AB10 / nf272 0x00BEB4D0 / nf277 0x00BFE900 fold pattern). This replaces the July
//   row's local models (a local CMovableResource with a 1-slot vtable, its own 1-slot ??_7CEngineWaterBackgroundSubPatch and the
//   Construct_00bfdc90 helper).
#include "../src/compiled/00/bf/CEngineWaterBackgroundSubPatch_CEngineWaterBackgroundSubPatch_00bfd480.cpp"
