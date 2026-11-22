// 0072f750 CTCDummyVillager::CreateVillager
// Leaf: ?CreateVillager@CTCDummyVillager@@QAEPAVCThingAICreature@@AAVCTCVillage@@ABVC3DVector@@_N@Z
// Flags:
#define FABLE_RETAIL_CREATE_VILLAGER_PASS27
#define FABLE_RETAIL_CDEFINITIONMANAGER
#define FABLE_RETAIL_CDEFCLASSBASE_GETSUBDEF_BODY

#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CTHING_ORIGINAL_VTABLE
#define FABLE_RETAIL_CTHINGPHYSICAL
#define FABLE_RETAIL_CTHINGGAMEOBJECT
#define FABLE_RETAIL_CTHINGCREATUREBASE
#define FABLE_RETAIL_CTHINGAICREATURE
#define FABLE_RETAIL_CTHINGBUILDING
#define FABLE_RETAIL_CTCBUYABLEHOUSE
#define FABLE_RETAIL_CTHING_PBASEDEF
#define FABLE_RETAIL_CTHINGBASEDEF
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CINTELLIGENTPOINTER_DEREFERENCE
#define FABLE_RETAIL_CINTELLIGENTPOINTER_GET_ORIGINAL_CHAIN
#define FABLE_RETAIL_CINTELLIGENTPOINTER_COMPARE_NULL_INT
#define FABLE_RETAIL_CCLASS
#define FABLE_RETAIL_BASE_OBJECT
#define FABLE_RETAIL_CRESOURCELIST
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_C3DVECTOR
#define FABLE_RETAIL_CTHING_GETTC_EXPANDED
#define FABLE_RETAIL_CTCVILLAGE
#define FABLE_RETAIL_CTHINGAICREATUREINIT_CTOR_BODY
#define FABLE_RETAIL_CINTELLIGENTPOINTER_ASSIGN
#define FABLE_RETAIL_IS_THING_ALIVE
#include "engine/CThingAICreature.h"
#include "engine/CThingBuilding.h"
#include "engine/CThingBaseDef.h"
#include "engine/CTCDummyVillager.h"
#include "engine/CTCBuyableHouse.h"
#include "engine/CTCVillage.h"
#include "engine/CDefinitionManager.h"
#include "engine/CVillagePeopleDef.h"
#include "engine/CThingAICreatureInit.h"
#include "engine/ThingAlive.h"
#include <limits.h>

CThingAICreature* CTCDummyVillager::CreateVillager(CTCVillage& village,const C3DVector& pos,bool stop_autoplace)
{
    if (Guard) DefType=village.GetNextGuardTypeAndIncrement();
    CThingAICreatureInit init;
    init.HomeBuilding=HomeBuilding.Get();
    init.WorkBuilding=WorkBuilding.Get();
    init.EnableCreatureAutoPlacing=!stop_autoplace;
    Creature=CThingAICreature::Create(DefType,pos,4,init);
    village.AddThingToVillage(*Creature);
    Creature->SetAsAllowedToFollowHero(true);
    float prob_of_gay_per_dialogue_enabled_man=0.0f;
    CDefPointer<const CVillagePeopleDef> pvillage_people_def;
    if (village.GetParentThing().PeekBaseDef().GetSubDef(&pvillage_people_def))
        prob_of_gay_per_dialogue_enabled_man=pvillage_people_def->ProbabilityOfHomsexualityPerDialogueEnabledMan;
    if (prob_of_gay_per_dialogue_enabled_man>0 && !(village.GetNumberOfGayVillagers()>0))
        prob_of_gay_per_dialogue_enabled_man=1.0f;
    Creature->AutoSetupSexuality(prob_of_gay_per_dialogue_enabled_man);
    CTCBuyableHouse* buyable_house;
    if (IS_THING_ALIVE(HomeBuilding) && HomeBuilding->GetTC(&buyable_house))
        buyable_house->MovedInNotification(DayToBeRecreated);
    if (IS_THING_ALIVE(WorkBuilding) && WorkBuilding->GetTC(&buyable_house))
        buyable_house->MovedInNotification(DayToBeRecreated);
    DayToBeRecreated=INT_MAX;
    FrameToBeRecreated=INT_MAX;
    return Creature.Get();
}