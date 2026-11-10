// 00814ea0 NHitLocations::CPhysicalPrimitiveSphere::GetClosestPointToPos2D -- horizontal closest point and direction.
// Leaf: ?GetClosestPointToPos2D@CPhysicalPrimitiveSphere@NHitLocations@@UBE?AVC3DVector@@ABVCThing@@ABV3@AAV3@@Z
// Retail: 00814ea0,238 bytes, const virtual thiscall slot10.
#define FABLE_RETAIL_C3DVECTOR
#define FABLE_RETAIL_C3DVECTOR_ISNONZERO_ZYX
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CDEFSTRINGTABLE
#define FABLE_RETAIL_CCLASS
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTCPHYSICSBASE
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CTHINGPHYSICAL
#define FABLE_RETAIL_CTHINGGAMEOBJECT
#define FABLE_RETAIL_CTHINGCREATUREBASE
#define FABLE_RETAIL_CTHINGCREATUREBASE_COMBATRADIUS
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#include "engine/C3DVector.h"
#include "engine/CTCPhysicsBase.h"
#include "engine/CThingCreatureBase.h"
#include "engine/NHitLocations_CPhysicalPrimitiveSphere.h"
extern const float DELTA_NOTIONAL_ZERO;
namespace NHitLocations {
C3DVector CPhysicalPrimitiveSphere::GetClosestPointToPos2D(const CThing& thing, const C3DVector& pos, C3DVector& direction) const
{
    C3DVector centre = GetSphereBase(thing);
    float radius = GetSphereRadius(thing);
    C3DVector offset = pos - centre;
    offset.Z = 0.0f;
    if (offset.IsNonZero())
        offset.AccurateNormalise();
    direction = offset;
    return centre + offset * radius;
}
}