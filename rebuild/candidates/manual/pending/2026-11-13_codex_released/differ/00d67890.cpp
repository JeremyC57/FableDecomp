// NScript::CQ_GuildTrainingWoodsWillScript::Main() -- the guild-woods Will training quest main: bind WillWhisper, wait for GuildWoods, set objective 6 option 2, spawn WatchForTermination and DoMission, give the kill-bandits quest card, take scripted control of Whisper, the bandits and the hero for the intro cutscene, set every bandit on the hero at 15 health, then until the bandits are dead re-collect them each frame and heal the hero with Whisper's line and the strafe hint when he runs low; finally play the outro and succeed
// Grade: DIFFER(3472v3426) -- the resize(n, value) of the bandit handle vector: retail expands the plain resize(n) with the temporary's ctor after the size; ours expands it only with an explicit temporary, and VC7.1 then colours registers differently from entry (retail keeps 0 in edi and 1 in ebx; the conditional-temporary flag home sits 4 bytes higher). Bisect: plain resize(n) inlines in a mini TU and after one preceding wait, not after two (an inline-frequency effect of the preceding waits)
// Next idea: find the spelling of the waits before the resize that keeps the resize(n) site cold enough to expand (retail inlines the plain resize(n))
// Leaf: ?Main@CQ_GuildTrainingWoodsWillScript@NScript@@UAEXXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00D67890, 3426 bytes; slot 2 of the level-script vtable 0x012D1E10 (1 RegisterMain 0x00D677D0).
#define FABLE_RETAIL_CWIDESCREENMAGICPAUSEENTITIES_FORCEINLINE_CTOR
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_NSCRIPT
#define FABLE_RETAIL_SUSPENDABLE_PROCESS
#define FABLE_RETAIL_BASE_OBJECT
#define FABLE_RETAIL_STLP_MAP
#define FABLE_RETAIL_CDEFSTRINGTABLE
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_STLP_VECTOR_RESIZE_OUTOFLINE
#define FABLE_RETAIL_CSCRIPTGAMERESOURCEOBJECTSCRIPTEDTHINGBASE_OUTOFLINE_DTOR
#define FABLE_RETAIL_CSCRIPTGAMERESOURCEOBJECTSCRIPTEDTHINGBASE_OUTOFLINE_ASSIGN
#define FABLE_RETAIL_CSCRIPTGAMERESOURCEOBJECTMOVIEBASE_OUTOFLINE_CTOR
#define FABLE_RETAIL_CSCRIPTGAMERESOURCEOBJECTMOVIEBASE_OUTOFLINE_DTOR
#include "fable_string.h"
#include "engine/CCountedPointer.h"
#include "engine/C3DVector.h"
#include "engine/CScriptThing.h"
#include "engine/CGameScriptInterfaceBase.h"
#include "engine/CQ_SunnyvaleMasterData.h"
#include "engine/CEntityScriptBase.h"
#include "engine/CQ_GuildTrainingWoodsWillScript.h"
#include "engine/CEntityScriptBinding.h"
#include "engine/CSpawnedFuncBase.h"
#include "engine/CSpawnedFunc.h"
#include "engine/CScriptGameResourceObjectScriptedThingBase.h"
#include "engine/CScriptGameResourceObjectMovieBase.h"
#include "engine/CWideScreenMagicPauseEntities.h"
#include "engine/fable_script_cutscene.h"
#include "engine/CScriptDef.h"
#include "engine/CDefPointer.h"
enum EScriptAIPriority { SCRIPT_AI_PRIORITY_LOWEST=0, SCRIPT_AI_PRIORITY_LOW=1, SCRIPT_AI_PRIORITY_MEDIUM=2, SCRIPT_AI_PRIORITY_HIGH=3, SCRIPT_AI_PRIORITY_HIGHEST=4 };
extern CDefPointer<const CScriptDef> pGlobalScriptDef;   // 0x0143E90C

