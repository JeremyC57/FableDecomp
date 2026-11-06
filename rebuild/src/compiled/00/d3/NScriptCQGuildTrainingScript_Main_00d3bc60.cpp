// NScript::CQ_GuildTrainingScript::Main() -- the Guild training quest main: on a fresh start take the hero's weapons, teleport him to the guild arrival point, wait for Lookout Point, give the melee training quest card and start screen, run the arrival cutscene and fade out; then teleport him to the training start point, wait for the Heroes' Guild, bind the fourteen training entity scripts, switch off quest-card guidance and weapon-out crime, make the guild guards ignore crimes and spawn the tutorial / friendly-attack / Whisper / sparrow / bookcase threads
// Leaf: ?Main@CQ_GuildTrainingScript@NScript@@UAEXXZ
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_NSCRIPT
#define FABLE_RETAIL_SUSPENDABLE_PROCESS
#define FABLE_RETAIL_BASE_OBJECT
#define FABLE_RETAIL_CLEVELSCRIPTBASE
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CDEFSTRINGTABLE
#define FABLE_RETAIL_CQ_GUILDTRAININGSCRIPT_MAIN
#define FABLE_RETAIL_CENTITYSCRIPTBINDING_FORCEINLINE_CTOR
#include "stlport/fable_containers.h"
#include "fable_string.h"
#include "engine/CCountedPointer.h"
#include "engine/CScriptThing.h"
#include "engine/CGameScriptInterfaceBase.h"
#include "engine/CQ_SunnyvaleMasterData.h"
#include "engine/CEntityScriptBase.h"
#include "engine/CQ_GuildTrainingScript.h"
#include "engine/CEntityScriptBinding.h"
#include "engine/CSpawnedFuncBase.h"
#include "engine/CSpawnedFunc.h"
#include "engine/CRGBColour.h"

namespace NScript {
#define FABLE_GT_ENTITY(Name) \
    class CQ_GuildTrainingScript::Name : public CEntityScriptBase { \
    public: \
        static CCountedPointer<CEntityScriptBase> FABLE_FASTCALL Alloc(CQ_GuildTrainingScript&, CScriptDataBase*, const CScriptThing&); \
    };
FABLE_GT_ENTITY(CAppleGirl)                  // Alloc 0x00D3D060
FABLE_GT_ENTITY(CApprenticeSpeedTest)        // Alloc 0x00D3E1F0
FABLE_GT_ENTITY(CSpeedFriend)                // Alloc 0x00D407C0
FABLE_GT_ENTITY(CRaceMarker)                 // Alloc 0x00D409E0
FABLE_GT_ENTITY(CMeleeApprentice)            // Alloc 0x00D40C10
FABLE_GT_ENTITY(CCombatApprentice)           // Alloc 0x00D41A30
FABLE_GT_ENTITY(CSkillApprentice)            // Alloc 0x00D41B10
FABLE_GT_ENTITY(CWillApprentice)             // Alloc 0x00D43280
FABLE_GT_ENTITY(CHeroBed)                    // Alloc 0x00D40B30
FABLE_GT_ENTITY(CSkillTarget)                // Alloc 0x00D41BF0
FABLE_GT_ENTITY(CBirdKiller)                 // Alloc 0x00D42F40
FABLE_GT_ENTITY(CKillBird)                   // Alloc 0x00D43040
FABLE_GT_ENTITY(CPreMeleeMaze)               // Alloc 0x00D43CD0
#undef FABLE_GT_ENTITY
}

inline CScriptThing::~CScriptThing() { }

