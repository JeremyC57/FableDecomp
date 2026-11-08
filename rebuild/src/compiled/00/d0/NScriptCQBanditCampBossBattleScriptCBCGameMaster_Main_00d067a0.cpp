// NScript::CQ_BanditCampBossBattleScript::CBCGameMaster::Main() -- bandit-camp games master in the residential camp: give the hero the camp pass once, then (unless the pub game was already chatted about, the hero holds the pass or gate 2 is open) wait for the hero to talk, introduce the tavern game under a paused movie (guard spoken / not spoken lines) and mark the pub game chatted; idle once the hero has the pass or gate 2 is open; otherwise run the prize table until Spot the Addition is beaten, wait for the result and the hero to leave the game, then say TEXT_QST_009_GAMES_MASTER_WON, hand over the pass (taken from Me) and medium experience, and idle
// Inline-budget: flag /d2inlS15 -- every implicit MovieBase dtor site: VC7.1's caller IL-size cap (default S9) calls the COMDAT 0x006E7B80 in this 2.6 KB Main, retail expands it; RELOCATION_MATCH for S15 and S20, S12/S14 are the default shape
// Leaf: ?Main@CBCGameMaster@CQ_BanditCampBossBattleScript@NScript@@EAEXXZ
// Flags: /d2inlS15
// Retail: Fable.exe (TLC, VC7.1) @ 0x00D067A0, 2628 bytes, plain `ret` (several epilogue copies), frame 0x38.
// Identity: Ego_r.pdb / FableWin.pdb ?Main@CBCGameMaster@CQ_BanditCampBossBattleScript@NScript@@EAEXXZ (private virtual); the single
//   .rdata reference is slot 1 of the CBCGameMaster entity vtable 0x012C8DE4 (slot 0 the landed scalar deleting dtor 0x00D06780, slot 2
//   the landed Init 0x00D06760, slot 3 GetParentScript 0x00D06750, slot 4 OnPersist 0x00D0EE10, slot 5 OnPredicateFail 0x00D06770);
//   was the placeholder Quest009_Gamesmaster_ResidentialBanditCamp_Setup.
// Callees: CIsThingAlive (vtable 0x012C32F0) + CScriptThing copy 0x004ABE90 / ~ 0x004AA840, SetPredicate 0x00F35B10,
//   IsActiveThreadTerminating 0x00F35B30, CBaseObject 0x0099A380 / ~ 0x0099A430, ScriptedThingBase IsValid 0x00CD23B9 / Release
//   0x00CD2770 / GetScriptThing 0x007E7490 / Speak 0x007E7390 / IsPerformingScriptTask 0x007E7450 / dtor 0x007E74D0,
//   CCountedPointer<MovieBase> dtor 0x006E7AB0, CCharString 0x0099EBF0 / ~ 0x0099EAE0; Game slots 7, 8, 70, 121 GiveHeroObject,
//   128 GiveHeroExperience, 184 IsObjectInThingsPossession, 222 GiveThingHeroRewardItem, 264, 360, 361, 370, 379, 586
//   RemoveItemFromContainer, 736 GetBestTimeGuessTheAddition, 737 IsHeroInTavernGame, 864 SetQuitTavernGame, 866 SetPrizeTavernTable,
//   877 GetSpotTheAdditionBeaten; CScriptThing slot 27 MsgIsTalkedToBy; pGlobalScriptDef 0x0143E90C EXPERIENCE_GRANT_MEDIUM +0x40.
// Layout: engine/CQ_BanditCampBossBattleScript.h (FABLE_RETAIL_CQ_BANDITCAMPBOSSBATTLESCRIPT_GAMEMASTER): GivenPass +0x20; parent
//   Gate2Open +0x4c, SpokenToSecondGuard +0x5c, PubGameChatted +0x6d.
#define FABLE_RETAIL_NSCRIPT
#define FABLE_RETAIL_CLEVELSCRIPTBASE
#define FABLE_RETAIL_BASE_OBJECT
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CDEFSTRINGTABLE
#define FABLE_RETAIL_CWIDESCREENMAGICPAUSEENTITIES_FORCEINLINE_CTOR
#define FABLE_RETAIL_CQ_BANDITCAMPBOSSBATTLESCRIPT_GAMEMASTER
#define FABLE_RETAIL_CSCRIPTGAMERESOURCEOBJECTMOVIEBASE_FORCEINLINE_CTOR
#include "fable_string.h"
#include "engine/CQ_BanditCampBossBattleScript.h"
#include "engine/CIsThingAlive.h"
#include "engine/CScriptGameResourceObjectScriptedThingBase.h"
template<> CCountedPointer<CScriptGameResourceObjectMovieBase>::~CCountedPointer();   // 0x006E7AB0
#include "engine/CScriptGameResourceObjectMovieBase.h"
#include "engine/CGameScriptInterfaceBase.h"
#include "engine/CWideScreenMagicPauseEntities.h"
#include "engine/fable_script_distance.h"
#include "engine/fable_script_def_global.h"
enum EScriptAIPriority { SCRIPT_AI_PRIORITY_LOWEST=0, SCRIPT_AI_PRIORITY_LOW=1, SCRIPT_AI_PRIORITY_MEDIUM=2, SCRIPT_AI_PRIORITY_HIGH=3, SCRIPT_AI_PRIORITY_HIGHEST=4 };
enum ETextGroupSelectionMethod { GROUP_SELECT_NONE=0, GROUP_SELECT_RANDOM=1, GROUP_SELECT_RANDOM_NO_REPEAT=2, GROUP_SELECT_SEQUENTIAL=3, GROUP_SELECT_FIRST_ONLY=4 };
inline NScript::CIsThingAlive::CIsThingAlive(const CScriptThing& thing) : Thing(thing) { }
inline NScript::CIsThingAlive::~CIsThingAlive() { }


