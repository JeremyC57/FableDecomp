// NScript::CQS_GuardianSisterInfoScript::CMazeAtTavern::Main() -- Maze in the Bowerstone tavern: live while alive, take scripted control, become an opinion source with a quest-core minimap marker and information, not pushable; each frame: wave the hero over once (ST_HELLO + TEXT_QST_027_MAZE_CALL_HERO_OVER_10) when he comes within GSI_MazeCallsOverDistance, face him until the guardian has spoken, answer a hit (not a heal-life spell) with TEXT_QST_027_MAZE_ON_HIT_10 under a paused speech movie and calm down; when talked to cast ME and HERO in CS_GUARDIANSISTER_BOWERSTONE, mark GuardianSpokeToHero, complete and deactivate the quest and remove herself
// Leaf: ?Main@CMazeAtTavern@CQS_GuardianSisterInfoScript@NScript@@EAEXXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00E25F70, 2049 bytes, thiscall, plain `ret`.
// Identity (proven): slot 1 (Main) of the CMazeAtTavern entity vtable stored by the landed Alloc 0x00E25C70 (Init 0x00E25D20,
//   GetParentScript 0x00E25D10, OnPredicateFail 0x00E25D30 landed); Ego_r.pdb ?Main@CMazeAtTavern@CQS_GuardianSisterInfoScript@NScript@@EAEXXZ.
// Callees: CIsThingAlive (vtable 0x012C32F0, dtor 0x00E241B0) + CScriptThing copy 0x004ABE90 / ~ 0x004AA840, SetPredicate
//   0x00F35B10, IsActiveThreadTerminating 0x00F35B30, CBaseObject 0x0099A380, ScriptedThingBase ctor 0x007E72A0 (seh_hero; seh_me's
//   expanded) / operator= 0x008ABD10 / IsValid 0x00CD23B9 / Release 0x00CD2770 / dtor 0x007E74D0 / GetScriptThing 0x007E7490 /
//   Speak 0x007E7390 / PlayAnimation 0x007E73D0 / IsPerformingScriptTask 0x007E7450, MovieBase ctor 0x006E7B60 / dtor 0x006E7B80,
//   the cast map ctor 0x00CDBF70 / dtor 0x00CDBFB0 / operator[] 0x00CD3D2E, RunCutsceneMacro 0x00CBFB7D, IsDistanceBetweenThingsUnder
//   0x00CBE2FF, CCharString 0x0099EBF0 / ~ 0x0099EAE0, pGlobalScriptDef 0x0143E90C (GSI_MazeCallsOverDistance +0xC68),
//   NGlobalConsole::GDefaultToUsingPhysicsInCutsceneAnimations 0x01375748; CGameScriptInterfaceBase slots 7, 8, 70, 72, 108
//   RemoveThing, 264, 281 DeactivateQuestLater, 288 SetQuestAsCompleted, 350 MiniMapAddMarker, 360 / 361, 364-366, 370, 379, 452
//   ClearThingBestEnemyTarget, 475, 622 EntitySetAsOpinionSource, 655 GetActiveQuestName, 844 SetIsPushableByHero (by-value
//   CScriptThing, copy expanded); CScriptThing slots 6 GetPos, 21, 27, 41, 42, 67 SetFriendsWithEverythingFlag.
// Layout: Ego_r.pdb CMazeAtTavern (40 bytes = retail new size): ParentClass +0x14, PMasterData +0x18, TalkCounter +0x1c, WavedOver
//   +0x20, HitCount +0x24 (the landed Init 0x00E25D20 model); parent GuardianSpokeToHero Ego_r +0x5c = retail +0x48.
#define FABLE_RETAIL_CWIDESCREENMAGICPAUSEENTITIES_FORCEINLINE_CTOR
#define FABLE_RETAIL_NSCRIPT
#define FABLE_RETAIL_CLEVELSCRIPTBASE
#define FABLE_RETAIL_BASE_OBJECT
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CDEFSTRINGTABLE
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_STLP_MAP
#define FABLE_RETAIL_CSCRIPTGAMERESOURCEOBJECTSCRIPTEDTHINGBASE_OUTOFLINE_DTOR
#define FABLE_RETAIL_CSCRIPTGAMERESOURCEOBJECTSCRIPTEDTHINGBASE_OUTOFLINE_ASSIGN
#define FABLE_RETAIL_CSCRIPTGAMERESOURCEOBJECTMOVIEBASE_OUTOFLINE_CTOR
#define FABLE_RETAIL_CSCRIPTGAMERESOURCEOBJECTMOVIEBASE_OUTOFLINE_DTOR
#include "fable_string.h"
#include "engine/CCountedPointer.h"
#include "engine/CScriptThing.h"
#include "engine/C3DVector.h"
#include "engine/CQ_SunnyvaleMasterData.h"
#include "engine/CScriptGameResourceObjectScriptedThingBase.h"
#include "engine/CScriptGameResourceObjectMovieBase.h"
#include "engine/CGameScriptInterfaceBase.h"
#include "engine/CEntityScriptBase.h"
#include "engine/CLevelScriptBase.h"
#include "engine/CWideScreenMagicPauseEntities.h"
#include "engine/CIsThingAlive.h"
#include "engine/fable_script_cutscene.h"
#include "engine/fable_script_def_global.h"
#include "engine/fable_script_animation_defaults.h"
enum ETextGroupSelectionMethod { GROUP_SELECT_NONE=0, GROUP_SELECT_RANDOM=1, GROUP_SELECT_RANDOM_NO_REPEAT=2, GROUP_SELECT_SEQUENTIAL=3, GROUP_SELECT_FIRST_ONLY=4 };
enum EScriptAIPriority { SCRIPT_AI_PRIORITY_LOWEST=0, SCRIPT_AI_PRIORITY_LOW=1, SCRIPT_AI_PRIORITY_MEDIUM=2, SCRIPT_AI_PRIORITY_HIGH=3, SCRIPT_AI_PRIORITY_HIGHEST=4 };
enum EHeroAbility { HERO_ABILITY_NULL=0, HERO_ABILITY_HEAL_LIFE_SPELL=14 };
inline NScript::CIsThingAlive::CIsThingAlive(const CScriptThing& thing) : Thing(thing) { }
inline NScript::CIsThingAlive::~CIsThingAlive() { }
inline CScriptThing::CScriptThing(const CScriptThing& other) : CBaseClass(other), PImp(other.PImp) { }
template<> std::map<CCharString,CScriptGameResourceObjectScriptedThingBase>::map();              // 0x00CDBF70
template<> std::map<CCharString,CScriptGameResourceObjectScriptedThingBase>::~map();             // 0x00CDBFB0
bool FABLE_FASTCALL IsDistanceBetweenThingsUnder(const CScriptThing&, const CScriptThing&, float);   // 0x00CBE2FF

