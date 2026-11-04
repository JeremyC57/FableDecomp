// NCameraModes::CTools::GetMeshExtent(const CThing& thing) const -- for a thing with a physics mesh, the squared lengths of the mesh's world bounding-box X and Y edges summed (0 without a mesh)
// Leaf: ?GetMeshExtent@CTools@NCameraModes@@QBEMABVCThing@@@Z
// Flags: none
// Retail: Fable.exe (TLC, VC7.1) @ 0x00875F00, 175 bytes, `ret 4` (`this` unused); Ego_r.pdb ?GetMeshExtent@CTools@NCameraModes@@QBEMABVCThing@@@Z.
// Callees: CTCPhysicsBase virtual GetPhysicsMeshInfo (slot 93, +0x174), CTCPhysicsBase::GetPhysicsMeshObject() const 0x006B0850 (by value),
//   CPhysicsMeshObject::GetBoundingBoxXEdge() const 0x00A11C20 / GetBoundingBoxYEdge() const 0x00A11C90 (Ego_r.pdb, by value),
//   CPhysicsMeshObject::~CPhysicsMeshObject 0x00A127D0. Constant 0x0122DEDC = 0.0f.
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CCLASS
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_C3DVECTOR
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CTCPHYSICSBASE
#define FABLE_RETAIL_CPHYSICSMESHOBJECT_EDGES
#define FABLE_RETAIL_NCAMERAMODES_CTOOLS
#include "fable_string.h"
#include "engine/C3DVector.h"
#include "engine/CThing.h"
#include "engine/CTCBase.h"
#include "engine/CTCPhysicsBase.h"
#include "engine/CPhysicsMeshObject.h"
#include "engine/NCameraModes_CTools.h"

float NCameraModes::CTools::GetMeshExtent(const CThing& thing) const
{
    if (thing.GetPhysicsTC()->GetPhysicsMeshInfo()) {
        CPhysicsMeshObject mesh = thing.GetPhysicsTC()->GetPhysicsMeshObject();
        return mesh.GetBoundingBoxXEdge().GetSquaredMagnitude() + mesh.GetBoundingBoxYEdge().GetSquaredMagnitude();
    }
    return 0.0f;
}