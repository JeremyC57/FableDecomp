// CCountedPointer::~CCountedPointer() -- (CCountedPointer<const C3DMeshStats>) release one counted pointer to a mesh's statistics
// Leaf: ??1?$CCountedPointer@$$CBVC3DMeshStats@@@@QAE@XZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x004C24C0, 53 bytes. Callees: the count block's DeleteFunc (indirect), operator delete 0x00BFE9BC.
//
// The shared count block's RefCount drops by one; at zero its DeleteFunc runs on the block's Data and the block is
// freed. Data and then Info are cleared (the destructor's Data-first order, engine/CCountedPointer.h: retail emits one
// ~CCountedPointer COMDAT per T).
// Names (agentFAM10, 2026-11-12, queue land1011dl; was _global::CCountedPointer_Release_004c24c0 with the stand-in pointee
//   CUnprovenPointee_004c24c0): FableWin.pdb ??1?$CCountedPointer@$$CBVC3DMeshStats@@@@QAE@XZ; its only two callers 0x00B39390 /
//   0x00B393A8 (both in GetParentTransformAnimatedMesh 0x00B39080, which reloc_bindings already binds to this symbol) destroy the
//   CCountedPointer<const C3DMeshStats> that C3DMeshInfo::GetMeshStats 0x00A25530 returns (Ego_r.pdb + FableWin.pdb
//   ?GetMeshStats@C3DMeshInfo@@QBE?AV?$CCountedPointer@$$CBVC3DMeshStats@@@@XZ).
// Matching: `this` in ecx; DeleteFunc is __fastcall (Data in ecx); the block goes back through operator delete.
#include "engine/C3DMeshStats.h"
#include "engine/CCountedPointer.h"
template CCountedPointer<const C3DMeshStats>::~CCountedPointer();
