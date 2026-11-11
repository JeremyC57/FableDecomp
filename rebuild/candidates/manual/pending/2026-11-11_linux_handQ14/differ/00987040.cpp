// CThingFilter_IsAICreatureMatchingTypeAndFactionNearby::operator()(const CThingAICreature& creature) const -- accept an AI creature of our creature type, strictly inside the squared search distance, allied with our faction and (when the angle is restricted) whose normalised offset lies within the cosine of the side vector
// Grade: DIFFER(ours v retail) -- 370v374 on the shared headers (filter header, CThing PeekTC expanded, CTCEnemy, packed C3DVector): the return polarity is fixed (the conditional `!RestrictAngle ? true : offset.Normalise().Dot(SideVec) > MinCosine` gives retail's jne/mov eax,1/xor eax,eax layout with the unrestricted path sharing the true block); left: the normalised-offset products -- retail loads Y and Z from memory and multiplies by the factor in st(1)..st(3), keeping the factor alive to the end (fstp st(0) after the compare), ours copies the factor (fld st(1); fmul [Y]) and consumes it in the Z product.
// Leaf: ??RCThingFilter_IsAICreatureMatchingTypeAndFactionNearby@@QBE_NABVCThingAICreature@@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00987040, length 372 bytes, __thiscall, `ret 4`.
// Callees: CThingCreatureBase::GetCreatureType() const 0x00662000, CThing::GetPos() const 0x004C73D0, the const
//   CVectorMap LowerBound 0x004365B0 (inside CThing::PeekTC<const CTCEnemy>), CTCEnemy::IsAlliedWithFaction 0x0076C7F0.
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CTHING_GETTC_EXPANDED
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CTHINGPHYSICAL
#define FABLE_RETAIL_CTHINGGAMEOBJECT
#define FABLE_RETAIL_CTHINGCREATUREBASE
#define FABLE_RETAIL_CTHINGAICREATURE
#define FABLE_RETAIL_CTHINGCREATUREBASE_CREATURETYPE
#define FABLE_RETAIL_CTCENEMY_QUERIES
#define FABLE_RETAIL_C3DVECTOR
#define FABLE_RETAIL_C3DVECTOR_PACKED
#define FABLE_RETAIL_C3DVECTOR_SQUAREDMAGNITUDE_XYZ
#include "fable_string.h"
#include "stlport/fable_containers.h"
#include "engine/CThing.h"
#include "engine/CThingCreatureBase.h"
#include "engine/CThingAICreature.h"
#include "engine/CTCEnemy.h"
#include "engine/CThingFilter_IsAICreatureMatchingTypeAndFactionNearby.h"

bool CThingFilter_IsAICreatureMatchingTypeAndFactionNearby::operator()(const CThingAICreature& creature) const
{
    if (creature.GetCreatureType() == OurCreatureType) {
        C3DVector offset = creature.GetPos() - OurPos;
        float x2 = offset.X * offset.X;
        float y2 = offset.Y * offset.Y;
        float z2 = offset.Z * offset.Z;
        const CTCEnemy* enemy;
        if (x2 + y2 + z2 < MaxSqDist && creature.PeekTC(TCI_ENEMY, &enemy) && enemy->IsAlliedWithFaction(PFaction))
            return !RestrictAngle ? true : offset.Normalise().Dot(SideVec) > MinCosine;
    }
    return false;
}
