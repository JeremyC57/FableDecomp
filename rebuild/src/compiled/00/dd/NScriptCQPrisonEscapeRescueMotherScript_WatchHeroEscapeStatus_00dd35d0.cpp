// NScript::CQ_PrisonEscapeRescueMotherScript::WatchHeroEscapeStatus() -- monitor inventory recovery, reset the torture chamber and fail on the mother's death.
// Leaf: ?WatchHeroEscapeStatus@CQ_PrisonEscapeRescueMotherScript@NScript@@QAEXXZ
#define FABLE_RETAIL_NSCRIPT
#define FABLE_RETAIL_CLEVELSCRIPTBASE
#define FABLE_RETAIL_BASE_OBJECT
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CWIDESTRING
#include "engine/CCountedPointer.h"
#include "engine/CScriptThing.h"
#include "engine/CQ_PrisonEscapeRescueMotherScript.h"
#include "engine/CGameScriptInterfaceBase.h"

inline const C3DVector& CScriptThing::GetPos() const
{
    if (!PImp.Data) return NullCoord;
    return PImp.Data->GetPos();
}

void NScript::CQ_PrisonEscapeRescueMotherScript::WatchHeroEscapeStatus()
{
    bool TortureChamberReset=false;
    while (!((MasterScriptData->PrisonKeyStolenByHero && Game->IsRegionLoaded("PrisonCourtyard")) || MotherRescuedFromPrisonAutoSaveCheckPoint)) {
        Game->NewScriptFrame();
        if (IsActiveThreadTerminating()) return;
    }
    if (IsActiveThreadTerminating()) return;
    for (;;) {
        if (IsActiveThreadTerminating()) return;
        if (!HeroHasFoundWeapons && Game->IsObjectInThingsPossession("OBJECT_HERO_EQUIPMENT_BAG",Game->GetHero())) {
            if (IsActiveThreadTerminating()) return;
            HeroHasFoundWeapons=true;
            Game->TakeObjectFromHero("OBJECT_HERO_EQUIPMENT_BAG");
            Game->ReturnAllConfiscatedItemsToHero();
            Game->SetHeroWillAsUsable(true);
            Game->SetQuestCardObjective(Game->GetActiveQuestName(),"TEXT_QUEST_PRISON_ESCAPE_OBJECTIVE_04","PrisonTortureChamber","");
            CScriptThing jump_guard_pos=Game->GetThingWithScriptName("M_JumpGuard");
            CScriptThing jump_guard=Game->CreateCreatureOnEntity("CREATURE_PRISON_GUARD",jump_guard_pos,"PrisonEscapeGuard");
            Game->NewScriptFrame();
            if (IsActiveThreadTerminating()) return;
            long HLStyleConversationIndex=Game->AddNewConversation(jump_guard,false,false);
            Game->AddLineToConversation(HLStyleConversationIndex,"TEXT_QST_055_GUARD_SPOTS_HERO",false,jump_guard,Game->GetHero());
            while (!Game->IsLevelLoaded("PrisonCourtyard")) {
                Game->NewScriptFrame();
                if (IsActiveThreadTerminating()) return;
            }
            if (IsActiveThreadTerminating()) return;
        }
        if (Game->IsLevelLoaded("PrisonTortureChamber") && !TortureChamberReset) {
            if (IsActiveThreadTerminating()) return;
            const CScriptThing& MotherInCell=Game->GetThingWithScriptName("MotherTorture");
            CScriptThing MotherSpawnPos=Game->GetThingWithScriptName("Q_MOTHER_MARKER_ONE");
            if (!MotherInCell.IsAlive()) {
                if (IsActiveThreadTerminating()) return;
                Game->CreateCreature("CREATURE_SCARLET_ROBE",MotherSpawnPos.GetPos(),"MotherTorture",false);
            }
            TortureChamberReset=true;
        }
        if (MotherDied) break;
        Game->NewScriptFrame();
    }
    if (IsActiveThreadTerminating()) return;
    Game->SetQuestAsFailed(Game->GetActiveQuestName(),true,L"TEXT_QST_055_QUEST_FAILED_MOTHER_DIED",false);
    for (;;) {
        Game->NewScriptFrame();
        if (IsActiveThreadTerminating()) return;
    }
}