// Leaf: ?Copy@CCameraCollisionDef@@UAEXPBVCDefClassBase@@@Z
// CCameraCollisionDef::Copy(CDefClassBase const*)
#define FABLE_RETAIL_CCAMERACOLLISIONDEF_VIRTUAL_MODEL
#define FABLE_RETAIL_DEF_CLASS_BASE
#include "engine/CCameraCollisionDef.h"  // retyped onto the PDB layout; byte parity re-verified
#include "engine/CDefClassBase.h"                       // retail def chain (0x25 packed, vtable 0x01230BCC)


// Base-class Copy, called via rel32.

void CCameraCollisionDef::Copy(const class CDefClassBase* src)
{
    CCameraCollisionDef* self = this;
    ((CDefClassBase*)(self))->operator=(*(const CDefClassBase*)(src));
    self->CameraCollisionMesh = ((const CCameraCollisionDef*)src)->CameraCollisionMesh;
}
// Flatten route (flatten_pilot v6, NF335): CCameraCollisionDef_Copy -> CCameraCollisionDef::Copy, the RTTI-placed PDB member; class model: engine/CCameraCollisionDef.h under FABLE_RETAIL_CCAMERACOLLISIONDEF_VIRTUAL_MODEL.
