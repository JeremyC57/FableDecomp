// CPhysicsMesh::GetRigidBodyBoundingPoints(std::vector<C3DVector>& points) const -- copy the mesh points and make them relative to the centre of gravity
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00A6CB20, length 127 bytes (+ int3 padding).
// Leaf: ?GetRigidBodyBoundingPoints@CPhysicsMesh@@QBEXAAV?$vector@VC3DVector@@V?$allocator@VC3DVector@@@std@@@std@@@Z
// Callees: std::vector<C3DVector>::operator=(const vector&) @ 0x0075B8D0 (STLport, out of line).
// Layout: Ego_r.pdb CPhysicsMesh with the retail 12-byte CArray: TotalPoints +0x3c, CentreOfGravity +0xa8 (PDB +0xc4).
// Shape: the output array kept in ebx; the element count (`/12` via 0x2AAAAAAB) is re-derived from first/last every pass
// with an unsigned index (`jb`), the loop entered only for a non-empty array; each element is reached as
// `*(begin() + i)` (the STLport operator[] form) and the centre subtracted component by component; `ret 4`.
// Naming (NF138, 2026-11-20; q4802): the local C3DVector / CArray / CPhysicsMesh models replaced by engine/CPhysicsMesh.h
//   (FABLE_RETAIL_CPHYSICSMESH_CALCULATEBOUNDINGINFO types TotalPoints as CArray<C3DVector>; new opt-in GETRIGIDBODYBOUNDINGPOINTS), engine/CArray.h,
//   engine/C3DVector.h and the STLport vector (FABLE_RETAIL_STLP_VECTOR_ASSIGN). Own leaf: the parameter is the PDB's
//   std::vector<C3DVector>& (Ego_r.pdb 0x00B4DCE0 / FableWin.pdb 0x030F7040), was the local CArray<C3DVector>&; 0x0075B8D0 binds
//   ??4?$vector@VC3DVector@@V?$allocator@VC3DVector@@@std@@@std@@QAEAAV01@ABV01@@Z (was ??4?$CArray@VC3DVector@@@@QAEAAV0@ABV0@@Z).
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CARRAY
#define FABLE_RETAIL_C3DVECTOR
#define FABLE_RETAIL_C3DVECTOR_SUBTRACT_ASSIGN
#define FABLE_RETAIL_STLP_VECTOR_ASSIGN
#define FABLE_RETAIL_CPHYSICSMESH_CALCULATEBOUNDINGINFO
#define FABLE_RETAIL_CPHYSICSMESH_GETRIGIDBODYBOUNDINGPOINTS
#include "engine/C3DVector.h"
#include "engine/CArray.h"
#include "engine/CPhysicsMesh.h"

void CPhysicsMesh::GetRigidBodyBoundingPoints(std::vector<C3DVector>& points) const
{
    points = TotalPoints;
    for (unsigned long i = 0; i < points.size(); ++i)
        points[i] -= CentreOfGravity;
}
