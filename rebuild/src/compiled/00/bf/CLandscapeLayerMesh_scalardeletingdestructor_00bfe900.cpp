// CLandscapeLayerMesh::scalar deleting destructor(flags) -- the destructor (called out of line, 0x00BFDE20), then the class operator delete (FreeVirtual 0x00BD9B90 through the landscape memory manager) on flag bit 0
// Leaf: ??_GCLandscapeLayerMesh@@UAEPAXI@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00BFE900, thiscall, `ret 4`.
// Identity: slot 0 of vtable 0x012A835C (rebuild/manifest/rtti-vtables.tsv .?AVCLandscapeLayerMesh@@); Ego_r.pdb ??_GCLandscapeLayerMesh@@UAEPAXI@Z;
//   the body calls ~CLandscapeLayerMesh 0x00BFDE20 (??1CLandscapeLayerMesh@@UAE@XZ, both PDBs), then on flag bit 0 the class delete
//   (FableWin.pdb ??3CLandscapeLayerMesh@@SIXPAX@Z, inline): FreeVirtual 0x00BD9B90 through NEngineInternal::GPLandscapeRenderer->MemoryManager.
// TU (NF277, 2026-11-22; nf276 ruling 1): VC7.1 emits ??_GCLandscapeLayerMesh as a pick-any COMDAT beside the vtable, only in a TU whose code
//   stores the vptr: the destructor's. The destructor has its own row (0x00BFDE20), so this function is produced by that translation unit,
//   included below with no code of its own (the nf251 0x00B2AB10 / nf272 0x00BEB4D0 fold pattern). This replaces the July row's local models
//   (its own 3-slot ??_7CLandscapeLayerMesh, a local CMovableResource whose Verify returned 0, and the ForceNew_00bfe900 helper).
#include "../src/compiled/00/bf/CLandscapeLayerMesh_CLandscapeLayerMesh_00bfde20.cpp"
