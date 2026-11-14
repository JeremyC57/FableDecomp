// NThingShot::CTCShotDefinitionBase::UpdateCollision(bool& killed_me) -- sweep the shot's last weapon-trail segment against the hit locations of the solid things in a box around it and report the nearest hit as a collision
// Grade: DIFFER(1643v1648) -- 52 hunks, one register permutation: retail colours hitLocations (ebp) before this (ebx); ours this (prio 56, ebp) before hitLocations (prio 52, ebx), so the first && temp takes bl, const 0 goes to edi and weaponDef.Object gets no callee-saved register
// Leaf: ?UpdateCollision@CTCShotDefinitionBase@NThingShot@@MAEXAA_N@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x007BFE30, 1648 bytes, thiscall `ret 4`. Identity: Ego_r.pdb ?UpdateCollision@CTCShotDefinitionBase@NThingShot@@MAEXAA_N@Z
//   (donor 0x007C0E70) + FableWin.pdb; the header's COLLISION opt-in declares it (protected virtual).
#define FABLE_RETAIL_CTCSHOTDEFINITIONBASE_COLLISION
#define FABLE_RETAIL_CTCSHOTDEFINITIONBASE_MOVEMENT
#define FABLE_RETAIL_CTCSHOTDEFINITIONBASE_ONHITBYMELEE
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CCHARSTRING_FORCEINLINE_EQ
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CINTELLIGENTPOINTER_COPY
#define FABLE_RETAIL_CINTELLIGENTPOINTER_CONVERT_MUTABLE
#define FABLE_RETAIL_CINTELLIGENTPOINTER_ASSIGN
#define FABLE_RETAIL_C3DVECTOR_CROSS_MEMBER_X_GROUP
#define FABLE_RETAIL_C3DVECTOR_NONZERO_SQUARED_XYZ
#define FABLE_RETAIL_C3DVECTOR_ANGLE_BODIES
#define FABLE_RETAIL_C3DVECTOR
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CENGINEPERSISTENTPRIMITIVEBASE
#define FABLE_RETAIL_CRESOURCELIST
#define FABLE_RETAIL_CCLASS
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CTHING_ORIGINAL_VTABLE
#define FABLE_RETAIL_CTHINGPHYSICAL
#define FABLE_RETAIL_CTHINGGAMEOBJECT
#define FABLE_RETAIL_CTHING_CUTSCENEPAUSE
#define FABLE_RETAIL_CTHING_GETTC_EXPANDED
#define FABLE_RETAIL_CTHING_GETTCDRIVER
#define FABLE_RETAIL_CTHING_MOREACCESS
#define FABLE_RETAIL_CTHING_STATEACCESS
#define FABLE_RETAIL_CTHING_LOADSINGLE
#define FABLE_RETAIL_CTHING_VIRTUALS
#define FABLE_RETAIL_CTHINGPHYSICAL_VIRTUALS
#define FABLE_RETAIL_CTCPHYSICSBASE
#define FABLE_RETAIL_CWORLD
#define FABLE_RETAIL_CWORLDMAP
#define FABLE_RETAIL_CMAINGAMECOMPONENT
#define FABLE_RETAIL_CGAMEDEFINITIONMANAGER
#define FABLE_RETAIL_CDEFINITIONMANAGER
#define FABLE_RETAIL_CINTELLIGENTPOINTER_COMPARE_NULL_INT
#define FABLE_RETAIL_IS_THING_ALIVE
#define FABLE_RETAIL_STLP_FIND_RANDOM_DISPATCH
#define FABLE_RETAIL_STLP_FIND_NOINLINE_BODY
#define FABLE_RETAIL_CINTELLIGENTPOINTER_EQUAL_MIXED
#define FABLE_RETAIL_CCLASSFACTORY
#define FABLE_RETAIL_CTHINGMANAGER
#define FABLE_RETAIL_CTHINGMANAGER_ALLPAUSE
#define FABLE_RETAIL_CTHINGSEARCHTOOLS
#define FABLE_RETAIL_FILTER_BIND3_FUNCTION
#define FABLE_RETAIL_CARRAY_SIZE_CTOR_OUTOFLINE
#define FABLE_RETAIL_CTCHITLOCATIONS_TRIANGLE
#define FABLE_RETAIL_CTCWEAPONTRAIL_UPDATEWEAPONTRAILS
#include "stlport/fable_containers.h"
#include "engine/CThing.h"
#include "engine/NThingShot_CTCShotDefinitionBase.h"
#include "engine/CTCWeaponTrail.h"
#include "engine/CThingBaseDef.h"
#include "engine/CWeaponDef.h"
#include "engine/CThingShotDef.h"
#include "engine/CDefClassBase_GetSubDef.h"
#include "engine/CThingManager.h"
#include "engine/CThingSearchTools.h"
#include "engine/C2DBoxI.h"
#include "engine/CFilter_Bind3.h"
#include "engine/CThingFilter_IsNot.h"
#include "engine/CThingFilter_SolidToThing.h"
#include "engine/CTCHitLocations.h"
#include "engine/CArray.h"
#include "engine/ThingAlive.h"
#include "stlport/fable_algorithm.h"

