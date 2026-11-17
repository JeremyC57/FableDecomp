// CCountedPointer<const C3DMesh2>::~CCountedPointer() -- release one counted pointer to a mesh
// Leaf: ??1?$CCountedPointer@$$CBVC3DMesh2@@@@QAE@XZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00A36EC0, 53 bytes. Callees: the count block's DeleteFunc (indirect), operator delete 0x00BFE9BC.
//
// The shared count block's RefCount drops by one; at zero its DeleteFunc runs on the block's Data and the block is
// freed. Data and then Info are cleared (the destructor's Data-first order, engine/CCountedPointer.h: retail emits one
// ~CCountedPointer COMDAT per T). The catalog's "Release" placeholder and the earlier _Dest_val label name this body; the
// order of the two stores is the destructor's, not ReleaseData's (Info first).
// Matching: `this` in ecx; DeleteFunc is __fastcall (Data in ecx); the block goes back through operator delete.
#include "engine/CCountedPointer.h"
// NF79 (round 7, q10974): the pointee is const C3DMesh2: FableWin.pdb 0x029364C0 ??1?$CCountedPointer@$$CBVC3DMesh2@@@@QAE@XZ, and in retail
//   CEngineClothSimulationAnimatedMesh::RunSimulation 0x00B82100 destroys with it (`lea ecx,[esp+24h] ; call 0x00A36EC0`) the very local it
//   passed as argument 1 to C3DMeshInfo::GetMesh 0x00A25E70 (`lea edx,[esp+2Ch]` after two pushes), whose Ego_r.pdb + FableWin.pdb
//   signature types it CCountedPointer<const C3DMesh2>&. Was the placeholder pointee CUnprovenPointee_00a36ec0.
class C3DMesh2;
template CCountedPointer<const C3DMesh2>::~CCountedPointer();
