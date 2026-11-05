// C3DMeshObject::operator=(const C3DMeshObject&) -- memberwise copy: mesh info, the counted mesh-stats pointer (same-block test, release, copy, add-ref), the position matrix, the inverse-matrix flag and matrix, the scale
// Retail: Fable.exe (TLC, VC7.1) function @ 0x004C3380, length 120 bytes.
// Leaf: ??4C3DMeshObject
// Plan phase 5 (2026-11-05), CCountedPointer wave: the local CCountedPointer / count-block stand-ins replaced by the shared engine/CCountedPointer.h (Ego_r.pdb: CCountedPointer<T> Data +0x0 / Info +0x4; CCPPointerInfo RefCount +0x0 / DeleteFunc +0x4 / Data +0x8).
// Plan phase 3 (2026-11-04), C3DVector wave: the local C3DVector stand-in replaced by the shared engine/C3DVector.h (PDB C3DVector TPI 0x2783: public X/Y/Z at 0/4/8, size 12).
// Callees: the counted block's destroy thunk (`call [info+4]` with the data in ecx, __fastcall) and 0x00BFE9BC
// operator delete for the block when the count reaches zero.
// Layout (PDB C3DMeshObject, 120 bytes; no engine header): vptr +0, MeshInfo +4, Mesh (CCountedPointer: Data +8,
// Info +0xc) , PositionMatrix +0x10 (48-byte POD: `rep movsd` x12), InversePositionMatrixSetup +0x40,
// InversePositionMatrix +0x44, Scale +0x74.
// Shape: the counted-pointer assignment is the landed CCountedPointer::operator= (0x00754F10) inlined: the
// source block read before the `this->Info != other.Info` compare, the source object between the compare and
// its branch, the count decremented in place and re-read through the member, the block re-read for delete.
#include "engine/CCountedPointer.h"
void __cdecl operator delete(void* p);                   // 0x00BFE9BC


#include "engine/C3DVector.h"
class CMatrix3x4 { public: C3DVector XAxis, YAxis, ZAxis, Position; };   // 48-byte POD
class C3DMeshInfo;
class C3DMeshStats;

class C3DMeshObject { public:
    virtual void v00();
    C3DMeshInfo*                         MeshInfo;                    // +0x04
    CCountedPointer<const C3DMeshStats>  Mesh;                        // +0x08
    CMatrix3x4                           PositionMatrix;              // +0x10
    bool                                 InversePositionMatrixSetup;  // +0x40
    CMatrix3x4                           InversePositionMatrix;       // +0x44
    float                                Scale;                       // +0x74   sizeof == 0x78
    C3DMeshObject& operator=(const C3DMeshObject& other);
};

C3DMeshObject& C3DMeshObject::operator=(const C3DMeshObject& other)
{
    MeshInfo = other.MeshInfo;
    Mesh = other.Mesh;
    PositionMatrix = other.PositionMatrix;
    InversePositionMatrixSetup = other.InversePositionMatrixSetup;
    InversePositionMatrix = other.InversePositionMatrix;
    Scale = other.Scale;
    return *this;
}