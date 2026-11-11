// CThingPhysical::IsHidden() const -- returns false
// Leaf: ?IsHidden@CThingPhysical@@UBE_NXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00529760, thiscall, `ret`.
// Identity: slot 50 of CThingTrackNode's vtable 0x01272CE4 (defined here: the slot holds this body in 11 vtable(s), all of CThingPhysical or derived); the Xbox 360 Test PDB virtual order (third source) aligned on 27 TLC-named slots of that vtable puts IsHidden(void) at this slot; Ego_r/FableWin name set: ?IsHidden@CThingPhysical@@UBE_NXZ; `ret` matches the signature.
// Header: engine/CThingPhysical.h (opt-ins as in the landed row NCameraModesCTools_GetThingPointIntersectionFromTo_008771a0.cpp).
#define FABLE_RETAIL_CTHINGPHYSICAL_VSLOTS
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CTHINGPHYSICAL
#include "engine/NCameraModes_CTools.h"
#include "engine/CThingPhysical.h"
#include "engine/CTCCameraCollision.h"
#include "engine/CTCHitLocations.h"
#include "engine/CPhysicsMeshObject.h"
#include "engine/CMeshIntersectionData.h"
#include "engine/C3DVector.h"
bool CThingPhysical::IsHidden() const
{
    return false;
}
