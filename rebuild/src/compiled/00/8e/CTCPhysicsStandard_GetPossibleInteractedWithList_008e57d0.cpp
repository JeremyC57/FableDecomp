// CTCPhysicsStandard::GetPossibleInteractedWithList(const C2DBoxI& box, vector<CThing*>& things, float extraRadius) const -- when the body interacts or collides with things at all, gather the things in the box that it can collide or interact with, that overlap its sphere (radius + extraRadius) and that have physics (interface 2)
// Plan phase 3 (2026-11-04), C3DVector wave: the local C3DVector stand-in replaced by the shared engine/C3DVector.h (PDB C3DVector TPI 0x2783: public X/Y/Z at 0/4/8, size 12).
// Retail: Fable.exe (TLC, VC7.1) @ 0x008E57D0, 164 bytes (`ret 0Ch`; PDB ?GetPossibleInteractedWithList@CTCPhysicsStandard@@IBEXABVC2DBoxI@@AAV?$vector@PAVCThing@@...@@M@Z).
// Callees: virtual CanInteractWithThings (+0xf0), CanCollideWithThings (+0xe4), CanCollideWithThingsOneWay (+0xe8) and
//   IsExcludingCreatures (+0xf4) (engine/CTCPhysicsBase.h retail vtable); CTCPhysicsBase::GetThingSearchTools() const @ 0x006AFD40 (GetWorld() const ->
//   CWorld::PThingSearchTools +0x20; manifest "CThingCreatureBase::GetCombatManager" is propagated), CThingSearchTools::
//   GetAllThingsInRoughArea<const CThing, vector<CThing*>, CFilter_Bind3<...>> @ 0x008E60D0 (const, box / filter / out).
// Layout: Ego_r.pdb filters: CThingFilter_CanCollideOrInteractWithThing {TCPhysics, ExcludeCreatures} (no shared header: modelled
//   below, constructor + declared operator() only), CThingFilter_OverlappingSphere {SpherePos, Radius}, CThingFilter_HasTC {Type},
//   CFilter_Bind3 {F1, F2, F3} (engine/ headers); CTCPhysicsBase Position +0x0c, Radius +0x34.
// Shape: the three filters are named locals (frame: HasTC +4, CanCollide +8, Bind3 +0x10, sphere +0x1c) and the Bind3 a temporary
//   in the call.
// Name fix (NF59, queue q10515; NF55 porting 0x00725C40 Move): Ego_r.pdb 0x0090DA20
//   ?GetPossibleInteractedWithList@CTCPhysicsStandard@@IBEXABVC2DBoxI@@AAV?$vector@PAVCThing@@V?$allocator@PAVCThing@@@std@@@std@@M@Z
//   (protected const). The local CTCPhysicsBase (made-up virtual slot names GetCollidesWithThings / GetInteractsWithThings), C2DBoxI,
//   vector, filter and CThingSearchTools models are replaced by the shared headers: engine/CTCPhysicsStandard.h
//   (FABLE_RETAIL_CTCPHYSICSSTANDARD_MOVE declares the member), the retail engine/CTCPhysicsBase.h vtable (CanInteractWithThings /
//   CanCollideWithThings / CanCollideWithThingsOneWay / IsExcludingCreatures, GetThingSearchTools), engine/CThingSearchTools.h (the
//   template declared), the filter headers and stlport/fable_containers.h.
// Leaf: ?GetPossibleInteractedWithList@CTCPhysicsStandard@@IBEXABVC2DBoxI@@AAV?$vector@PAVCThing@@V?$allocator@PAVCThing@@@std@@@std@@M@Z
#define FABLE_RETAIL_CTHINGSEARCHTOOLS
#define FABLE_RETAIL_CTCPHYSICSSTANDARD
#define FABLE_RETAIL_CTCPHYSICSSTANDARD_MOVE
#define FABLE_RETAIL_CTCPHYSICSBASE
#define FABLE_RETAIL_CTCPHYSICSBASE_GETTHINGSEARCHTOOLS
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_C3DVECTOR
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CCHARSTRING
#include "engine/CThing.h"
#include "engine/C3DVector.h"
#include "engine/C2DBoxI.h"
#include "engine/CTCPhysicsBase.h"
#include "engine/CTCPhysicsStandard.h"
#include "engine/CThingSearchTools.h"
#include "engine/CThingFilter_OverlappingSphere.h"
#include "engine/CThingFilter_HasTC.h"
#include "engine/CFilter_Bind3.h"

// Ego_r.pdb CThingFilter_CanCollideOrInteractWithThing {TCPhysics, ExcludeCreatures}; no shared header yet.
class CThingFilter_CanCollideOrInteractWithThing : public std::unary_function<CThing,bool> {
    const CTCPhysicsBase* TCPhysics;                   // +0x00
    bool                  ExcludeCreatures;            // +0x04
public:
    CThingFilter_CanCollideOrInteractWithThing(const CTCPhysicsBase* physics, bool excludeCreatures)
        : TCPhysics(physics), ExcludeCreatures(excludeCreatures) { }
    bool operator()(const CThing& thing) const;
};

void CTCPhysicsStandard::GetPossibleInteractedWithList(const C2DBoxI& area, std::vector<CThing*>& things, float range) const
{
    if (CanInteractWithThings() || CanCollideWithThings() || CanCollideWithThingsOneWay()) {
        CThingFilter_CanCollideOrInteractWithThing canFilter(this, IsExcludingCreatures());
        CThingFilter_OverlappingSphere sphereFilter(Position, range + Radius);
        CThingFilter_HasTC physicsFilter(TCI_PHYSICS);
        GetThingSearchTools().GetAllThingsInRoughArea<const CThing>(area,
            CFilter_Bind3<CThingFilter_CanCollideOrInteractWithThing, CThingFilter_OverlappingSphere, CThingFilter_HasTC>(
                canFilter, sphereFilter, physicsFilter), things);
    }
}
