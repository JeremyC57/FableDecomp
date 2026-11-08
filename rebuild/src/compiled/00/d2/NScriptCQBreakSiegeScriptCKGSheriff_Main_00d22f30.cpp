// NScript::CQ_BreakSiegeScript::CKG_Sheriff::Main() -- Break the Siege, the Knothole Glade sheriff: wait for the bandit demands and the chief's refusal; until the siege starts offer the hero the open-the-gates question and, when hit, heal and ally; on yes run CS_BREAKSIEGE_GATESOPEN(NOMEN) and start the siege; then wait for the bandit leader's escape line and the end cut-scene
// Inline-budget: flag /d2inlS15 -- every implicit MovieBase dtor site (speech / question / cut-scene exit pads): VC7.1's caller IL-size cap (default S9) calls the COMDAT 0x006E7B80 in this 3.4 KB Main, retail expands it; RELOCATION_MATCH for S15, S12/S14 are the default shape
// Leaf: ?Main@CKG_Sheriff@CQ_BreakSiegeScript@NScript@@EAEXXZ
// Flags: /d2inlS15
// Retail: Fable.exe (TLC, VC7.1) @ 0x00D22F30, 3399 bytes, thiscall, plain `ret` (shared epilogue 0x00D23C63 with ~seh_me).
// Identity (proven, two sources): slot 1 (Main) of the CKG_Sheriff entity vtable 0x012CAF1C stored by its landed Alloc 0x00D1DF20
//   (OnPersist 0x00D21010 is slot 4 of the same vtable); Ego_r.pdb and FableWin.pdb ?Main@CKG_Sheriff@CQ_BreakSiegeScript@NScript@@EAEXXZ.
// Model: the landed siblings CDisplacedTrader::Main 0x00D1FD30 (same script) and CEndGameScythe::Main 0x00D2ED20 (cast map, cut-scene macro):
//   ParentClass (engine/CQ_BreakSiegeScript.h, +0x14) flags BanditDemandsKnown +0x4A, ChiefRefusesDemands +0x4B, BanditLeaderFree +0x4C,
//   BanditSiegeStart +0x4D, EndCutsceneDone +0x54. Callees: CScriptThing copy 0x004ABE90 / ~ 0x004AA840, SetPredicate 0x00F35B10,
//   IsActiveThreadTerminating 0x00F35B30, STB IsValid 0x00CD23B9 / Release 0x00CD2770 / ctor 0x007E72A0 (later handles) / dtor 0x007E74D0 /
//   GetScriptThing 0x007E7490 / Speak 0x007E7390 / IsPerformingScriptTask 0x007E7450, CBaseObject 0x0099A380 / = 0x0099A3B0 / ~ 0x0099A430,
//   CCountedPointer<STB>::operator= 0x008AB1E0, CCountedPointer<MovieBase>::~ 0x006E7AB0, map<CCharString,STB> ctor 0x00CDBF70 / ~ 0x00CDBFB0 /
//   [] 0x00CD3D2E, vector<CScriptThing>::~vector 0x008AC970, RunCutsceneMacro 0x00CBFB7D, CCharString 0x0099EBF0 / ~ 0x0099EAE0; Game slots
//   7, 8, 39, 70, 72, 108, 114, 133, 231, 264, 265, 296, 350, 352, 360, 361, 364-366, 370, 371, 376, 379, 475, 517, 526, 599, 655;
//   Me slots 21 MsgIsHitBy, 27 MsgIsTalkedToBy, 41 MsgIsHitBySpecialAbilityFrom, 42 MsgIsHitByAnySpecialAbilityFrom.
// Attempts (STATUS.tsv): the tail `if (T) return; do NSF; while (!T);` is what cross-jumps into retail's entry jump to the loop test.
#define FABLE_RETAIL_CWIDESCREENMAGICPAUSEENTITIES_FORCEINLINE_CTOR
#define FABLE_RETAIL_CSCRIPTGAMERESOURCEOBJECTMOVIEBASE_FORCEINLINE_CTOR
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_NSCRIPT
#define FABLE_RETAIL_CLEVELSCRIPTBASE
#define FABLE_RETAIL_BASE_OBJECT
#define FABLE_RETAIL_STLP_MAP
#define FABLE_RETAIL_CSCRIPTGAMERESOURCEOBJECTSCRIPTEDTHINGBASE_OUTOFLINE_ASSIGN
#define FABLE_RETAIL_CSCRIPTGAMERESOURCEOBJECTSCRIPTEDTHINGBASE_OUTOFLINE_DTOR
#include "fable_string.h"
#include "engine/CCountedPointer.h"
#include "engine/CScriptThing.h"
#include "engine/CGameScriptInterfaceBase.h"
#include "engine/CQ_SunnyvaleMasterData.h"
#include "engine/CEntityScriptBase.h"
#include "engine/CQ_BreakSiegeScript.h"
#include "engine/C3DVector.h"
#include "engine/CScriptGameResourceObjectScriptedThingBase.h"
class CScriptGameResourceObjectMovieBase;
template<> CCountedPointer<CScriptGameResourceObjectMovieBase>::~CCountedPointer();   // 0x006E7AB0
#include "engine/CScriptGameResourceObjectMovieBase.h"
#include "engine/CWideScreenMagicPauseEntities.h"
#include "engine/CIsThingAlive.h"
#include "engine/fable_script_cutscene.h"
enum ETextGroupSelectionMethod { GROUP_SELECT_NONE=0 };
enum EScriptAIPriority { SCRIPT_AI_PRIORITY_LOWEST=0, SCRIPT_AI_PRIORITY_LOW=1, SCRIPT_AI_PRIORITY_MEDIUM=2, SCRIPT_AI_PRIORITY_HIGH=3, SCRIPT_AI_PRIORITY_HIGHEST=4 };
enum EHeroAbility { HERO_ABILITY_NULL=0, HERO_ABILITY_HEAL_LIFE_SPELL=14 };
template<> std::map<CCharString,CScriptGameResourceObjectScriptedThingBase>::map();                 // 0x00CDBF70
template<> std::map<CCharString,CScriptGameResourceObjectScriptedThingBase>::~map();                // 0x00CDBFB0
template<> std::vector<CScriptThing>::~vector();                                                    // 0x008AC970
template<> CCountedPointer<CScriptGameResourceObjectScriptedThingBase>& CCountedPointer<CScriptGameResourceObjectScriptedThingBase>::operator=(const CCountedPointer<CScriptGameResourceObjectScriptedThingBase>& other);   // 0x008AB1E0
__forceinline CScriptGameResourceObjectScriptedThingBase& CScriptGameResourceObjectScriptedThingBase::operator=(const CScriptGameResourceObjectScriptedThingBase& other) { CScriptGameResourceObjectBase::operator=(other); PImp=other.PImp; return *this; }
inline NScript::CIsThingAlive::CIsThingAlive(const CScriptThing& thing) : Thing(thing) { }
inline NScript::CIsThingAlive::~CIsThingAlive() { }

