// CCountedPointer::operator=<C3DMesh2>(const CCountedPointer<C3DMesh2>&) -- share a mesh pointer into a const-mesh pointer (T = const C3DMesh2, U = C3DMesh2)
// Leaf: ??$?4VC3DMesh2@@@?$CCountedPointer@$$CBVC3DMesh2@@@@QAEAAV0@ABV?$CCountedPointer@VC3DMesh2@@@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00A2ACC0, 72 bytes. Callees: the count block's DeleteFunc (indirect), operator delete 0x00BFE9BC.
// Was landed as _global::CCountedPointer_Set_00a2acc0 (stand-in name; local CCountedPtr_assign model).
// T / U proven by: C3DMeshInfo::GetMesh 0x00A25E70 (PDB ?GetMesh@C3DMeshInfo@@QBE_NAAV?$CCountedPointer@$$CBVC3DMesh2@@@@KK@Z: the out
//   parameter is CCountedPointer<const C3DMesh2>&) assigns it from LOD + 0x1C = C3DMeshLODInfo::Mesh (Ego_r +0x1C CCountedPointer<C3DMesh2>)
//   at all four call sites; FableWin.pdb has exactly this member-template operator= (the only ??$?4 for C3DMesh2).
// Member: the member template operator= of engine/CCountedPointer.h (loads Data / Info, then the ShareData body), own COMDAT.
// Model: opt-in FABLE_RETAIL_CCOUNTEDPOINTER_RELEASEDATA (ReleaseData zeroes Data first; its stores die under ShareData's own).
#define FABLE_RETAIL_CCOUNTEDPOINTER_RELEASEDATA
#include "engine/CCountedPointer.h"
class C3DMesh2;
template CCountedPointer<const C3DMesh2>& CCountedPointer<const C3DMesh2>::operator=(const CCountedPointer<C3DMesh2>&);