void NScript::CQ_GuildTrainingWoodsWillScript::Main()
{
    MissionSucceeded = false;
    MissionFailed = false;
    MissionOver = false;
    WhisperAnimate = false;
    BanditsAlive = true;
    AddEntityScriptBinding(new CEntityScriptBinding<CQ_GuildTrainingWoodsWillScript>("WillWhisper", this, &CWillWhisper::Alloc, 1));
    PostAddScriptedEntities();
    Game->PostAddScriptedEntities();
    while (!Game->IsLevelLoaded("GuildWoods")) {
        Game->NewScriptFrame();
        if (IsActiveThreadTerminating())
            return;
    }
    if (IsActiveThreadTerminating())
        return;
    Game->SetQuestCardObjective("Q_GuildTrainingWoodsWill", "TEXT_QUEST_GUILD_TRAINING_OBJECTIVE_06_OPTION_02", "", "");
    AddSpawnedFunction(new CSpawnedFunc<CQ_GuildTrainingWoodsWillScript>("WatchForTermination", 0, this, &CQ_GuildTrainingWoodsWillScript::WatchForTermination), CCharString(""));
    AddSpawnedFunction(new CSpawnedFunc<CQ_GuildTrainingWoodsWillScript>("DoMission", 0, this, &CQ_GuildTrainingWoodsWillScript::DoMission), CCharString(""));
    Game->GiveHeroQuestCardDirectly("OBJECT_QUEST_CARD_TRAINING_KILL_BANDITS", "Q_GuildTrainingWoodsWill", false);
    CScriptThing whisper = Game->GetThingWithScriptName("WillWhisper");
    CScriptGameResourceObjectScriptedThingBase seh_whisper;
    if (seh_whisper.IsValid())
        seh_whisper.Release();
    while (!Game->StartScriptingEntity(whisper, seh_whisper, SCRIPT_AI_PRIORITY_HIGHEST)) {
        Game->NewScriptFrame();
        if (IsActiveThreadTerminating())
            return;
    }
    if (IsActiveThreadTerminating())
        return;
    std::vector<CScriptThing> bandits;
    Game->GetAllThingsWithScriptName("WillBandit", bandits);
    {
        std::vector<CScriptGameResourceObjectScriptedThingBase> seh_bandits;
        unsigned long count = bandits.size();
        seh_bandits.resize(count, CScriptGameResourceObjectScriptedThingBase());
        for (unsigned long i = 0; i < bandits.size(); ++i)
            Game->StartScriptingEntity(bandits[i], seh_bandits[i], SCRIPT_AI_PRIORITY_HIGHEST);
        CScriptGameResourceObjectScriptedThingBase seh_hero;
        if (seh_hero.IsValid())
            seh_hero.Release();
        while (!Game->StartScriptingEntity(Game->GetHero(), seh_hero, SCRIPT_AI_PRIORITY_HIGHEST)) {
            Game->NewScriptFrame();
            if (IsActiveThreadTerminating())
                return;
        }
        if (IsActiveThreadTerminating())
            return;
        std::map<CCharString,CScriptGameResourceObjectScriptedThingBase> mymap;
        mymap["HERO"] = seh_hero;
        mymap["BAN1"] = seh_bandits[0];
        mymap["BAN2"] = seh_bandits[1];
        mymap["BAN3"] = seh_bandits[2];
        mymap["WHISPER"] = seh_whisper;
        CScriptGameResourceObjectMovieBase intro_movie;
        Game->StartMovieSequence("", intro_movie);
        CWideScreenMagicPauseEntities PauseMyEntitiesPlease(*Game);
        Game->FixMovieSequenceCamera(true);
        RunCutsceneMacro("CS_GUILD_WILL_WOODS_INTRO", mymap, 0, 0, false, true);
        Game->FixMovieSequenceCamera(false);
    }
    for (unsigned long i = 0; i < bandits.size(); ++i) {
        if (IsActiveThreadTerminating())
            return;
        Game->GiveThingBestEnemyTarget(bandits[i], Game->GetHero());
        Game->ModifyThingHealth(bandits[i], 15.0f - Game->GetHealth(bandits[i]), false);
        bandits[i].SetToKillOnLevelUnload(false);
    }
    if (IsActiveThreadTerminating())
        return;
    if (seh_whisper.IsValid())
        seh_whisper.Release();
    while (BanditsAlive) {
        Game->NewScriptFrame();
        if (IsActiveThreadTerminating())
            return;
        if (!Game->IsLevelLoaded("GuildWoods")) {
            if (IsActiveThreadTerminating())
                return;
            while (!Game->IsLevelLoaded("GuildWoods")) {
                Game->NewScriptFrame();
                if (IsActiveThreadTerminating())
                    return;
            }
            if (IsActiveThreadTerminating())
                return;
        }
        while (!bandits.empty())
            bandits.pop_back();
        Game->GetAllThingsWithScriptName("WillBandit", bandits);
        if (bandits.size() == 0) {
            if (IsActiveThreadTerminating())
                return;
            BanditsAlive = false;
        }
        if (Game->GetHealth(Game->GetHero()) < pGlobalScriptDef->GUI_MinHealth) {
            if (IsActiveThreadTerminating())
                return;
            long conversation = Game->AddNewConversation(whisper, false, false);
            Game->AddPersonToConversation(conversation, Game->GetHero());
            Game->AddLineToConversation(conversation, "TEXT_QST_028_TEEN_WHISPER_SKILL_WOODS_HEALTH", false, whisper, Game->GetHero());
            if (Game->IsXbox()) {
                if (IsActiveThreadTerminating())
                    return;
                Game->DisplayGameInfo("TEXT_QST_028_ONSCREENHELP_STRAFE_HELP");
                while (!Game->MsgIsGameInfoClickedPast()) {
                    Game->NewScriptFrame();
                    if (IsActiveThreadTerminating())
                        return;
                }
            } else {
                if (IsActiveThreadTerminating())
                    return;
                Game->DisplayGameInfo("TEXT_QST_028_ONSCREENHELP_STRAFE_HELP_PC");
                while (!Game->MsgIsGameInfoClickedPast()) {
                    Game->NewScriptFrame();
                    if (IsActiveThreadTerminating())
                        return;
                }
            }
            if (IsActiveThreadTerminating())
                return;
            WhisperAnimate = true;
            Game->ChangeHeroHealthBy(1000.0f, true, false);
        }
    }
    if (IsActiveThreadTerminating())
        return;
    if (seh_whisper.IsValid())
        seh_whisper.Release();
    while (!Game->StartScriptingEntity(whisper, seh_whisper, SCRIPT_AI_PRIORITY_HIGHEST)) {
        Game->NewScriptFrame();
        if (IsActiveThreadTerminating())
            return;
    }
    if (IsActiveThreadTerminating())
        return;
    {
        CScriptGameResourceObjectScriptedThingBase seh_hero;
        if (seh_hero.IsValid())
            seh_hero.Release();
        while (!Game->StartScriptingEntity(Game->GetHero(), seh_hero, SCRIPT_AI_PRIORITY_HIGHEST)) {
            Game->NewScriptFrame();
            if (IsActiveThreadTerminating())
                return;
        }
        if (IsActiveThreadTerminating())
            return;
        std::map<CCharString,CScriptGameResourceObjectScriptedThingBase> mymap;
        mymap["HERO"] = seh_hero;
        mymap["WHISPER"] = seh_whisper;
        CScriptGameResourceObjectMovieBase outro_movie;
        Game->StartMovieSequence("", outro_movie);
        CWideScreenMagicPauseEntities PauseMyEntitiesPlease(*Game);
        Game->FixMovieSequenceCamera(true);
        RunCutsceneMacro("CS_GUILD_WILL_WOODS_OUTRO", mymap, 0, 0, false, true);
        Game->FixMovieSequenceCamera(false);
    }
    MissionSucceeded = true;
}