void NScript::CQ_BreakSiegeScript::CKG_Sheriff::Main()
{
    {
        CIsThingAlive alive(Me);
        SetPredicate(alive);
    }
    Game->NewScriptFrame();
    if (IsActiveThreadTerminating())
        return;
    Game->EntitySetAsKillable(Me, false, false);
    Game->EntitySetAsToAddToComboMultiplierWhenHit(Me, false);
    while (!ParentClass.BanditDemandsKnown) {
        Game->NewScriptFrame();
        if (IsActiveThreadTerminating())
            return;
    }
    if (IsActiveThreadTerminating())
        return;
    CScriptGameResourceObjectScriptedThingBase seh_me;
    if (seh_me.IsValid())
        seh_me.Release();
    while (!Game->StartScriptingEntity(Me, seh_me, SCRIPT_AI_PRIORITY_HIGHEST)) {
        Game->NewScriptFrame();
        if (IsActiveThreadTerminating())
            return;
    }
    if (IsActiveThreadTerminating())
        return;
    while (!ParentClass.ChiefRefusesDemands) {
        Game->NewScriptFrame();
        if (IsActiveThreadTerminating())
            return;
    }
    if (IsActiveThreadTerminating())
        return;
    if (!ParentClass.BanditSiegeStart) {
        if (IsActiveThreadTerminating())
            return;
        Game->SetThingHasInformation(Me, false, true, false);
        Game->MiniMapAddMarker(Me, "HUD_ORB_QUEST_OPTIONAL");
    }
    while (!ParentClass.BanditSiegeStart) {
        Game->NewScriptFrame();
        if (IsActiveThreadTerminating())
            return;
        if (Me.MsgIsTalkedToBy("SCRIPT_NAME_HERO")) {
            if (IsActiveThreadTerminating())
                return;
            CScriptGameResourceObjectMovieBase speech_movie;
            Game->StartMovieSequence("", speech_movie);
            CWideScreenMagicPauseEntities PauseMyEntitiesPlease(*Game);
            Game->GiveHeroYesNoQuestion("TEXT_QST_018_GATEGUARD_QUESTION_OPEN_GATES", "TEXT_OBJECT_HERO_ANSWER_YES", "TEXT_OBJECT_HERO_ANSWER_NO", "", true);
            long answer;
            while ((answer = Game->MsgIsQuestionAnsweredYesOrNo()) < 0) {
                Game->NewScriptFrame();
                if (IsActiveThreadTerminating())
                    return;
            }
            if (IsActiveThreadTerminating())
                return;
            if (answer == 1) {
                if (IsActiveThreadTerminating())
                    return;
                break;
            }
        }
        if (Me.MsgIsHitBy("SCRIPT_NAME_HERO") ||
            (Me.MsgIsHitByAnySpecialAbilityFrom("SCRIPT_NAME_HERO") && !Me.MsgIsHitBySpecialAbilityFrom(HERO_ABILITY_HEAL_LIFE_SPELL, "SCRIPT_NAME_HERO"))) {
            if (IsActiveThreadTerminating())
                return;
            CScriptGameResourceObjectMovieBase speech_movie;
            Game->StartMovieSequence("", speech_movie);
            CWideScreenMagicPauseEntities PauseMyEntitiesPlease(*Game);
            if (Game->GetHealth(seh_me.GetScriptThing()) > 0.0f) {
                seh_me.Speak(Game->GetHero(), "TEXT_QST_018_SHERIFF_ON_HIT", (ETextGroupSelectionMethod)0, false, true, false);
                while (seh_me.IsPerformingScriptTask()) {
                    Game->NewScriptFrame();
                    if (IsActiveThreadTerminating())
                        return;
                }
                if (IsActiveThreadTerminating())
                    return;
            }
            Game->ModifyThingHealth(Me, 10000.0f, false);
            Game->EntitySetThingAsAllyOfThing(Me, Game->GetHero());
        }
    }
    if (IsActiveThreadTerminating())
        return;
    if (!ParentClass.BanditSiegeStart) {
        if (IsActiveThreadTerminating())
            return;
        Game->ClearThingHasInformation(Me);
        Game->MiniMapRemoveMarker(Me);
        std::vector<CScriptThing> followers;
        Game->GetFollowingEntityList(Game->GetHero(), followers);
        CScriptGameResourceObjectScriptedThingBase seh_hero;
        CScriptGameResourceObjectScriptedThingBase seh_leader;
        Game->StartScriptingEntity(Game->GetHero(), seh_hero, SCRIPT_AI_PRIORITY_HIGHEST);
        Game->StartScriptingEntity(Game->GetThingWithScriptName("BanditSiegeLeader"), seh_leader, SCRIPT_AI_PRIORITY_HIGHEST);
        std::map<CCharString, CScriptGameResourceObjectScriptedThingBase> mymap;
        mymap["HERO"] = seh_hero;
        mymap["SHERIFF"] = seh_me;
        mymap["LEADER"] = seh_leader;
        CScriptGameResourceObjectMovieBase speech_movie;
        Game->StartMovieSequence("", speech_movie);
        CWideScreenMagicPauseEntities PauseMyEntitiesPlease(*Game);
        Game->FixMovieSequenceCamera(true);
        if (followers.size() != 0) {
            if (IsActiveThreadTerminating())
                return;
            RunCutsceneMacro("CS_BREAKSIEGE_GATESOPEN", mymap, 0, 0, false, true);
        }
        else {
            if (IsActiveThreadTerminating())
                return;
            RunCutsceneMacro("CS_BREAKSIEGE_GATESOPENNOMEN", mymap, 0, 0, false, true);
        }
        Game->FixMovieSequenceCamera(false);
        Game->RemoveThing(Game->GetThingWithScriptName("Blockage"), false, true);
    }
    if (!ParentClass.BanditSiegeStart) {
        if (IsActiveThreadTerminating())
            return;
        Game->SetQuestCardObjective(Game->GetActiveQuestName(), "TEXT_QUEST_BREAK_SIEGE_OBJECTIVE_03", "KnotholeGlade", "KnotholeGlade");
        Game->SetWeaponOutCrimeEnabled(false);
        Game->EntitySetAsKillable(Me, true, true);
        Game->EntitySetAsToAddToComboMultiplierWhenHit(Me, true);
        CScriptThing leader = Game->GetThingWithScriptName("ImprisonedBanditLeader");
        Game->EntitySetThingAsAllyOfThing(Me, leader);
        Game->EntitySetThingAsAllyOfThing(leader, Me);
        ParentClass.BanditSiegeStart = true;
        if (seh_me.IsValid())
            seh_me.Release();
        while (!Game->StartScriptingEntity(Me, seh_me, SCRIPT_AI_PRIORITY_HIGH)) {
            Game->NewScriptFrame();
            if (IsActiveThreadTerminating())
                return;
        }
        if (IsActiveThreadTerminating())
            return;
    }
    while (!ParentClass.EndCutsceneDone) {
        Game->NewScriptFrame();
        if (IsActiveThreadTerminating())
            return;
        if (ParentClass.BanditLeaderFree) {
            if (IsActiveThreadTerminating())
                return;
            Game->Pause(2.0f);
            Game->EntitySetFacingAngleTowardsThing(Me, Game->GetHero(), false);
            long conversation = Game->AddNewConversation(Me, false, false);
            Game->AddPersonToConversation(conversation, Game->GetHero());
            Game->AddLineToConversation(conversation, "TEXT_QST_018_SHERIFF_ESCAPE_1", false, Me, Game->GetHero());
            break;
        }
    }
    seh_me.Release();
    while (!ParentClass.EndCutsceneDone) {
        Game->NewScriptFrame();
        if (IsActiveThreadTerminating())
            return;
    }
    if (IsActiveThreadTerminating())
        return;
    do {
        Game->NewScriptFrame();
    } while (!IsActiveThreadTerminating());
}