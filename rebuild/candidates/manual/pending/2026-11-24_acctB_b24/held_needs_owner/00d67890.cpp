// NScript::CQ_GuildTrainingWoodsWillScript::Main() -- the guild-woods Will training quest main: reset the flags, bind WillWhisper, wait for GuildWoods, set objective 6 option 2, spawn WatchForTermination and DoMission, give the kill-bandits quest card, take scripted control of Whisper, the bandits and the hero for the intro cutscene, set every bandit on the hero at 15 health, then until the bandits are dead re-collect them each frame and heal the hero with Whisper's line and the strafe hint when he runs low; finally play the outro and succeed
// Grade: RELOCATION_MATCH (B24A11, stock flags)
// Inline-budget: explicit specialization of the one-argument vector<CScriptGameResourceObjectScriptedThingBase>::resize with STLport's own
//   body (`resize(n, _Tp())`), FABLE_FORCEINLINE under #pragma inline_depth(0) (the landed CSwordCrowd::Main 0x00ED5F50 / staged 0x00ED8680
//   thread-ctor device): retail EXPANDS resize(n) at 0x00D67C82 (size() first, then the handle temporary's ctor 0x007E72A0 CALLED, the
//   two-argument resize 0x00CD3CCB called, dtor 0x007E74D0 called). c2 trace (a11/inl): plain inline resize(n) sz 66 / c1 58 at f .0625
//   (sc 182, called); with inline_depth(0) only c1 36 (sc 113, called); forced without inline_depth(0) the nested ctor expands (3472).
//   Per-row flags do not fit (/d2inlT114 3478, /d2inlT183 3590). FABLE_RETAIL_STLP_VECTOR_RESIZE_VALUE_DECL_ONLY: the two-argument
//   resize declared only (retail calls 0x00CD3CCB; no COMDAT copy emitted).
// Shape (debug build, FableWin 0x03772EA0 + Ego_d.pdb): InitialiseVariables() first (FableWin 0x03774DC0; expanded in retail, header
//   inline member); locals Whisper, AllBandits, seh_Whisper, _Fail_Val (TRetVoid; 25 TRetVoid::Return exits); block {BanditHandles,
//   seh_Hero, mymap, speech_movie, PauseMyEntitiesPlease, {long i}}; block {long i}; block {long HLStyleConversationIndex}; outro block
//   {seh_Hero, mymap, speech_movie, PauseMyEntitiesPlease}; both i loops open with `xor ecx,ecx ; je ; jmp` = `if (false) {} else for`;
//   BanditHandles.resize(AllBandits.size()) one-argument (FableWin calls ?resize@...@QAEXI@Z).
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
#define FABLE_RETAIL_PHASE1_ARCHER_PASS26
#define FABLE_RETAIL_STLP_VECTOR_RESIZE_VALUE_DECL_ONLY
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
#include "engine/fable_script_def_global.h"
#include "engine/EScriptAIPriority.h"
#include "engine/TRetVoid.h"

