// NScript::CQ_AmbushTradersScript::CAT_BanditAlly::Main() -- the hireable Ambush Traders bandit: wait for the bandit intro, wander round his post; if already hired and following, restart his speech-marker thread, health bar and hero-follow; then each frame: teleport to the quest end point in Bowerstone Jail, leave Bowerstone Posh, switch allegiance after the hero's third attack, warn when attacked, nag the hero back from an invalid region, walk off and fade out when the quest ends, play the follow / waiting / hire-offer cut scenes when talked to, obey the FOLLOW / WAIT expressions, and walk out of the region once the traders escaped
// Leaf: ?Main@CAT_BanditAlly@CQ_AmbushTradersScript@NScript@@EAEXXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00F063B0, 5651 bytes, thiscall, no EH frame, one epilogue.
// Identity (proven, two sources): the address appears once in .rdata, at slot 1 of the entity vtable 0x012F7D88 stored by
//   CAT_BanditAlly::Alloc 0x00F037A0 (slot 0 the landed scalar deleting dtor 0x00F03870, slot 2 the landed Init 0x00F05F30, slot 4 the
//   landed OnPersist 0x00F0A8E0); Ego_r.pdb and FableWin.pdb both hold ?Main@CAT_BanditAlly@CQ_AmbushTradersScript@NScript@@EAEXXZ.
// Callees: CCharString 0x0099E4B0 / 0x0099EBF0 / ~ 0x0099EAE0, operator+ 0x0099F570, CBasicString<char>::Compare 0x00411570,
//   SetPredicate 0x00F35B10 with the CIsThingAlive temporary (vtable 0x012C32F0, ctor inline, dtor 0x00E241B0), IsActiveThreadTerminating
//   0x00F35B30, CScriptThing copy 0x004ABE90 / ~ 0x004AA840 / IsAlive 0x004AB130, the scripted-thing handle (CBaseObject 0x0099A380,
//   IsValid 0x00CD23B9, Release 0x00CD2770, MoveToPosition 0x007E72F0, Speak 0x007E7390, IsPerformingScriptTask 0x007E7450,
//   GetScriptThing 0x007E7490, ~ 0x007E74D0), MovieBase ctor 0x006E7B60 / dtor 0x006E7B80, CSpawnedFuncBase 0x00CDD450,
//   AddSpawnedFunction 0x00CB7E50, rand 0x00BFEB16, IsDistanceBetweenThingsUnder 0x00CBE2FF, IsDistanceFromThingToPositionOver
//   0x00CBE45C, FadeOutAndKillEntity 0x00CBE716; parent AddBanditHealthBar 0x00F07A80, AskBanditHireQuestion 0x00F07ED0,
//   BanditHireSetup 0x00F081E0, WatchForBanditSpeechMarkers 0x00F07B80 (the CSpawnedFunc1 vtable 0x012F7EBC); own
//   SwitchBanditAllegience 0x00F079D0.
// Data: ParentClass (+0x14) QuestFailed +0x48, QuestFinished +0x49, RegionInvalid +0x4c, BanditIntroDone +0x4d, TraderEscaped +0x4e,
//   NoEnemiesLeft +0x4f, PhaseComplete +0x50, SpyContactMade +0x5f, ActivatedPhase3 +0x66, BanditAttackedByHero +0x90,
//   BanditAttackByHeroCount +0x94, BanditSwitchAllegienceComplete +0x98; own members from the CAT_BanditAlly opt-in;
//   pGlobalScriptDef AmbushTradersBanditWanderMax +0xa04, AmbushTradersRenown +0xa0c, AmbushTradersInvalidRegionTimerMax +0xa5c /
//   Var +0xa60.
// Shape: generated entity script (the CAT_ConvoyMember 0x00F0CAA0 model): a T-check before every statement, WAIT_UNTIL loops,
//   `for (;;) { ...; NewScriptFrame(); T-return; }` main loop; the expression name is a function-scope CCharString.
#include "fable_compiler.h"
#define FABLE_RETAIL_CWIDESCREENMAGICPAUSEENTITIES_FORCEINLINE_CTOR
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_NSCRIPT
#define FABLE_RETAIL_BASE_OBJECT
#define FABLE_RETAIL_CLEVELSCRIPTBASE
#define FABLE_RETAIL_SUSPENDABLE_PROCESS
#define FABLE_RETAIL_CDEFSTRINGTABLE
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CCHARSTRING_FORCEINLINE_TEXT_EQ
#define FABLE_RETAIL_CBASICSTRING_TEXT_EQ_INLINE
#define FABLE_RETAIL_CSCRIPTGAMERESOURCEOBJECTSCRIPTEDTHINGBASE_OUTOFLINE_DTOR
#define FABLE_RETAIL_CSCRIPTGAMERESOURCEOBJECTSCRIPTEDTHINGBASE_OUTOFLINE_ASSIGN
#define FABLE_RETAIL_CSCRIPTGAMERESOURCEOBJECTMOVIEBASE_OUTOFLINE_CTOR
#define FABLE_RETAIL_CSCRIPTGAMERESOURCEOBJECTMOVIEBASE_OUTOFLINE_DTOR
#define FABLE_RETAIL_CQ_AMBUSHTRADERSSCRIPT_MAIN
#define FABLE_RETAIL_CQ_AMBUSHTRADERSSCRIPT_BANDITALLY
#include "fable_string.h"
#include "engine/CCountedPointer.h"
#include "engine/C3DVector.h"
#include "engine/CScriptThing.h"
#include "engine/CGameScriptInterfaceBase.h"
#include "engine/CQ_SunnyvaleMasterData.h"
#include "engine/CEntityScriptBase.h"
#include "stlport/fable_containers.h"
#include "engine/CQ_AmbushTradersScript.h"
#include "engine/CScriptGameResourceObjectScriptedThingBase.h"
#include "engine/CScriptGameResourceObjectMovieBase.h"
#include "engine/CWideScreenMagicPauseEntities.h"
#include "engine/CSpawnedFunc1.h"
#include "engine/CIsThingAlive.h"
#include "engine/CScriptDef.h"
#include "engine/CDefPointer.h"
#include "engine/CTimer.h"
#include "engine/TRetVoid.h"
#include "engine/fable_script_distance.h"
#include "engine/fable_script_fade.h"
#include "engine/fable_script_text.h"
enum EScriptAIPriority { SCRIPT_AI_PRIORITY_LOWEST=0, SCRIPT_AI_PRIORITY_LOW=1, SCRIPT_AI_PRIORITY_MEDIUM=2, SCRIPT_AI_PRIORITY_HIGH=3, SCRIPT_AI_PRIORITY_HIGHEST=4 };
enum ETextGroupSelectionMethod { GROUP_SELECT_NONE=0, GROUP_SELECT_RANDOM=1, GROUP_SELECT_RANDOM_NO_REPEAT=2, GROUP_SELECT_SEQUENTIAL=3, GROUP_SELECT_FIRST_ONLY=4 };
enum EScriptEntityMoveType { ENTITY_MOVE_WALK=0, ENTITY_MOVE_RUN=1, ENTITY_MOVE_SNEAK=2 };
enum EScriptingStateGroups { SCRIPTING_STATE_GROUP_NONE=0, SCRIPTING_STATE_GROUP_WANDER=4 };
extern "C" int __cdecl rand();
bool FABLE_FASTCALL IsDistanceFromThingToPositionOver(const CScriptThing& thing, const C3DVector& pos, float distance);   // 0x00CBE45C
extern CGameScriptInterfaceBase* pDSTGame;
extern CDefPointer<const CScriptDef> pGlobalScriptDef;
FABLE_FORCEINLINE CTimer& CTimer::operator=(long time) { pDSTGame->SetTimer(TimerIndex, time); return *this; }
FABLE_FORCEINLINE long CTimer::GetTime() const { return pDSTGame->GetTimer(TimerIndex); }
inline const C3DVector& CScriptThing::GetPos() const { if (PImp.IsNull()) return NullCoord; return PImp->GetPos(); }
inline NScript::CIsThingAlive::CIsThingAlive(const CScriptThing& thing):Thing(thing) {}
// The body of CBasicString<char>::Compare 0x00411570 (the landed 0x00411570 loop; chunk 807 lesson): same-TU and not inlined, as in
// retail, so VC7.1 knows the call leaves the string alone and keeps `expression`'s data pointer in esi across the compare chain.
template<> inline __declspec(noinline) long __fastcall CBasicString<char>::Compare(const char* a, const char* b)
{
    for (;;) {
        char ca = *a;
        if (ca == 0 && *b == 0)
            return 0;
        char cb = *b;
        if (ca < cb)
            return -1;
        if (ca > cb)
            return 1;
        ++a;
        ++b;
    }
}

