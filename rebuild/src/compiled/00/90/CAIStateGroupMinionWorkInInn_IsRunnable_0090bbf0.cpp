// CAIStateGroup_MinionWorkInInn::IsRunnable() -- test whether the creature can work in its inn.
// Retail: 0x0090bbf0,100 bytes,thiscall.
// Base compilation: /O2 /Oy /W3
// Leaf: ?IsRunnable@CAIStateGroup_MinionWorkInInn@@UAE_NXZ
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CCLASS
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CTHINGPHYSICAL
#define FABLE_RETAIL_CTHINGGAMEOBJECT
#define FABLE_RETAIL_CTHINGCREATUREBASE
#define FABLE_RETAIL_CTHINGAICREATURE
#define FABLE_RETAIL_CTHINGAICREATURE_HOMEBUILDING
#define FABLE_RETAIL_CTHINGAICREATURE_SAVE
#define FABLE_RETAIL_CTHINGAICREATURE_WORKBUILDING
#define FABLE_RETAIL_CTHINGAICREATURE_PEEKWORKBUILDING
#define FABLE_RETAIL_CTHINGBUILDING_PEEKSIMDEF
#define FABLE_RETAIL_CSIMBUILDINGDEF
#define FABLE_RETAIL_CAISTATEGROUPBASE
#define FABLE_RETAIL_CAISTATEGROUPBASE_MEMBERS
#define FABLE_RETAIL_CAISTATEGROUPBASE_GETCREATURE
#define FABLE_RETAIL_CAIBRAIN_CREATURE_REFERENCE
#define FABLE_RETAIL_CAIBRAIN_GETCREATURE
#define FABLE_RETAIL_CDEFPOINTER_RELEASE_SELFDELETING_POINTEE
#define FABLE_RETAIL_CIVCOUNTEDPOINTEEBASE_RELEASE_DELETES
#include "engine/CSimBuildingDef.h"
#include "engine/CThingAICreature.h"
#include "engine/CThingBuilding.h"
#include "engine/CAIStateGroup_MinionWorkInInn.h"
#include "engine/CAIBrain.h"
inline const CThingBuilding* CThingAICreature::PeekWorkBuilding() const
{
    return WorkBuilding.Get();
}
bool CAIStateGroup_MinionWorkInInn::IsRunnable()
{
    const CThingBuilding* pwork_building=GetCreature().PeekWorkBuilding();
    if(pwork_building && GetCreature().HasTC(TCI_VILLAGE_MEMBER)) {
        CDefPointer<const CSimBuildingDef> sim_building_def=pwork_building->PeekPSimDefinition();
        if(sim_building_def.Get())
            return (sim_building_def->Property & 0x20)!=0;
    }
    return false;
}