namespace NCreatureMode { bool __fastcall IsRolling(const CThing& thing); }   // 0x0085EEB0 ?IsRolling@NCreatureMode@@YI_NABVCThing@@@Z

namespace {
// Ego_r.pdb / FableWin.pdb CThingFilter_ShotsToCheckForCollisionWith@?A0x6dcc4763 (file-local, empty: the row passes a byte slot);
// its ??R body is compiled with the GetAllThingsInRoughArea instance 0x007C2D10, not here. Declaration only.
class CThingFilter_ShotsToCheckForCollisionWith : public std::unary_function<CThing,bool> {
public:
    bool operator()(const CThing& thing) const;
};
}

void NThingShot::CTCShotDefinitionBase::UpdateCollision(bool& killed_me)
{
    killed_me = false;
    bool useExtendedTrails = false;
    CDefPointer<const CWeaponDef> weaponDef;
    if (ParentThing->GetPBaseDef()->GetSubDef(&weaponDef) && weaponDef->UseExtendedProjectileTrails)
        useExtendedTrails = true;
    CTCWeaponTrail* trail;
    if (ParentThing->GetTC(&trail)) {
        std::vector<std::pair<C3DVector, C3DVector> > points;
        if (trail->GetLastProjectileWeaponTrailPoints(points) && points.size() >= 2) {
            const std::pair<C3DVector, C3DVector>& last = *(points.end() - 1);
            const std::pair<C3DVector, C3DVector>& prev = *(points.end() - 2);
            C3DVector centre;
            C3DVector line;
            if (useExtendedTrails) {
                centre = (last.first + prev.second) * 0.5f;
                line = last.first - prev.second;
            } else {
                centre = (prev.first + last.first) * 0.5f;
                line = last.first - prev.first;
            }
            C2DBoxI area(C2DCoordI((long)centre.X, (long)centre.Y), (long)((line.GetMagnitude() + 0.5f) * 1.5f));
            std::vector<CThing*> things;
            CThing* shooter = PShooter.Get();
            CThing* ignore = ParentThing;
            if (IS_THING_ALIVE(shooter))
                ignore = shooter;
            GetThingSearchTools().GetAllThingsInRoughArea<CThing>(area,
                GFFilter_Bind3(CThingFilter_IsNot(*ignore), CThingFilter_ShotsToCheckForCollisionWith(), CThingFilter_SolidToThing(*ParentThing)),
                things);

            float closestDistanceSquared = 1.0e10f;
            CThing* closestThing = 0;
            NHitLocations::CHitLocation* closestHitLocation = 0;
            C3DVector closestPos;
            C3DVector closestNormal;
            for (std::vector<CThing*>::iterator it = things.begin(); it != things.end(); ++it) {
                CThing* thing = *it;
                if ((thing->GetTypeFlags() & 0x20) &&
                    (GetThingManager().IsNonScriptedEntitesPauseMode() || GetThingManager().IsAllEntitesPauseMode()))
                    continue;
                if (NCreatureMode::IsRolling(*thing) && !PShotDef->CanHitRollingTargets)
                    continue;
                CTCHitLocations* hitLocations;
                if (thing->GetTC(&hitLocations) &&
                    std::find(HitThingsList.begin(), HitThingsList.end(), CIntelligentPointer<CThing>(thing)) == HitThingsList.end()) {
                C3DVector hitPos;
                C3DVector hitNormal;
                NHitLocations::CHitLocation* hitLocation;
                if (useExtendedTrails) {
                    CArray<C3DVector> triangle(3);
                    triangle[0] = last.first;
                    triangle[1] = last.second;
                    triangle[2] = prev.first;
                    hitLocation = hitLocations->GetPCombatFirstHitLocationIntersectionWithTriangle(triangle, hitPos, hitNormal, 0.25f);
                    if (!hitLocation) {
                        triangle[0] = last.second;
                        triangle[1] = prev.second;
                        triangle[2] = prev.first;
                        hitLocation = hitLocations->GetPCombatFirstHitLocationIntersectionWithTriangle(triangle, hitPos, hitNormal, 0.25f);
                    }
                } else {
                    hitLocation = hitLocations->GetPCombatFirstHitLocationIntersectionWithFiniteLine(prev.first, last.first, hitPos, hitNormal, 0.0f);
                }
                if (hitLocation) {
                    float distanceSquared = (hitPos - prev.first).GetSquaredMagnitude();
                    if (distanceSquared < closestDistanceSquared) {
                        closestDistanceSquared = distanceSquared;
                        closestPos = hitPos;
                        closestNormal = hitNormal;
                        closestThing = thing;
                        closestHitLocation = hitLocation;
                    }
                }
                }
            }
            if (IS_THING_ALIVE(closestThing)) {
                PBestCollisionHitLocation = closestHitLocation;
                ParentThing->OnCollision((ECollideType)1, closestPos, closestNormal, closestThing);
            }
        }
    }
}