namespace NScript {
class CQS_GuardianSisterInfoScript : public CLevelScriptBase<CQ_SunnyvaleMasterData> {
public:
    bool GuardianSpokeToHero;                       // +0x48 (Ego_r +0x5c)
    class CMazeAtTavern;
};
class CQS_GuardianSisterInfoScript::CMazeAtTavern : public CEntityScriptBase {
public:
    CQS_GuardianSisterInfoScript* ParentClass;      // +0x14
    CQ_SunnyvaleMasterData* PMasterData;            // +0x18
    long TalkCounter;                               // +0x1c
    bool WavedOver;                                 // +0x20
    long HitCount;                                  // +0x24
private:
    virtual void Main();
};
}

void NScript::CQS_GuardianSisterInfoScript::CMazeAtTavern::Main()
{
    {
        CIsThingAlive alive(Me);
        SetPredicate(alive);
    }
    Game->NewScriptFrame();
    if (IsActiveThreadTerminating()) return;
    CScriptGameResourceObjectScriptedThingBase seh_me;
    if (seh_me.IsValid()) seh_me.Release();
    while (!Game->StartScriptingEntity(Me, seh_me, SCRIPT_AI_PRIORITY_HIGH)) {
        Game->NewScriptFrame();
        if (IsActiveThreadTerminating()) return;
    }
    if (IsActiveThreadTerminating()) return;
    Game->EntitySetAsOpinionSource(Me, "OPINION_SOURCE_MAZE_BS_PUB");
    C3DVector ExitPos = Game->GetThingWithScriptName("M_MazeExit").GetPos();
    Game->MiniMapAddMarker(Me, "HUD_ORB_QUEST_CORE");
    Game->SetThingHasInformation(Me, false, true, false);
    Game->SetIsPushableByHero(Me, false);
    long TriggerDistance = pGlobalScriptDef->GSI_MazeCallsOverDistance;
    while (!IsActiveThreadTerminating()) {
        if (!ParentClass->GuardianSpokeToHero && !WavedOver && IsDistanceBetweenThingsUnder(Game->GetHero(), Me, (float)TriggerDistance)) {
            if (IsActiveThreadTerminating()) return;
            seh_me.PlayAnimation("ST_HELLO", false, false, false, true, NGlobalConsole::GDefaultToUsingPhysicsInCutsceneAnimations, false, false);
            long HLStyleConversationIndex = Game->AddNewConversation(Me, false, false);
            Game->AddPersonToConversation(HLStyleConversationIndex, Game->GetHero());
            Game->AddLineToConversation(HLStyleConversationIndex, "TEXT_QST_027_MAZE_CALL_HERO_OVER_10", false, Me, Game->GetHero());
            WavedOver = true;
        }
        if (!ParentClass->GuardianSpokeToHero) {
            if (IsActiveThreadTerminating()) return;
            Game->EntitySetFacingAngleTowardsThing(Me, Game->GetHero(), false);
        }
        if (Me.MsgIsHitBy("SCRIPT_NAME_HERO") ||
            (Me.MsgIsHitByAnySpecialAbilityFrom("SCRIPT_NAME_HERO") && !Me.MsgIsHitBySpecialAbilityFrom(HERO_ABILITY_HEAL_LIFE_SPELL, "SCRIPT_NAME_HERO"))) {
            if (IsActiveThreadTerminating()) return;
            if (seh_me.IsValid()) seh_me.Release();
            while (!Game->StartScriptingEntity(Me, seh_me, SCRIPT_AI_PRIORITY_HIGHEST)) {
                Game->NewScriptFrame();
                if (IsActiveThreadTerminating()) return;
            }
            if (IsActiveThreadTerminating()) return;
            CScriptGameResourceObjectMovieBase speech_movie;
            Game->StartMovieSequence("", speech_movie);
            CWideScreenMagicPauseEntities PauseMyEntitiesPlease(*Game);
            if (Game->GetHealth(seh_me.GetScriptThing()) > 0.0f) {
                seh_me.Speak(Game->GetHero(), "TEXT_QST_027_MAZE_ON_HIT_10", GROUP_SELECT_NONE, false, true, false);
                while (seh_me.IsPerformingScriptTask()) {
                    Game->NewScriptFrame();
                    if (IsActiveThreadTerminating()) return;
                }
                if (IsActiveThreadTerminating()) return;
            }
            ++HitCount;
            Me.SetFriendsWithEverythingFlag(true);
            Game->ClearThingBestEnemyTarget(Me);
        }
        if (Me.MsgIsTalkedToBy("SCRIPT_NAME_HERO")) {
            if (IsActiveThreadTerminating()) return;
            if (seh_me.IsValid()) seh_me.Release();
            while (!Game->StartScriptingEntity(Me, seh_me, SCRIPT_AI_PRIORITY_HIGHEST)) {
                Game->NewScriptFrame();
                if (IsActiveThreadTerminating()) return;
            }
            if (IsActiveThreadTerminating()) return;
            {
                CScriptGameResourceObjectScriptedThingBase seh_hero;
                Game->StartScriptingEntity(Game->GetHero(), seh_hero, SCRIPT_AI_PRIORITY_HIGHEST);
                std::map<CCharString,CScriptGameResourceObjectScriptedThingBase> mymap;
                mymap["ME"] = seh_me;
                mymap["HERO"] = seh_hero;
                CScriptGameResourceObjectMovieBase speech_movie;
                Game->StartMovieSequence("", speech_movie);
                CWideScreenMagicPauseEntities PauseMyEntitiesPlease(*Game);
                RunCutsceneMacro("CS_GUARDIANSISTER_BOWERSTONE", mymap, 0, 0, false, true);
            }
            ParentClass->GuardianSpokeToHero = true;
            Game->ClearThingHasInformation(Me);
            Game->SetQuestAsCompleted(Game->GetActiveQuestName(), false, false, false);
            Game->DeactivateQuestLater(Game->GetActiveQuestName(), 0);
            Game->RemoveThing(Me, false, true);
        }
        Game->NewScriptFrame();
    }
}