#ifdef _MSC_VER
#pragma inline_depth(0)
#endif
template<> FABLE_FORCEINLINE void std::vector<CScriptGameResourceObjectScriptedThingBase>::resize(size_type n) { resize(n, CScriptGameResourceObjectScriptedThingBase()); }
#ifdef _MSC_VER
#pragma inline_depth()
#endif
void NScript::CQ_GuildTrainingWoodsWillScript::Main()
{
    TRetVoid _Fail_Val;
    InitialiseVariables();
    AddEntityScriptBinding(new CEntityScriptBinding<CQ_GuildTrainingWoodsWillScript>("WillWhisper", this, &CWillWhisper::Alloc, 1));
    PostAddScriptedEntities();
    Game->PostAddScriptedEntities();
    while (!Game->IsLevelLoaded("GuildWoods")) {
        Game->NewScriptFrame();
        if (IsActiveThreadTerminating())
            return _Fail_Val.Return();
    }
    if (IsActiveThreadTerminating())
        return _Fail_Val.Return();
    Game->SetQuestCardObjective("Q_GuildTrainingWoodsWill", "TEXT_QUEST_GUILD_TRAINING_OBJECTIVE_06_OPTION_02", "", "");
    AddSpawnedFunction(new CSpawnedFunc<CQ_GuildTrainingWoodsWillScript>("WatchForTermination", 0, this, &CQ_GuildTrainingWoodsWillScript::WatchForTermination), CCharString(""));
    AddSpawnedFunction(new CSpawnedFunc<CQ_GuildTrainingWoodsWillScript>("DoMission", 0, this, &CQ_GuildTrainingWoodsWillScript::DoMission), CCharString(""));
    Game->GiveHeroQuestCardDirectly("OBJECT_QUEST_CARD_TRAINING_KILL_BANDITS", "Q_GuildTrainingWoodsWill", false);
    CScriptThing Whisper = Game->GetThingWithScriptName("WillWhisper");
    CScriptGameResourceObjectScriptedThingBase seh_Whisper;
    if (seh_Whisper.IsValid())
        seh_Whisper.Release();
    while (!Game->StartScriptingEntity(Whisper, seh_Whisper, SCRIPT_AI_PRIORITY_HIGHEST)) {
        Game->NewScriptFrame();
        if (IsActiveThreadTerminating())
            return _Fail_Val.Return();
    }
    if (IsActiveThreadTerminating())
        return _Fail_Val.Return();
    std::vector<CScriptThing> AllBandits;
    Game->GetAllThingsWithScriptName("WillBandit", AllBandits);
    {
        std::vector<CScriptGameResourceObjectScriptedThingBase> BanditHandles;
        BanditHandles.resize(AllBandits.size());
        if (false) {} else for (long i = 0; i < AllBandits.size(); i++)
            Game->StartScriptingEntity(AllBandits[i], BanditHandles[i], SCRIPT_AI_PRIORITY_HIGHEST);
        CScriptGameResourceObjectScriptedThingBase seh_Hero;
        if (seh_Hero.IsValid())
            seh_Hero.Release();
        while (!Game->StartScriptingEntity(Game->GetHero(), seh_Hero, SCRIPT_AI_PRIORITY_HIGHEST)) {
            Game->NewScriptFrame();
            if (IsActiveThreadTerminating())
                return _Fail_Val.Return();
        }
        if (IsActiveThreadTerminating())
            return _Fail_Val.Return();
        std::map<CCharString,CScriptGameResourceObjectScriptedThingBase> mymap;
        mymap["HERO"] = seh_Hero;
        mymap["BAN1"] = BanditHandles[0];
        mymap["BAN2"] = BanditHandles[1];
        mymap["BAN3"] = BanditHandles[2];
        mymap["WHISPER"] = seh_Whisper;
        CScriptGameResourceObjectMovieBase speech_movie;
        Game->StartMovieSequence("", speech_movie);
        CWideScreenMagicPauseEntities PauseMyEntitiesPlease(*Game);
        Game->FixMovieSequenceCamera(true);
        RunCutsceneMacro("CS_GUILD_WILL_WOODS_INTRO", mymap, 0, 0, false, true);
        Game->FixMovieSequenceCamera(false);
    }
    if (false) {} else for (long i = 0; i < AllBandits.size(); i++) {
        if (IsActiveThreadTerminating())
            return _Fail_Val.Return();
        Game->GiveThingBestEnemyTarget(AllBandits[i], Game->GetHero());
        Game->ModifyThingHealth(AllBandits[i], 15.0f - Game->GetHealth(AllBandits[i]), false);
        AllBandits[i].SetToKillOnLevelUnload(false);
    }
    if (IsActiveThreadTerminating())
        return _Fail_Val.Return();
    if (seh_Whisper.IsValid())
        seh_Whisper.Release();
    while (BanditsAlive) {
        Game->NewScriptFrame();
        if (IsActiveThreadTerminating())
            return _Fail_Val.Return();
        if (!Game->IsLevelLoaded("GuildWoods")) {
            if (IsActiveThreadTerminating())
                return _Fail_Val.Return();
            while (!Game->IsLevelLoaded("GuildWoods")) {
                Game->NewScriptFrame();
                if (IsActiveThreadTerminating())
                    return _Fail_Val.Return();
            }
            if (IsActiveThreadTerminating())
                return _Fail_Val.Return();
        }
        while (!AllBandits.empty())
            AllBandits.pop_back();
        Game->GetAllThingsWithScriptName("WillBandit", AllBandits);
        if (AllBandits.size() == 0) {
            if (IsActiveThreadTerminating())
                return _Fail_Val.Return();
            BanditsAlive = false;
        }
        if (Game->GetHealth(Game->GetHero()) < pGlobalScriptDef->GUI_MinHealth) {
            if (IsActiveThreadTerminating())
                return _Fail_Val.Return();
            long HLStyleConversationIndex = Game->AddNewConversation(Whisper, false, false);
            Game->AddPersonToConversation(HLStyleConversationIndex, Game->GetHero());
            Game->AddLineToConversation(HLStyleConversationIndex, "TEXT_QST_028_TEEN_WHISPER_SKILL_WOODS_HEALTH", false, Whisper, Game->GetHero());
            if (Game->IsXbox()) {
                if (IsActiveThreadTerminating())
                    return _Fail_Val.Return();
                Game->DisplayGameInfo("TEXT_QST_028_ONSCREENHELP_STRAFE_HELP");
                while (!Game->MsgIsGameInfoClickedPast()) {
                    Game->NewScriptFrame();
                    if (IsActiveThreadTerminating())
                        return _Fail_Val.Return();
                }
            } else {
                if (IsActiveThreadTerminating())
                    return _Fail_Val.Return();
                Game->DisplayGameInfo("TEXT_QST_028_ONSCREENHELP_STRAFE_HELP_PC");
                while (!Game->MsgIsGameInfoClickedPast()) {
                    Game->NewScriptFrame();
                    if (IsActiveThreadTerminating())
                        return _Fail_Val.Return();
                }
            }
            if (IsActiveThreadTerminating())
                return _Fail_Val.Return();
            WhisperAnimate = true;
            Game->ChangeHeroHealthBy(1000.0f, true, false);
        }
    }
    if (IsActiveThreadTerminating())
        return _Fail_Val.Return();
    if (seh_Whisper.IsValid())
        seh_Whisper.Release();
    while (!Game->StartScriptingEntity(Whisper, seh_Whisper, SCRIPT_AI_PRIORITY_HIGHEST)) {
        Game->NewScriptFrame();
        if (IsActiveThreadTerminating())
            return _Fail_Val.Return();
    }
    if (IsActiveThreadTerminating())
        return _Fail_Val.Return();
    {
        CScriptGameResourceObjectScriptedThingBase seh_Hero;
        if (seh_Hero.IsValid())
            seh_Hero.Release();
        while (!Game->StartScriptingEntity(Game->GetHero(), seh_Hero, SCRIPT_AI_PRIORITY_HIGHEST)) {
            Game->NewScriptFrame();
            if (IsActiveThreadTerminating())
                return _Fail_Val.Return();
        }
        if (IsActiveThreadTerminating())
            return _Fail_Val.Return();
        std::map<CCharString,CScriptGameResourceObjectScriptedThingBase> mymap;
        mymap["HERO"] = seh_Hero;
        mymap["WHISPER"] = seh_Whisper;
        CScriptGameResourceObjectMovieBase speech_movie;
        Game->StartMovieSequence("", speech_movie);
        CWideScreenMagicPauseEntities PauseMyEntitiesPlease(*Game);
        Game->FixMovieSequenceCamera(true);
        RunCutsceneMacro("CS_GUILD_WILL_WOODS_OUTRO", mymap, 0, 0, false, true);
        Game->FixMovieSequenceCamera(false);
    }
    MissionSucceeded = true;
}
