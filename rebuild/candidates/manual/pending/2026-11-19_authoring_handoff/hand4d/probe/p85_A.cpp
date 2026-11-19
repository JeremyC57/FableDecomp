// CThingSearchTools::GetAllThingsInRoughArea<CThingGameObject, std::list<CThingGameObject*>, CFilter_Bind4<CThingFilter_IsNot, CThingFilter_CombatIsStrikeToStrike, CThingFilter_CombatOverlappingSphere, CThingFilter_CombatCanCollideOrInteractWithThing> >(const C2DBoxI&, const CFilter_Bind4<...>&, std::list<CThingGameObject*>&) const -- appends every game object in the area's map cells (each once per search) that is not the striker, is strike-to-strike with it, whose hit-location bounding sphere overlaps the strike sphere and that it can collide or interact with
// Grade: (in progress)
#define FABLE_RETAIL_CTHINGSEARCHTOOLS
#define FABLE_RETAIL_CTHINGSEARCHTOOLS_CLIPAREA
#define FABLE_RETAIL_CTHINGSEARCHTOOLS_ORIGINAL_ROUGHAREA
#define FABLE_RETAIL_CTHINGSEARCHTOOLS_ROUGHAREA_EGOD
#define FABLE_RETAIL_CTHINGMAP_PEEKCELL_INLINE
#define FABLE_RETAIL_CTCMAPWHO
#define FABLE_RETAIL_CTCMAPWHO_SEARCHID
#define FABLE_RETAIL_CTHINGGAMEOBJECT_STATTYPEFLAGS
#define FABLE_RETAIL_CTHINGPHYSICAL
#define FABLE_RETAIL_CTHINGGAMEOBJECT
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_C3DVECTOR
#define FABLE_RETAIL_C3DVECTOR_NORMALISE_SQUARE_HELPER
#define FABLE_RETAIL_HITLOCATIONS_BOUND
#define FABLE_RETAIL_C2DBOXI
#define FABLE_RETAIL_C2DBOXI_COPY
#define FABLE_RETAIL_C2DBOXI_GETWIDTH
#define FABLE_RETAIL_C2DBOXI_GETHEIGHT
#include "engine/C2DExtentsI.h"
#include "engine/C3DVector.h"
#include "engine/CThing.h"
class CTCMapwho;
#include "engine/CTCMapwho.h"
#include "engine/NThingMap_CThingMap.h"
#include "engine/CThingGameObject.h"
#include "engine/CThingSearchTools.h"
#include "engine/C2DBoxI.h"
#include "engine/CTCHitLocations.h"
#include "engine/CThingFilter_IsNot.h"
#include "engine/CFilter_Bind4.h"

namespace {
class CThingFilter_CombatIsStrikeToStrike : public std::unary_function<CThing,bool> {
public:
    bool operator()(const CThing& test_thing) const;                    // 0x008565E0
private:
    const CThing& ThingStriking;                                        // +0x00
};
class CThingFilter_CombatCanCollideOrInteractWithThing : public std::unary_function<CThing,bool> {
public:
    bool operator()(const CThing& test_thing) const;                    // 0x00857250
private:
    const CTCPhysicsBase& TCPhysics;                                    // +0x00
    bool ExcludeCreatures;                                              // +0x04
    bool ExcludeDeadCreatures;                                          // +0x05
};
}

class CThingFilter_CombatOverlappingSphere : public std::unary_function<CThing,bool> {
public:
    CThingFilter_CombatOverlappingSphere(const C3DVector& pos, float radius) : SpherePos(pos), Radius(radius) {}
    bool operator()(const CThing& test_thing) const;
private:
    C3DVector SpherePos;                                                // +0x00
    float Radius;                                                       // +0x0c
};

bool CThingFilter_CombatOverlappingSphere::operator()(const CThing& test_thing) const
{
    const CTCHitLocations* ptc_hit_locations;
    if (test_thing.PeekTC(&ptc_hit_locations)) {
        C3DVector centre_pos;
        float radius;
        ptc_hit_locations->GetBoundingSphere(centre_pos, radius, true);
        if ((centre_pos - SpherePos).GetSquaredMagnitude() < GFSqr(radius + Radius))
            return true;
    }
    return false;
}

template void CThingSearchTools::GetAllThingsInRoughArea<CThingGameObject, std::list<CThingGameObject*>, CFilter_Bind4<CThingFilter_IsNot, CThingFilter_CombatIsStrikeToStrike, CThingFilter_CombatOverlappingSphere, CThingFilter_CombatCanCollideOrInteractWithThing> >(const C2DBoxI&, const CFilter_Bind4<CThingFilter_IsNot, CThingFilter_CombatIsStrikeToStrike, CThingFilter_CombatOverlappingSphere, CThingFilter_CombatCanCollideOrInteractWithThing>&, std::list<CThingGameObject*>&) const;