void NScript::CQ_BanditCampBossBattleScript::CBCGameMaster::Main()
{
    {
        CIsThingAlive filter(Me);
        SetPredicate(filter);
    }
    Game->NewScriptFrame();
    if (IsActiveThreadTerminating()) return;
    CScriptGameResourceObjectScriptedThingBase seh_me;
    if (!GivenPass) {
        if (IsActiveThreadTerminating()) return;
        GivenPass = true;
        Game->GiveThingHeroRewardItem(Me, "OBJECT_RESIDENTIAL_BANDIT_CAMP_PASS", "");
    }
    Game->SetThingHasInformation(Me, false, true, false);
    if (!ParentClass.PubGameChatted && !Game->IsObjectInThingsPossession("OBJECT_RESIDENTIAL_BANDIT_CAMP_PASS", Game->GetHero()) && !ParentClass.Gate2Open) {
        if (IsActiveThreadTerminating()) return;
        if (seh_me.IsValid()) seh_me.Release();
        while (!Game->StartScriptingEntity(Me, seh_me, SCRIPT_AI_PRIORITY_HIGHEST)) {
            Game->NewScriptFrame();
            if (IsActiveThreadTerminating()) return;
        }
        if (IsActiveThreadTerminating()) return;
        while (!Me.MsgIsTalkedToBy("SCRIPT_NAME_HERO") && !Game->IsObjectInThingsPossession("OBJECT_RESIDENTIAL_BANDIT_CAMP_PASS", Game->GetHero()) && !ParentClass.Gate2Open) {
            Game->NewScriptFrame();
            if (IsActiveThreadTerminating()) return;
        }
        if (IsActiveThreadTerminating()) return;
        if (!Game->IsObjectInThingsPossession("OBJECT_RESIDENTIAL_BANDIT_CAMP_PASS", Game->GetHero()) && !ParentClass.Gate2Open) {
            if (IsActiveThreadTerminating()) return;
            {
                CScriptGameResourceObjectMovieBase speech_movie;
                Game->StartMovieSequence("", speech_movie);
                CWideScreenMagicPauseEntities PauseMyEntitiesPlease(*Game);
                if (ParentClass.SpokenToSecondGuard) {
                    if (IsActiveThreadTerminating()) return;
                    if (Game->GetHealth(seh_me.GetScriptThing()) > 0.0f) {
                        seh_me.Speak(Game->GetHero(), "TEXT_QST_009_GAMES_MASTER_INTRO_GUARD_SPOKEN", GROUP_SELECT_NONE, false, true, false);
                        while (seh_me.IsPerformingScriptTask()) {
                            Game->NewScriptFrame();
                            if (IsActiveThreadTerminating()) return;
                        }
                        if (IsActiveThreadTerminating()) return;
                    }
                } else {
                    if (IsActiveThreadTerminating()) return;
                    if (Game->GetHealth(seh_me.GetScriptThing()) > 0.0f) {
                        seh_me.Speak(Game->GetHero(), "TEXT_QST_009_GAMES_MASTER_INTRO_GUARD_NOT_SPOKEN", GROUP_SELECT_NONE, false, true, false);
                        while (seh_me.IsPerformingScriptTask()) {
                            Game->NewScriptFrame();
                            if (IsActiveThreadTerminating()) return;
                        }
                        if (IsActiveThreadTerminating()) return;
                    }
                }
            }
            if (seh_me.IsValid()) seh_me.Release();
            ParentClass.PubGameChatted = true;
        } else {
            if (IsActiveThreadTerminating()) return;
            if (seh_me.IsValid()) seh_me.Release();
            Game->ClearThingHasInformation(Me);
            for (;;) {
                Game->NewScriptFrame();
                if (IsActiveThreadTerminating()) return;
            }
        }
    }
    if (Game->IsObjectInThingsPossession("OBJECT_RESIDENTIAL_BANDIT_CAMP_PASS", Game->GetHero()) || ParentClass.Gate2Open) {
        if (IsActiveThreadTerminating()) return;
        if (seh_me.IsValid()) seh_me.Release();
        Game->ClearThingHasInformation(Me);
        for (;;) {
            Game->NewScriptFrame();
            if (IsActiveThreadTerminating()) return;
        }
    }
    Game->ClearThingHasInformation(Me);
    Game->SetPrizeTavernTable(false);
    while (!Game->GetSpotTheAdditionBeaten()) {
        Game->NewScriptFrame();
        if (IsActiveThreadTerminating()) return;
        if (Game->IsObjectInThingsPossession("OBJECT_RESIDENTIAL_BANDIT_CAMP_PASS", Game->GetHero()) || ParentClass.Gate2Open) {
            if (IsActiveThreadTerminating()) return;
            Game->SetPrizeTavernTable(true);
            if (seh_me.IsValid()) seh_me.Release();
            for (;;) {
                Game->NewScriptFrame();
                if (IsActiveThreadTerminating()) return;
            }
        }
    }
    if (IsActiveThreadTerminating()) return;
    Game->SetPrizeTavernTable(true);
    Game->SetQuitTavernGame(true);
    while (Game->GetBestTimeGuessTheAddition() == 0.0f || Game->IsHeroInTavernGame()) {
        Game->NewScriptFrame();
        if (IsActiveThreadTerminating()) return;
    }
    if (IsActiveThreadTerminating()) return;
    {
        CScriptGameResourceObjectMovieBase speech_movie;
        Game->StartMovieSequence("", speech_movie);
        CWideScreenMagicPauseEntities PauseMyEntitiesPlease(*Game);
        if (seh_me.IsValid()) seh_me.Release();
        while (!Game->StartScriptingEntity(Me, seh_me, SCRIPT_AI_PRIORITY_HIGHEST)) {
            Game->NewScriptFrame();
            if (IsActiveThreadTerminating()) return;
        }
        if (IsActiveThreadTerminating()) return;
        if (Game->GetHealth(seh_me.GetScriptThing()) > 0.0f) {
            seh_me.Speak(Game->GetHero(), "TEXT_QST_009_GAMES_MASTER_WON", GROUP_SELECT_NONE, false, true, false);
            while (seh_me.IsPerformingScriptTask()) {
                Game->NewScriptFrame();
                if (IsActiveThreadTerminating()) return;
            }
            if (IsActiveThreadTerminating()) return;
        }
        Game->GiveHeroObject("OBJECT_RESIDENTIAL_BANDIT_CAMP_PASS", -1, false);
        Game->RemoveItemFromContainer(Me, "OBJECT_RESIDENTIAL_BANDIT_CAMP_PASS");
        Game->GiveHeroExperience(pGlobalScriptDef->EXPERIENCE_GRANT_MEDIUM);
        if (seh_me.IsValid()) seh_me.Release();
    }
    for (;;) {
        Game->NewScriptFrame();
        if (IsActiveThreadTerminating()) return;
    }
}