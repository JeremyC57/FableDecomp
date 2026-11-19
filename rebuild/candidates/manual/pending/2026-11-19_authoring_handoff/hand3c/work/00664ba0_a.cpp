// CThingCreatureBase::GetCurrentMeshRadius() const -- the creature def's radius spread interpolated by its hero-stat radius weight for this thing's hero experience; 1 when the thing has no experience component or no creature def
// Leaf: ?GetCurrentMeshRadius@CThingCreatureBase@@QBEMXZ
// Flags: none
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CDEFINITIONMANAGER
#define FABLE_RETAIL_CRESOURCELIST
#define FABLE_RETAIL_CDEFCLASSBASE_GETSUBDEF_NOINLINE
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CTHINGPHYSICAL
#define FABLE_RETAIL_CTHINGGAMEOBJECT
#define FABLE_RETAIL_CTHINGCREATUREBASE
#define FABLE_RETAIL_CTHINGCREATUREBASE_MESHSIZE
#define FABLE_RETAIL_CCREATUREDEF_TRANSFER
#define FABLE_RETAIL_CDEFSTRINGTABLE
#define FABLE_RETAIL_CCREATUREDEF_RESPAWN_PASS24
#define FABLE_RETAIL_CTCHEROEXPERIENCE
#include "fable_string.h"
#include "stlport/fable_containers.h"
#include "engine/CThingCreatureBase.h"
#include "engine/CThingCreatureDef.h"
#include "engine/CCreatureDef.h"
#include "engine/CHeroAbilityStatWeight.h"
#include "engine/CDefClassBase_GetSubDef.h"
#include "engine/CTCHeroExperience.h"

float CThingCreatureBase::GetCurrentMeshRadius() const
{
    const CTCHeroExperience* ptc_hero_experience;
    if (!PeekTC(&ptc_hero_experience))
        return 1.0f;
    CDefPointer<const CCreatureDef> pcreature_def(NULL_DEF);
    if (!PeekDefinition().GetSubDef(&pcreature_def))
        return 1.0f;
    float param = pcreature_def->RadiusWeight.GetParameter(*ptc_hero_experience);
    float rad = pcreature_def->RadiusSpread.LowValue + (pcreature_def->RadiusSpread.HighValue - pcreature_def->RadiusSpread.LowValue) * param;
    return rad;
}
