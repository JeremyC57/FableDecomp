// CEngineInternalPrimitiveParticleMesh::Update(const CEnginePrimitive& primitive, unsigned long flags) -- apply a particle-mesh
// descriptor: flags, colour and the object transform built from the orientation set scaled by the size percentages.
// Leaf: ?Update@CEngineInternalPrimitiveParticleMesh@@UAEXABVCEnginePrimitive@@K@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00BC2480, 289 bytes (`ret 8`, virtual).
// Naming: FableWin.pdb / Ego_r.pdb ?Update@CEngineInternalPrimitiveParticleMesh@@UAEXABVCEnginePrimitive@@K@Z; the descriptor is Ego_r.pdb
//   CEnginePrimitiveParticleMesh (WorldPos +0x0C, Size +0x18, RightHandedSet +0x24, Colour +0x3C). Pool: 0.01f 0x01230A5C.
// Shape: the three size percentages are separate floats (x and y stay on the x87 stack, z is homed in the dead `flags` argument
//   slot [esp+2Ch]; a C3DVector local would take 12 frame bytes: 0x30 v 0x24); Forward and Up are by-value copies scaled in place
//   (operator*=), the right vector is CRightHandedSet::GetRight() read from the descriptor (Forward x Up, every difference grouped) and
//   scaled in place. The matrix rows are stored element by element, the first two negated at the store (`fld ; fchs ; fstp`,
//   0x00BC254E..0x00BC2582) and the translation read after them; CMatrix3x4::Set(-right, -forward, up, pos) builds whole negated
//   temporaries first (322 bytes), so the element form is kept.
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CENGINEPERSISTENTPRIMITIVEBASE
#define FABLE_RETAIL_CENGINEINTERNALPRIMITIVEBASE_ATTACHED
#define FABLE_RETAIL_CENGINEINTERNALPRIMITIVEPARTICLEMESH_CLASS
#define FABLE_RETAIL_CENGINEPRIMITIVEHANDLE
#define FABLE_RETAIL_CIVCOUNTEDPOINTER
#define FABLE_RETAIL_CRIGHTHANDEDSET
#define FABLE_RETAIL_CMATRIX3X4
#define FABLE_RETAIL_C3DVECTOR
#define FABLE_RETAIL_CRIGHTHANDEDSET_RIGHT_COMPONENTS_GROUPED
#include "engine/C3DVector.h"
#include "engine/CRightHandedSet.h"
#include "engine/CMatrix3x4.h"
#include "engine/CEnginePrimitiveParticleMesh.h"
#include "engine/CEngineInternalPrimitiveParticleMesh.h"
void CEngineInternalPrimitiveParticleMesh::Update(const CEnginePrimitive& primitive,unsigned long flags)
{
    const CEnginePrimitiveParticleMesh& mesh=static_cast<const CEnginePrimitiveParticleMesh&>(primitive);
    EnginePrimitiveFlags=flags;
    Colour=mesh.Colour;
    float scale_x=mesh.Size.X*0.01f;
    float scale_y=mesh.Size.Y*0.01f;
    float scale_z=mesh.Size.Z*0.01f;
    C3DVector forward=mesh.RightHandedSet.GetForward();
    C3DVector up=mesh.RightHandedSet.GetUp();
    C3DVector right=mesh.RightHandedSet.GetRight();
    right*=scale_x;
    forward*=scale_y;
    up*=scale_z;
    ObjectTransform.E11=-right.X; ObjectTransform.E12=-right.Y; ObjectTransform.E13=-right.Z;
    ObjectTransform.E21=-forward.X; ObjectTransform.E22=-forward.Y; ObjectTransform.E23=-forward.Z;
    ObjectTransform.E31=up.X; ObjectTransform.E32=up.Y; ObjectTransform.E33=up.Z;
    ObjectTransform.E41=mesh.WorldPos.X; ObjectTransform.E42=mesh.WorldPos.Y; ObjectTransform.E43=mesh.WorldPos.Z;
}