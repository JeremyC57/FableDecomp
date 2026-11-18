// CAIStateGroup_GoToPub::Update handles seating, tavern actions, the departure timer and the transition out.
// Grade: DIFFER(1949v2067). The shared version still emits calls where retail expands list insertion and the component-map search. Pointer temporary slots and the early branch layout also differ.
// Leaf: ?Update@CAIStateGroup_GoToPub@@MAEXXZ
// Flags: none
// Retail: Fable.exe 0x0094AB00, 2067 bytes.

class CTCBase;
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CCLASS
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CINTELLIGENTPOINTER_ASSIGN
#define FABLE_RETAIL_CTHING_GETTC_EXPANDED
#define FABLE_RETAIL_STLP_LIST
#define FABLE_RETAIL_CAISTATEGROUPBASE
#define FABLE_RETAIL_CAISTATEGROUPBASE_MEMBERS
#define FABLE_RETAIL_CAISTATEGROUPBASE_AIACTIONS
#define FABLE_RETAIL_CAISTATEGROUPBASE_MINIONWANDER
#define FABLE_RETAIL_CAISTATEGROUP_GOTOPUB
#define FABLE_RETAIL_CAISTATEGROUP_GOTOPUB_WALKTOSEATANDSIT
#define FABLE_RETAIL_CAISTATEGROUP_GOTOPUB_UPDATE_PASS24
#define FABLE_RETAIL_CAIBRAIN_GETCREATURE
#define FABLE_RETAIL_CAIBRAIN_SETOUTPUT
#define FABLE_RETAIL_CCREATUREACTIONBASE
#define FABLE_RETAIL_CCOMBATACTIONBASE
#define FABLE_RETAIL_CTHINGPHYSICAL
#define FABLE_RETAIL_CTHINGGAMEOBJECT
#define FABLE_RETAIL_CTHINGBUILDING
#define FABLE_RETAIL_CTHINGOBJECT
#define FABLE_RETAIL_CTHINGCREATUREBASE
#define FABLE_RETAIL_CTHINGAICREATURE
#define FABLE_RETAIL_CTHINGCREATUREBASE_PERFORMACTION
#define FABLE_RETAIL_CTHINGCREATUREBASE_ISFREETOPERFORMACTION
#define FABLE_RETAIL_CTCTAVERNTABLE_FREESEAT
#define FABLE_RETAIL_CTCTAVERNTABLE_SEAT
#define FABLE_RETAIL_CTCTAVERNTABLE_SEATS
#define FABLE_RETAIL_CTCTAVERNGAME
#define FABLE_RETAIL_CTCTHINGOWNER
#define FABLE_RETAIL_CTCAISCRATCHPAD_TABLESEAT
#define FABLE_RETAIL_CTCCREATUREMODEMANAGER
#define FABLE_RETAIL_CTCCREATUREMODEMANAGER_ADDREMOVEMODE
#define FABLE_RETAIL_IS_THING_ALIVE
#define FABLE_RETAIL_IS_THING_ALIVE_NOINLINE
#define FABLE_RETAIL_CINTELLIGENTPOINTER_COMPARE_NULL_INT
#define FABLE_RETAIL_CBRAINOUTPUT_ONLY
#define FABLE_RETAIL_CBRAINOUTPUT_ACTION_CALLED_PASS24
#define FABLE_RETAIL_CWORLD
#define FABLE_RETAIL_CWIDESTRING
#define FABLE_RETAIL_CCLASSFACTORY
#define FABLE_RETAIL_STLP_VECTOR
#define FABLE_RETAIL_GFRANDOM_OUTOFLINE
#define FABLE_RETAIL_C3DVECTOR
#define FABLE_RETAIL_CCOUNTEDPOINTER
#include "stlport/fable_containers.h"
#include "engine/CThing.h"
#include "engine/CIntelligentPointer.h"
#include "engine/CVectorMap.h"
#include "engine/C3DVector.h"
#include "engine/CCountedPointer.h"
#include "fable_string.h"

#include "engine/EMovementType.h"
#include "engine/CAIStateGroup_GoToPub.h"
#include "engine/CThingBuilding.h"
#include "engine/CThingObject.h"
#include "engine/CThingAICreature.h"
#include "engine/CAIBrain.h"
#include "engine/CTCThingOwner.h"
#include "engine/CTCTavernTable.h"
#include "engine/CTCTavernTankard.h"
#include "engine/CTCTavernGame.h"
#include "engine/CTCAIScratchpad.h"
#include "engine/CTCCreatureModeManager.h"
#include "engine/CCreatureAction_GetUpFromSitting.h"
#include "engine/ThingAlive.h"
#include "engine/CWorld.h"
#include "engine/CMainGameComponent.h"
#include "engine/GFRandom.h"
#include "engine/campfire_types.h"

