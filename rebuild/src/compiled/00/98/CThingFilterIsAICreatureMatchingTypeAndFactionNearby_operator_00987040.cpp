#define FABLE_RETAIL_C3DVECTOR_SQUARED_MAGNITUDE_ORIGINAL_EXPR_PASS23
// Original source locals to_vec and block-local ptc_enemy; original helpers replace fitted scalar snapshots.
// Leaf: ??RCThingFilter_IsAICreatureMatchingTypeAndFactionNearby@@QBE_NABVCThingAICreature@@@Z
// Flags: (none)
#define FABLE_RETAIL_C3DVECTOR_NORMALISE_SQUARE_HELPER
#define FABLE_RETAIL_C3DVECTOR_SQUARE_ORIGINAL_FORMAL
#define FABLE_RETAIL_C3DVECTOR_ORIGINAL_COMPONENT_CTOR
#define FABLE_RETAIL_C3DVECTOR_ORIGINAL_COPY_FORMAL
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
    if (creature.GetCreatureType()==OurCreatureType) {
        C3DVector to_vec=creature.GetPos()-OurPos;
        if (to_vec.GetSquaredMagnitude()<MaxSqDist) {
            const CTCEnemy* ptc_enemy;
            if (creature.PeekTC(TCI_ENEMY,&ptc_enemy) && ptc_enemy->IsAlliedWithFaction(PFaction))
                return !RestrictAngle?true:to_vec.Normalise().Dot(SideVec)>MinCosine;
        }
    }
    return false;
}