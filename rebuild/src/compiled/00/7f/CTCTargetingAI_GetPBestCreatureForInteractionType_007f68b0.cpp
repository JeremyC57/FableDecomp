// CTCTargetingAI::GetPBestCreatureForInteractionType(ECreatureInteractionType) const -- find the nearest eligible creature in the current sight area.
// Leaf: ?GetPBestCreatureForInteractionType@CTCTargetingAI@@UBEPAVCThingCreatureBase@@W4ECreatureInteractionType@@@Z
// Retail: 0x007F68B0, 255 bytes, virtual const thiscall.
#include "fable_compiler.h"
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_BASE_OBJECT
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_STLP_LIST
#define FABLE_RETAIL_CTCTARGETINGAI_ATTACK
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_C3DVECTOR
#define FABLE_RETAIL_CTHINGSEARCHTOOLS
#define FABLE_RETAIL_CTHINGSEARCHTOOLS_BESTROUGH
#define FABLE_RETAIL_CTHINGSEARCHTOOLS_BESTROUGH_BODY
#define FABLE_RETAIL_CTCMAPWHO
#define FABLE_RETAIL_CTCMAPWHO_SEARCHID
#define FABLE_RETAIL_CWORLDMAP
#define FABLE_RETAIL_CTHING_STATTYPEFLAGS
#define FABLE_RETAIL_C2DBOXI_RADIUS_INITIALIZERS
#define FABLE_RETAIL_CTCPERCEIVETHING
#define FABLE_RETAIL_CTCPERCEIVETHING_GETSIGHTRADIUS
#define FABLE_RETAIL_CTHING_GETTC_EXPANDED
#define FABLE_RETAIL_C2DBOXI_CTORS
#define FABLE_RETAIL_C2DBOXI
#define FABLE_RETAIL_CTHINGPHYSICAL
#define FABLE_RETAIL_CTHINGGAMEOBJECT
#define FABLE_RETAIL_CTHINGCREATUREBASE
#define FABLE_RETAIL_CTHINGAICREATURE
#define FABLE_RETAIL_CCLASS
#define FABLE_RETAIL_STLP_REMOVE_COPY_IF
#define FABLE_RETAIL_STLP_NOT1
#include "fable_string.h"
#include "engine/C3DVector.h"
#include "engine/C2DBoxI.h"
#include "engine/CThing.h"
#include "engine/CThingAICreature.h"
#include "engine/CTargetingDef.h"
#include "engine/CTCTargetingAI.h"
#include "engine/CWorldMap.h"
#include "engine/CTCMapwho.h"
#include "engine/NThingMap_CThingMap.h"
#include "engine/CThingSearchTools.h"
#include "engine/CThingCompare_Nearest.h"
#define FABLE_RETAIL_TARGETINGINTERACTION_FILTERS_PASS24
#include "engine/TargetingInteractionFilters_pass24.h"
#define FABLE_RETAIL_FILTER_BIND3_FUNCTION
#include "engine/CFilter_Bind3.h"
#include "engine/CTCPerceiveThing.h"

CThingCreatureBase* CTCTargetingAI::GetPBestCreatureForInteractionType(ECreatureInteractionType interaction_type) const
{
    const CThingAICreature& parent_creature = static_cast<const CThingAICreature&>(GetParentThing());
    const C3DVector& pos = parent_creature.GetPos();
    const CTCPerceiveThing* ptc_perceive;
    parent_creature.PeekTC(&ptc_perceive);
    CThingAICreature* thing_creature = 0;
    C2DBoxI area(pos.ToC2DCoordI(), (long)ptc_perceive->GetSightRadius());
    PeekThingSearchTools().GetPBestThingInRoughArea(area,
        GFFilter_Bind3(CThingFilter_IsAvailableForCreatureInteraction(parent_creature, interaction_type),
                      CThingFilter_IsAICreatureAwareOf(&parent_creature), std::not1(CThingFilter_IsIncapacitated())),
        CThingCompare_Nearest(pos), &thing_creature);
    return thing_creature;
}