// CPhysicsMeshObject::GetBoundingBoxXEdge() const -- half the mesh bounding box's X extent rotated by the object's position matrix (no translation)
// Leaf: ?GetBoundingBoxXEdge@CPhysicsMeshObject@@QBE?AVC3DVector@@XZ
// Flags: none
// Retail: Fable.exe (TLC, VC7.1) 0x00A11C20, 103 bytes (`ret 4`, C3DVector by hidden pointer); 0.5f @ 0x0122F59C, 0.0f @ 0x0122DEDC.
// Identity: Ego_r.pdb ?GetBoundingBoxXEdge@CPhysicsMeshObject@@QBE?AVC3DVector@@XZ (the X/Y/Z edges are 0x00A11C20 / 0x00A11C90 /
// 0x00A11BA0); the manifest's CEngineInternalPrimitiveParticleLine::GetWorldPosition is propagated and wrong.
// Shape: the extent vector rotated in place by FableWin.pdb's inline CMatrix3x4::TransformVector(C3DVector&) (xx/yy/zz copies,
// x*E1k + y*E2k + z*E3k per row), as an in-place transform keeps the half extent on the FPU for the X row (0x00A11C3B `fst`).
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_C3DVECTOR
#define FABLE_RETAIL_C3DBOUNDINGBOX
#define FABLE_RETAIL_CMATRIX3X4_TRANSFORMVECTOR
#define FABLE_RETAIL_CIVCOUNTEDPOINTER
#define FABLE_RETAIL_CPHYSICSMESHOBJECT
#define FABLE_RETAIL_CPHYSICSMESHOBJECT_MATRIX
#define FABLE_RETAIL_CPHYSICSMESHOBJECT_EDGES
#include "engine/C3DBoundingBox.h"
#include "engine/CPhysicsMeshObject.h"
// CPhysicsMesh (no shared header): only the bounding box is read; retail +0x84 (Min) / +0x90 (Max).
class CPhysicsMesh : public CIVCountedPointeeBase {
public:
    const C3DBoundingBox& PeekBoundingBox() const { return BoundingBox; }
private:
    unsigned char _pad[0x84 - sizeof(CIVCountedPointeeBase)];
    C3DBoundingBox BoundingBox;
};
C3DVector CPhysicsMeshObject::GetBoundingBoxXEdge() const
{
    const C3DBoundingBox& box = Mesh->PeekBoundingBox();
    C3DVector v((box.PeekMaxCoord().X - box.PeekMinCoord().X) * 0.5f, 0.0f, 0.0f);
    PositionMatrix.TransformVector(v);
    return v;
}