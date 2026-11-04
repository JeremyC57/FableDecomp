// CPhysicsMeshObject::GetBoundingBoxCentre() const -- the centre of the mesh's bounding box transformed into the object's frame (rotation rows plus position)
// Leaf: ?GetBoundingBoxCentre@CPhysicsMeshObject@@QBE?AVC3DVector@@XZ
// Flags: none
// Retail: Fable.exe (TLC, VC7.1) 0x00A11AE0, 185 bytes (`ret 4`, C3DVector by hidden pointer); 0.5f @ 0x0122F59C. Callees: none.
// Identity: Ego_r.pdb / FableWin.pdb ?GetBoundingBoxCentre@CPhysicsMeshObject@@QBE?AVC3DVector@@XZ (sibling of the landed edges
//   0x00A11C20 / 0x00A11C90 / 0x00A11BA0, same model).
// Shape: the box's inline GetCenter() `(MaxCoord + MinCoord) * 0.5f` into a named local, then CMatrix3x4::TransformPoint's
//   body (FableWin xx/yy/zz copies) written out on the local: the X and Y rows in the retail header's form
//   `E1k*xx+(E2k*yy+E3k*zz)+E4k`, the Z row as `E23*yy+(E13*xx+E33*zz)+E43`, which gives retail's E33, E13, E23 term order
//   (0x00A11B6A..0x00A11B82; the header's TransformPoint gives E33, E23, E13 there, 185v185 dl=6, and its accumulated form dl=4).
#define FABLE_RETAIL_CPHYSICSMESHOBJECT_CENTRE
#define FABLE_RETAIL_CMATRIX3X4
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

C3DVector CPhysicsMeshObject::GetBoundingBoxCentre() const
{
    C3DVector centre = Mesh->PeekBoundingBox().GetCenter();
    const CMatrix3x4& m = PositionMatrix;
    const float xx=centre.X;
    const float yy=centre.Y;
    const float zz=centre.Z;
    centre.X=m.E11*xx+(m.E21*yy+m.E31*zz)+m.E41;
    centre.Y=m.E12*xx+(m.E22*yy+m.E32*zz)+m.E42;
    centre.Z=m.E23*yy+(m.E13*xx+m.E33*zz)+m.E43;
    return centre;
}