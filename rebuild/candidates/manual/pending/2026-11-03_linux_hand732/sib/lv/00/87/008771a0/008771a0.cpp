// NCameraModes::CTools::GetThingPointIntersectionFromTo(const std::vector<const CThing*>& things, const C3DVector& from, const C3DVector& to, C3DVector& hitPos, C3DVector& hitNormal, const CThing** hitThing, bool useCameraMeshes, ELOSTest test) const -- nearest camera-blocking thing on the segment from->to
// Leaf: ?GetThingPointIntersectionFromTo@CTools@NCameraModes@@QBE_NABV?$vector@PBVCThing@@V?$allocator@PBVCThing@@@std@@@std@@ABVC3DVector@@1AAV5@2PAPBVCThing@@_NW4ELOSTest@12@@Z
// Flags:
// Retail: Fable.exe (TLC, VC7.1) function @ 0x008771A0, length 1264 bytes (`ret 20h`).
// Naming: Ego_r.pdb 0x0085DBB0 / FableWin.pdb ?GetThingPointIntersectionFromTo@CTools@NCameraModes@@QBE_NABV?$vector@PBVCThing@@...
//   _NW4ELOSTest@12@@Z (eight arguments = `ret 20h`; Ego_r orders it between PotentialFieldBufferPoints 0x0085DA60 = retail
//   0x00876E10 and GetStandardThings; the landed IsLineOfSight 0x00877C40 is two functions later). `this` is never read.
// Callees (PDB spellings): C3DVector::AccurateNormalise 0x00A14440; CVectorMap<ETCInterfaceType,CTCBase*>::LowerBound const
//   0x004365B0 (CThing::PeekTC<const CTCCameraCollision> key 0xF8 = TCI_CAMERA_COLLISION, PeekTC<const CTCHitLocations> key 0x7F =
//   TCI_HIT_LOCATIONS; both PeekTC instantiations are in FableWin.pdb); CTCCameraCollision::HasCameraPhysicsMeshObject 0x007E3010 /
//   GetCameraPhysicsMeshObject 0x007E30A0 (by value, hidden result); CPhysicsMeshObject::GetPointIntersectionFromTo const 0x00A12450
//   and its dtor 0x00A127D0; CTCHitLocations::GetPCombatFirstHitLocationIntersectionWithFiniteLine const 0x00816AA0 (the 5-byte
//   `jmp` into the non-const 0x008168B0; was bound as @FableImpl_816aa0@4); CThingPhysical::IsThingAbleToComeBetweenCameraAndObservedThing
//   0x006A5AB0; CTCHitLocations::GetBoundingSphere const 0x00815420. The thing-type test reads CThing::GetTypeFlags (inline,
//   ThingTypeFlags +0x6C): bit 1 must be set, bit 3 only passes when the ELOSTest has bit 3.
// Shape: the squared distance is taken from the unnamed `to - from` before the direction local is built (retail keeps the
//   difference on the x87 stack, `fst` x / z to temporaries); a `bool hit` declared before the mesh/hit-location branches gives
//   retail's register map (ebp = from, cc, bl = hit). The bounding-sphere re-test is `if (cc || able) { if (re-test) accept; }
//   else accept;` (an `accept` flag variable shifts the frame by 67 lines); the re-test start point is a named local (computed
//   before the `to - direction * 0.2f` argument); `hit = true` written first in the mesh branch (retail schedules `mov bl,1`
//   between the copies).
// Headers (chunk 724 round 3 overlay): new NCameraModes_CTools.h, CTCCameraCollision.h, CTCHitLocations.h; CThing.h
//   FABLE_RETAIL_CTHING + GetTypeFlags; CThingPhysical.h FABLE_RETAIL_CTHINGPHYSICAL + IsThingAbleToComeBetweenCameraAndObservedThing.
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CTHINGPHYSICAL
#include "engine/NCameraModes_CTools.h"
#include "engine/CThingPhysical.h"
#include "engine/CTCCameraCollision.h"
#include "engine/CTCHitLocations.h"
#include "engine/CPhysicsMeshObject.h"
#include "engine/CMeshIntersectionData.h"
#include "engine/C3DVector.h"

namespace NCameraModes {
bool CTools::GetThingPointIntersectionFromTo(const std::vector<const CThing*>& things, const C3DVector& from, const C3DVector& to,
    C3DVector& hitPos, C3DVector& hitNormal, const CThing** hitThing, bool useCameraMeshes, ELOSTest test) const
{
    const CThing* closestThing = 0;
    float closestDistanceSquared = (to - from).GetSquaredMagnitude();
    C3DVector direction = to - from;
    direction.AccurateNormalise();
    C3DVector closestPos;
    C3DVector closestNormal;
    for (std::vector<const CThing*>::const_iterator it = things.begin(); it != things.end(); ++it) {
        const CThing* thing = *it;
        if (thing && !thing->IsDead()) {
            unsigned long flags = thing->GetTypeFlags();
            if ((!(flags & 8) || (test & 8)) && (flags & 2)) {
                C3DVector pos;
                C3DVector normal;
                bool hit = false;
                if (useCameraMeshes) {
                    const CTCCameraCollision* cameraCollision;
                    if (!thing->PeekTC(&cameraCollision) || !cameraCollision->HasCameraPhysicsMeshObject())
                        cameraCollision = 0;
                    if (cameraCollision) {
                        CPhysicsMeshObject mesh = cameraCollision->GetCameraPhysicsMeshObject();
                        CMeshIntersectionData data;
                        if (mesh.GetPointIntersectionFromTo(from, to, data)) {
                            hit = true;
                            pos = data.IntersectionPosition;
                            normal = data.Normal * -1.0f;

                        }
                    }
                    if (!hit) {
                        const CTCHitLocations* hitLocations;
                        if (thing->PeekTC(&hitLocations)) {
                            C3DVector locationPos;
                            C3DVector locationNormal;
                            if (hitLocations->GetPCombatFirstHitLocationIntersectionWithFiniteLine(from, to, locationPos, locationNormal, 0.25f)) {
                                if (cameraCollision || static_cast<const CThingPhysical*>(thing)->IsThingAbleToComeBetweenCameraAndObservedThing()) {
                                    C3DVector centre;
                                    float radius;
                                    hitLocations->GetBoundingSphere(centre, radius, true);
                                    C3DVector testPos;
                                    C3DVector testNormal;
                                    C3DVector start = direction * radius * 2.5f + to;
                                    if (hitLocations->GetPCombatFirstHitLocationIntersectionWithFiniteLine(start, to - direction * 0.2f, testPos, testNormal, 0.25f)) {
                                        pos = locationPos;
                                        normal = locationNormal;
                                        hit = true;
                                    }
                                } else {
                                    pos = locationPos;
                                    normal = locationNormal;
                                    hit = true;
                                }
                            }
                        }
                    }
                } else {
                    const CTCHitLocations* hitLocations;
                    if (thing->PeekTC(&hitLocations) &&
                        hitLocations->GetPCombatFirstHitLocationIntersectionWithFiniteLine(from, to, pos, normal, 0.25f))
                        hit = true;
                }
                if (hit) {
                    float distanceSquared = (pos - from).GetSquaredMagnitude();
                    if (distanceSquared < closestDistanceSquared) {
                        closestDistanceSquared = distanceSquared;
                        closestPos = pos;
                        closestNormal = normal;
                        closestThing = thing;
                    }
                }
            }
        }
    }
    if (closestThing) {
        *hitThing = closestThing;
        hitPos = closestPos;
        hitNormal = closestNormal;
        return true;
    }
    return false;
}
}