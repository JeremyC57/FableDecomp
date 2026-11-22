// CTCDummyVillager::OnSerialise()
// Leaf: ?OnSerialise@CTCDummyVillager@@UAEXAAVCPersistContext@@_N@Z
// Flags: default
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CTHINGPHYSICAL
#define FABLE_RETAIL_CTHINGGAMEOBJECT
#define FABLE_RETAIL_CTHINGCREATUREBASE
#define FABLE_RETAIL_CTHINGAICREATURE
#define FABLE_RETAIL_CTHINGBUILDING
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CINTELLIGENTPOINTER_COMPARE_NULL_INT
#define FABLE_RETAIL_CINTELLIGENTPOINTER_GET_ORIGINAL_CHAIN
#define FABLE_RETAIL_IS_THING_ALIVE
#define FABLE_RETAIL_C3DVECTOR
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CDEFSTRING
#define FABLE_RETAIL_CDEFSTRINGTABLE
#define FABLE_RETAIL_CDEFINITIONMANAGER
#define FABLE_RETAIL_CRESOURCELIST
#define FABLE_RETAIL_CTHINGMANAGER
#define FABLE_RETAIL_CCLASSFACTORY
#define FABLE_RETAIL_CPERSISTCONTEXT
#define FABLE_RETAIL_CPERSISTCONTEXT_NAMED_DEFAULTS
#define FABLE_RETAIL_CPERSISTCONTEXT_GETDEFAULTVAL_BODY
#define FABLE_RETAIL_CPERSISTTRAITS
#define FABLE_RETAIL_CPERSISTTRAITS_UID_DEFAULT_PASS27
#define FABLE_RETAIL_CDATAOUTPUTSTREAM
#include "engine/CThingAICreature.h"
#include "engine/CThingBuilding.h"
#include "engine/CTCDummyVillager.h"
#include "engine/CThingManager.h"
#include "engine/CDefString.h"
#include "engine/CGameDefinitionManager.h"
#include "engine/CDataOutputStream.h"
#include "engine/CPersistTraits.h"
#include "engine/ThingAlive.h"

void CTCDummyVillager::OnSerialise(CPersistContext& context, bool)
{
    if (context.IsSaving()) {
        CCharString def_name = GetDefinitionManager().GetDefNameFromGlobalIndex(DefType);
        context.Transfer("DefType", def_name);
        HomeBuildingUID = 0;
        if (IS_THING_ALIVE(HomeBuilding)) {
            HomeBuildingUID = GetThingManager().RemapUIDForSerialisation(true, GetParentThing().IsLevelLess(),
                GetParentThing().GetCurrentMapNumber(), HomeBuilding->GetUniqueID());
        }
        context.Transfer("HomeBuildingUID", HomeBuildingUID);
        WorkBuildingUID = 0;
        if (IS_THING_ALIVE(WorkBuilding)) {
            WorkBuildingUID = GetThingManager().RemapUIDForSerialisation(true, GetParentThing().IsLevelLess(),
                GetParentThing().GetCurrentMapNumber(), WorkBuilding->GetUniqueID());
        }
        context.Transfer("WorkBuildingUID", WorkBuildingUID);
        if (IS_THING_ALIVE(Creature)) {
            CreatureUID = GetThingManager().RemapUIDForSerialisation(true, GetParentThing().IsLevelLess(),
                GetParentThing().GetCurrentMapNumber(), Creature->GetUniqueID());
        }
        context.Transfer("CreatureUID", CreatureUID);
    } else {
        CCharString def_name;
        context.Transfer("DefType", def_name);
        DefType = GetDefinitionManager().GetDefGlobalIndexFromName(def_name);
        context.Transfer("HomeBuildingUID", HomeBuildingUID, 0ui64);
        if (HomeBuildingUID) {
            HomeBuildingUID = GetThingManager().RemapUIDForSerialisation(false, GetParentThing().IsLevelLess(),
                GetParentThing().GetCurrentMapNumber(), HomeBuildingUID);
        }
        context.Transfer("WorkBuildingUID", WorkBuildingUID, 0ui64);
        if (WorkBuildingUID) {
            WorkBuildingUID = GetThingManager().RemapUIDForSerialisation(false, GetParentThing().IsLevelLess(),
                GetParentThing().GetCurrentMapNumber(), WorkBuildingUID);
        }
        context.Transfer("CreatureUID", CreatureUID, 0ui64);
        if (CreatureUID) {
            CreatureUID = GetThingManager().RemapUIDForSerialisation(false, GetParentThing().IsLevelLess(),
                GetParentThing().GetCurrentMapNumber(), CreatureUID);
        }
    }

    context.Transfer("DayToBeRecreated", DayToBeRecreated);
    context.Transfer("FrameToBeRecreated", FrameToBeRecreated);
    context.Transfer("Respawnable", Respawnable, true);
    context.Transfer("Guard", Guard, false);
}