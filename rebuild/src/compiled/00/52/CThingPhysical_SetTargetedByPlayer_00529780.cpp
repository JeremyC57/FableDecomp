// CThingPhysical::SetTargetedByPlayer(long) -- does nothing
// Leaf: ?SetTargetedByPlayer@CThingPhysical@@UAEXJ@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00529780, thiscall, `ret 4`.
// Identity: slot 52 of CThingTrackNode's vtable 0x01272CE4 (defined here: the slot holds this body in 14 vtable(s), all of CThingPhysical or derived); the Xbox 360 Test PDB virtual order (third source) aligned on 27 TLC-named slots of that vtable puts SetTargetedByPlayer(long) at this slot; Ego_r/FableWin name set: ?SetTargetedByPlayer@CThingPhysical@@UAEXJ@Z; `ret` matches the signature.
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
void CThingPhysical::SetTargetedByPlayer(long)
{
}