void CAIStateGroup_GoToPub::Update()
{
    {
        std::list<CThing*> tables;
        CTCThingOwner* owner;
        if (PPub.Get() && !PPub.Get()->IsDead() && ((CThing*)PPub.Get())->GetTC(TCI_THING_OWNER, &owner)) {
            for (std::list<CIntelligentPointer<CThing> >::iterator it = owner->GetObjectList().begin(); it != owner->GetObjectList().end(); ++it) {
                if ((*it).Get() && !(*it).Get()->IsDead() && (*it).Get()->HasTC(TCI_TAVERN_TABLE))
                    tables.push_back((*it).Get());
            }
            if (!tables.empty()) {
                PTable = 0;
                while (!tables.empty()) {
                    unsigned long index = GFRandom(tables.size(), GetPWorldSeed());
                    std::list<CThing*>::iterator it = tables.begin();
                    for (; it != tables.end(); ++it) {
                        if (index == 0) {
                            PTable = (CThingObject*)*it;
                            break;
                        }
                        --index;
                    }
                    CTCTavernTable* tc_tavern_table;
                    if (PTable.Get()->GetTC(TCI_TAVERN_TABLE, &tc_tavern_table) &&
                        tc_tavern_table->GetSeat(PBrain->GetCreature(), Pos, RHSet, ActionPointIndex, SeatIndex, 0)) {
                        PTankard = (CThing*)tc_tavern_table->GetTankard(SeatIndex);
                        goto found;
                    }
                    tables.erase(it);
                }
                ActionPointIndex = -1;
            }
        }
    }
    Terminate();
    return;
found:
    if (!WalktoSeatAndSit(true)) {
        Terminate();
        return;
    }
    {
        long fps = CMainGameComponent::GetConstantFPS();
        unsigned long extra = GFRandom(100, GetPWorldSeed());
        FrameLeavePub = (long)(CWorld::GetFrame() + ((float)extra + 100.0f) * fps);
    }
    bool to_continue;
    do {
        CTCTavernGame* tc_tavern_game;
        if (((CThing*)PTable.Get())->GetTC(TCI_TAVERN_GAME, &tc_tavern_game) && tc_tavern_game && tc_tavern_game->IsBeingPlayed) {
            if (!HandsOffMode) {
                HandsOffMode = true;
                CTCCreatureModeManager* ptc_mode_manager;
                PBrain->GetCreature().GetTC(TCI_ENTITY_MODE_MANAGER, &ptc_mode_manager);
                ptc_mode_manager->AddMode(0x2d);
            }
            EAIActionReturn ret = AIAction_DoNothing();
            to_continue = ret == (EAIActionReturn)4;
        } else {
            if (HandsOffMode) {
                HandsOffMode = false;
                CTCCreatureModeManager* ptc_mode_manager;
                PBrain->GetCreature().GetTC(TCI_ENTITY_MODE_MANAGER, &ptc_mode_manager);
                ptc_mode_manager->RemoveMode(0x2d);
            }
            to_continue = ChooseAction(true);
        }
        CTCAIScratchpad* tc_ai_scratchpad;
        PBrain->GetCreature().GetTC(TCI_AI_SCRATCHPAD, &tc_ai_scratchpad);
        UnderScriptControl = tc_ai_scratchpad->GetSeatName() != "" && tc_ai_scratchpad->GetTableName() != "";
    } while (IsStaffed() && to_continue && (FrameLeavePub > CWorld::GetFrame() || UnderScriptControl));
    if (!IsStopping() && !HandsOffMode && IS_THING_ALIVE(PTankard)) {
        CTCTavernTankard* tc_tavern_tankard;
        PTankard.Get()->GetTC(TCI_TAVERN_TANKARD, &tc_tavern_tankard);
        while (tc_tavern_tankard->Fullness != 0.0f) {
            if (!Drink()) {
                Terminate();
                return;
            }
        }
    }
    if (Sitting) {
        Sitting = false;
        if (PTable.Get() && !PTable.Get()->IsDead()) {
            CTCTavernTable* tc_tavern_table;
            ((CThing*)PTable.Get())->GetTC(TCI_TAVERN_TABLE, &tc_tavern_table);
            tc_tavern_table->SetSeatState(SeatIndex, false);
        }
        CBrainOutput output;
        CCreatureAction_GetUpFromSitting action(PBrain->GetCreature(), CCharString("SIT_AT_TABLE_OUTOF"), 0, false);
        if (PBrain->GetCreature().IsFreeToPerformAction(action)) {
            output.SetAsPerformAction(action);
            PBrain->SetOutput(output);
        }
        FreeSeat();
        CTCCreatureModeManager* ptc_mode_manager;
        PBrain->GetCreature().GetTC(TCI_ENTITY_MODE_MANAGER, &ptc_mode_manager);
        ptc_mode_manager->RemoveMode(0xb);
    }
    {
        long fps = CMainGameComponent::GetConstantFPS();
        NextFramePub = (long)(CWorld::GetFrame() + fps * 60.0f);
    }
    PayAtBar();
    AIAction_MinionWander(AI_MOVEMENT_WALKING, 0);
    Terminate();
}
