// CThingCreatureBase::GetCurrentMeshRadius() const -- the creature def's radius spread interpolated by its hero-stat radius weight for this thing's hero experience; 1 when the thing has no experience component or no creature def
// Leaf: ?GetCurrentMeshRadius@CThingCreatureBase@@QBEMXZ
// Flags: none
// Retail: Fable.exe (TLC, VC7.1) @ 0x00664BA0, 184 bytes, `ret`, float in st0.
// Names: Ego_d.pdb locals ptc_hero_experience / pcreature_def / param / rad; FableWin 0x01D999E0 shows the statement order:
//   PeekTC<const CTCHeroExperience>, CDefPointer<const CCreatureDef>(NULL_DEF), PeekDefinition().GetSubDef<CCreatureDef>, GetParameter,
//   then the three `->` reads of RadiusSpread (Low + (High - Low) * param). The debug-only DoErrorMessage arms are compiled out.
// Shape: the shared headers. GetSubDef<CCreatureDef> (out of line at 0x00485815) has its body visible in the TU but stays noinline
//   (FABLE_RETAIL_CDEFCLASSBASE_GETSUBDEF_NOINLINE). The visible body is what turns the parked failure-path release into retail's
//   `dec [ecx+4] ; jne` and shares the key / out-pointer frame word.
// Header note: CCreatureDef.h under FABLE_RETAIL_CCREATUREDEF_TRANSFER needs FABLE_RETAIL_CDEFSTRINGTABLE, or VC7.1 spins forever
//   (bisected). RESPAWN_PASS24 carries StatGetTypeName.
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