void NScript::CQ_AmbushTradersScript::CAT_BanditAlly::Main()
{
    TRetVoid _Fail_Val;
    CCharString expression;
    if (ParentClass.BanditSwitchAllegienceComplete) {
        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
        return;
    }
    SetPredicate(CIsThingAlive(Me));
    Game->NewScriptFrame();
    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
    while (!ParentClass.BanditIntroDone) {
        Game->NewScriptFrame();
        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
    }
    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
    CScriptGameResourceObjectScriptedThingBase seh_me;
    Game->SetWanderCentrePoint(Me, Me.GetPos());
    Game->SetWanderMinDistance(Me, 0.0f);
    Game->SetWanderMaxDistance(Me, pGlobalScriptDef->AmbushTradersBanditWanderMax);
    Game->SetScriptingStateGroup(Me, (EScriptingStateGroups)4);
    if (BanditHired && BanditFollowing) {
        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
        ParentClass.AddSpawnedFunction(new CSpawnedFunc1<CQ_AmbushTradersScript,CScriptThing>(CCharString("ParentClass.")+CCharString("WatchForBanditSpeechMarkers"),0,&ParentClass,&CQ_AmbushTradersScript::WatchForBanditSpeechMarkers,Me),CCharString(""));
        BanditHealthBarID = ParentClass.AddBanditHealthBar(Me);
        Game->EntitySetAsAllowedToFollowHero(Me, true);
        Game->EntityFollowThing(Me, Game->GetHero(), 1.0f, true);
        Game->SetEntityAsRegionFollowing(Game->GetHero(), Me, true);
    }
    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
    for (;;) {
        if (ParentClass.ActivatedPhase3 && Game->IsRegionLoaded("BowerstoneJail") && !BanditHired && !BanditTeleported) {
            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
            Game->EntityTeleportToThing(Me, Game->GetThingWithScriptName("M_AT_QuestEndPoint"), false);
            BanditTeleported = true;
        }
        else if (Game->IsRegionLoaded("BowerstonePosh") && BanditHired && BanditFollowing && !BanditRemovedFromBowerstonePosh) {
            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
            Game->RemoveThing(Me, false, false);
            BanditRemovedFromBowerstonePosh = true;
        }
        if (ParentClass.BanditAttackByHeroCount > 2 && !BanditSwitchedAllegience)
            break;
        if (ParentClass.BanditAttackedByHero) {
            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
            long conversation = Game->AddNewConversation(Me, false, false);
            Game->AddPersonToConversation(conversation, Game->GetHero());
            Game->AddLineToConversation(conversation, "TEXT_QST_003_BANDIT_ATTACK_WARNING", false, Me, Game->GetHero());
            ParentClass.BanditAttackedByHero = false;
        }
        if (ParentClass.RegionInvalid && BanditFollowing) {
            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
            if (ParentClass.QuestFinished) {
                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                Game->DeactivateQuestLater("Q_AmbushTraders_ActivatePhase1", 0);
            }
            else {
                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                if (ResetInvalidRegionTimer) {
                    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                    InvalidRegionTimer = pGlobalScriptDef->AmbushTradersInvalidRegionTimerMax - rand() % pGlobalScriptDef->AmbushTradersInvalidRegionTimerVar;
                    ResetInvalidRegionTimer = false;
                }
                if (InvalidRegionTimer.GetTime() == 0) {
                    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                    long conversation = Game->AddNewConversation(Me, false, false);
                    Game->AddPersonToConversation(conversation, Game->GetHero());
                    if (!ParentClass.SpyContactMade) {
                        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                        Game->AddLineToConversation(conversation, "TEXT_QST_003_BANDIT_WRONG_WAY", false, Me, Game->GetHero());
                    }
                    else {
                        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                        Game->AddLineToConversation(conversation, "TEXT_QST_003_BANDIT_WRONG_WAY_BACK", false, Me, Game->GetHero());
                    }
                    ResetInvalidRegionTimer = true;
                }
            }
        }
        if (ParentClass.QuestFailed || ParentClass.QuestFinished) {
            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
            if (seh_me.IsValid()) seh_me.Release();
            while (!Game->StartScriptingEntity(Me, seh_me, SCRIPT_AI_PRIORITY_HIGHEST)) {
                Game->NewScriptFrame();
                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
            }
            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
            CScriptThing region_exit = Game->GetNearestWithDefName(Me, "REGION_EXIT_POINT");
            if (region_exit.IsAlive()) {
                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                if (seh_me.IsValid()) {
                    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                    seh_me.MoveToPosition(region_exit.GetPos(), 1.0f, ENTITY_MOVE_RUN, false, true);
                }
                while (!IsDistanceBetweenThingsUnder(Me, region_exit, 2.0f)) {
                    Game->NewScriptFrame();
                    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                }
                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                Game->GiveHeroRenownPoints(pGlobalScriptDef->AmbushTradersRenown);
                FadeOutAndKillEntity(Me, 1.0f, true, true);
            }
        }
        if (Me.MsgIsTalkedToBy("SCRIPT_NAME_HERO")) {
            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
            if (seh_me.IsValid()) seh_me.Release();
            while (!Game->StartScriptingEntity(Me, seh_me, SCRIPT_AI_PRIORITY_HIGHEST)) {
                Game->NewScriptFrame();
                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
            }
            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
            if (BanditFollowing) {
                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                CScriptGameResourceObjectMovieBase following_movie;
                Game->StartMovieSequence("", following_movie);
                CWideScreenMagicPauseEntities PauseMyEntitiesPlease(*Game);
                if (Game->GetHealth(seh_me.GetScriptThing()) > 0.0f) {
                    seh_me.Speak(Game->GetHero(), "TEXT_QST_003_BANDIT_FOLLOWING", GROUP_SELECT_RANDOM_NO_REPEAT, false, true, false);
                    while (seh_me.IsPerformingScriptTask()) {
                        Game->NewScriptFrame();
                        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                    }
                    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                }
            }
            else {
                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                if (BanditHired) {
                    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                    CScriptGameResourceObjectMovieBase waiting_movie;
                    Game->StartMovieSequence("", waiting_movie);
                    CWideScreenMagicPauseEntities PauseMyEntitiesPlease(*Game);
                    if (Game->GetHealth(seh_me.GetScriptThing()) > 0.0f) {
                        seh_me.Speak(Game->GetHero(), "TEXT_QST_003_BANDIT_WAITING", GROUP_SELECT_RANDOM_NO_REPEAT, false, true, false);
                        while (seh_me.IsPerformingScriptTask()) {
                            Game->NewScriptFrame();
                            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                        }
                        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                    }
                }
                else {
                    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                    CScriptGameResourceObjectMovieBase hire_movie;
                    Game->StartMovieSequence("", hire_movie);
                    CWideScreenMagicPauseEntities PauseMyEntitiesPlease(*Game);
                    if (Game->GetHealth(seh_me.GetScriptThing()) > 0.0f) {
                        seh_me.Speak(Game->GetHero(), "TEXT_QST_003_BANDIT_HIRE_OFFER", GROUP_SELECT_RANDOM_NO_REPEAT, false, true, false);
                        while (seh_me.IsPerformingScriptTask()) {
                            Game->NewScriptFrame();
                            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                        }
                        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                    }
                    if (Game->GetHealth(seh_me.GetScriptThing()) > 0.0f) {
                        seh_me.Speak(Game->GetHero(), "TEXT_QST_003_BANDIT_HIRE_COST", GROUP_SELECT_RANDOM_NO_REPEAT, false, true, false);
                        while (seh_me.IsPerformingScriptTask()) {
                            Game->NewScriptFrame();
                            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                        }
                        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                    }
                    if (ParentClass.AskBanditHireQuestion(Me, seh_me)) {
                        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                        BanditHired = true;
                        BanditHealthBarID = ParentClass.BanditHireSetup(Me, seh_me);
                        Game->EntitySetAsAllowedToFollowHero(Me, true);
                        Game->EntityFollowThing(Me, Game->GetHero(), 1.0f, true);
                        Game->SetEntityAsRegionFollowing(Game->GetHero(), Me, true);
                        BanditFollowing = true;
                    }
                    else {
                        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                        Game->EntityStopFollowing(Game->GetHero());
                    }
                }
            }
            if (seh_me.IsValid()) seh_me.Release();
        }
        if (Me.MsgExpressionPerformedTo(expression)) {
            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
            if (seh_me.IsValid()) seh_me.Release();
            while (!Game->StartScriptingEntity(Me, seh_me, SCRIPT_AI_PRIORITY_HIGHEST)) {
                Game->NewScriptFrame();
                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
            }
            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
            long conversation = Game->AddNewConversation(Me, false, false);
            Game->AddPersonToConversation(conversation, Game->GetHero());
            if (expression == "EXPRESSION_FOLLOW") {
                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                if (!BanditHired) {
                    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                    CScriptGameResourceObjectMovieBase hire_movie;
                    Game->StartMovieSequence("", hire_movie);
                    CWideScreenMagicPauseEntities PauseMyEntitiesPlease(*Game);
                    if (ParentClass.AskBanditHireQuestion(Me, seh_me)) {
                        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                        Game->EntitySetAsAllowedToFollowHero(Me, true);
                        BanditHired = true;
                        BanditHealthBarID = ParentClass.BanditHireSetup(Me, seh_me);
                        Game->SetScriptingStateGroup(Me, (EScriptingStateGroups)0);
                        BanditFollowing = true;
                    }
                    else {
                        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                        Game->EntitySetAsAllowedToFollowHero(Me, false);
                        Game->NewScriptFrame();
                        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                        Game->NewScriptFrame();
                        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                        Game->EntityStopFollowing(Game->GetHero());
                        Game->NewScriptFrame();
                        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                        Game->NewScriptFrame();
                        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                    }
                }
                else {
                    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                    Game->AddLineToConversation(conversation, "TEXT_QST_003_BANDIT_FOLLOW_CONFIRM", false, Me, Game->GetHero());
                    BanditFollowing = true;
                }
            }
            else if (expression == "EXPRESSION_WAIT") {
                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                Game->AddLineToConversation(conversation, "TEXT_QST_003_BANDIT_WAIT_CONFIRM", false, Me, Game->GetHero());
                BanditFollowing = false;
                Game->SetWanderCentrePoint(Me, Me.GetPos());
                Game->SetWanderMinDistance(Me, 0.0f);
                Game->SetWanderMaxDistance(Me, pGlobalScriptDef->AmbushTradersBanditWanderMax);
                Game->SetScriptingStateGroup(Me, (EScriptingStateGroups)4);
            }
            if (seh_me.IsValid()) seh_me.Release();
        }
        if (ParentClass.TraderEscaped && ParentClass.NoEnemiesLeft && !ParentClass.PhaseComplete) {
            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
            Game->EntityStopFollowing(Me);
            if (seh_me.IsValid()) seh_me.Release();
            while (!Game->StartScriptingEntity(Me, seh_me, SCRIPT_AI_PRIORITY_HIGHEST)) {
                Game->NewScriptFrame();
                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
            }
            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
            CScriptThing exit_point = Game->GetNearestWithDefName(Me, "REGION_EXIT_POINT");
            if (seh_me.IsValid() && exit_point.IsAlive()) {
                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                C3DVector exit_pos = exit_point.GetPos();
                while (IsDistanceFromThingToPositionOver(seh_me.GetScriptThing(), exit_pos, 3.0f)) {
                    Game->NewScriptFrame();
                    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                    seh_me.MoveToPosition(exit_pos, 1.0f, ENTITY_MOVE_WALK, false, true);
                    while (seh_me.IsPerformingScriptTask()) {
                        Game->NewScriptFrame();
                        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                    }
                    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                }
                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                FadeOutAndKillEntity(Me, 1.0f, true, true);
                return;
            }
        }
        Game->NewScriptFrame();
        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
    }
    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
    if (!ParentClass.BanditSwitchAllegienceComplete) {
        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
        long conversation = Game->AddNewConversation(Me, false, false);
        Game->AddPersonToConversation(conversation, Game->GetHero());
        Game->AddLineToConversation(conversation, "TEXT_QST_003_BANDIT_ATTACK_SWITCH_ALLEGIENCE", false, Me, Game->GetHero());
        ParentClass.BanditSwitchAllegienceComplete = true;
    }
    SwitchBanditAllegience();
    BanditSwitchedAllegience = true;
}