void NScript::CQ_GuildTrainingScript::Main()
{
    if (GameState == 0) {
        if (IsActiveThreadTerminating())
            return;
        Game->RemoveAllHeroWeapons();
        Game->EntityTeleportToThing(Game->GetHero(), Game->GetThingWithScriptName("GuildArrivalHSP"), false);
        while (!Game->IsLevelLoaded("LookoutPoint")) {
            Game->NewScriptFrame();
            if (IsActiveThreadTerminating())
                return;
        }
        if (IsActiveThreadTerminating())
            return;
        Game->GiveHeroQuestCardDirectly("OBJECT_QUEST_CARD_TRAINING_MELEE", "Q_GuildTraining", false);
        Game->NewScriptFrame();
        if (IsActiveThreadTerminating())
            return;
        Game->KickOffQuestStartScreen(Game->GetActiveQuestName(), false, true);
        RunArrivalCutscene();
        Game->FadeScreenOut(0.5f, 0.0f, CRGBColour(0, 0, 0, 255));
        Game->NewScriptFrame();
        if (IsActiveThreadTerminating())
            return;
    }
    Game->EntityTeleportToThing(Game->GetHero(), Game->GetThingWithScriptName("GuildTrainingHSP"), false);
    while (!Game->IsLevelLoaded("HeroGuildComplex")) {
        Game->NewScriptFrame();
        if (IsActiveThreadTerminating())
            return;
    }
    if (IsActiveThreadTerminating())
        return;
    Game->SetAllowScreenFadingOnNextRegionChange(true);
    Game->SetRegionTextDisplayAsActive(true);
    AddEntityScriptBinding(new CEntityScriptBinding<CQ_GuildTrainingScript>("AppleGirl", this, &CAppleGirl::Alloc, 1));
    AddEntityScriptBinding(new CEntityScriptBinding<CQ_GuildTrainingScript>("ApprenticeSpeedTest", this, &CApprenticeSpeedTest::Alloc, 1));
    AddEntityScriptBinding(new CEntityScriptBinding<CQ_GuildTrainingScript>("SpeedFriend", this, &CSpeedFriend::Alloc, 1));
    AddEntityScriptBinding(new CEntityScriptBinding<CQ_GuildTrainingScript>("RaceMarker", this, &CRaceMarker::Alloc, 1));
    AddEntityScriptBinding(new CEntityScriptBinding<CQ_GuildTrainingScript>("MeleeApprentice", this, &CMeleeApprentice::Alloc, 1));
    AddEntityScriptBinding(new CEntityScriptBinding<CQ_GuildTrainingScript>("CombatApprentice", this, &CCombatApprentice::Alloc, 1));
    AddEntityScriptBinding(new CEntityScriptBinding<CQ_GuildTrainingScript>("SkillApprentice", this, &CSkillApprentice::Alloc, 1));
    AddEntityScriptBinding(new CEntityScriptBinding<CQ_GuildTrainingScript>("WillApprentice", this, &CWillApprentice::Alloc, 1));
    AddEntityScriptBinding(new CEntityScriptBinding<CQ_GuildTrainingScript>("HeroBed", this, &CHeroBed::Alloc, 1));
    AddEntityScriptBinding(new CEntityScriptBinding<CQ_GuildTrainingScript>("SkillTarget", this, &CSkillTarget::Alloc, 1));
    AddEntityScriptBinding(new CEntityScriptBinding<CQ_GuildTrainingScript>("WillDummy", this, &CWillDummy::Alloc, 1));
    AddEntityScriptBinding(new CEntityScriptBinding<CQ_GuildTrainingScript>("BirdKiller", this, &CBirdKiller::Alloc, 1));
    AddEntityScriptBinding(new CEntityScriptBinding<CQ_GuildTrainingScript>("KillBird", this, &CKillBird::Alloc, 1));
    AddEntityScriptBinding(new CEntityScriptBinding<CQ_GuildTrainingScript>("PreMeleeMaze", this, &CPreMeleeMaze::Alloc, 1));
    PostAddScriptedEntities();
    Game->PostAddScriptedEntities();
    Game->SetHeroGuideToShowQuestCardsWhenSpokenTo(false);
    Game->SetWeaponOutCrimeEnabled(false);
    Game->SetGuardsIgnoreCrimes(true);
    CScriptThing village = Game->GetNearestWithDefName(Game->GetHero(), "VILLAGE_GUILD_COMPLEX_INSIDE");
    Game->EnableGuards(village, false);
    AddSpawnedFunction(new CSpawnedFunc<CQ_GuildTrainingScript>("RunTutorials", 0, this, &CQ_GuildTrainingScript::RunTutorials), CCharString(""));
    AddSpawnedFunction(new CSpawnedFunc<CQ_GuildTrainingScript>("CheckFriendlyAttacks", 0, this, &CQ_GuildTrainingScript::CheckFriendlyAttacks), CCharString(""));
    AddSpawnedFunction(new CSpawnedFunc<CQ_GuildTrainingScript>("KeepTabsOnWhisper", 0, this, &CQ_GuildTrainingScript::KeepTabsOnWhisper), CCharString(""));
    AddSpawnedFunction(new CSpawnedFunc<CQ_GuildTrainingScript>("WatchForSparrowKilled", 0, this, &CQ_GuildTrainingScript::WatchForSparrowKilled), CCharString(""));
    AddSpawnedFunction(new CSpawnedFunc<CQ_GuildTrainingScript>("KeepBookcaseExitRemoved", 0, this, &CQ_GuildTrainingScript::KeepBookcaseExitRemoved), CCharString(""));
}