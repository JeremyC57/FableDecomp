// NScript::CV_AmbushScamScript::CAS_Buddy::Main() -- the ambush-scam "buddy" (guard or bandit by MoralitySwitch): starts the parent's watcher threads, waits for the trickster and allies with him, follows the hero into the ambush, talks to the hero, turns hostile when attacked, and walks off to its exit point when the quest ends.
// Grade: DIFFER(8551v8567) -- the CScriptThing copy split / function-wide inline budget: retail calls the CSpawnedFunc1 Par1 copy 0x004ABE90 and the by-value parameter dtor 0x004AA840 inside each of the 3 forced CSpawnedFunc1 ctors yet expands the same copy and dtor at depth 1 (the 3 parameter copies, the early-exit ~trickster / ~seh_me, the SetWander / HeroAttackOnBuddy copies) and calls the 2 bottom SetScriptingStateGroup copies; VC7.1 here expands the in-ctor pair (forced row-local copy) and so calls the early-exit dtors and expands the bottom copies
// Next idea: a spelling that keeps the copy / dtor called inside the forced CSpawnedFunc1 ctor while depth-1 sites expand (IMPLICIT_COPY, explicit instantiation before the inline bodies, a plain-inline ctor opt-in and a depth-1 inline pragma as a diagnostic were tried; the landed CUndeadRisingZombieAttacker::Init 0x00E0DAA0 spawn relies on such a pragma, now banned)
// Leaf: ?Main@CAS_Buddy@CV_AmbushScamScript@NScript@@EAEXXZ
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00E2FF00, length 8567 bytes, thiscall, no EH frame.
// Name: slot 1 of the entity vtable 0x012E286C (0 scalar deleting dtor 0x00E2BF80, 2 Init 0x00E2BFA0, 3 GetParentScript 0x00E2BF60,
//   4 OnPersist 0x00E2FEA0, 5 OnPredicateFail 0x00E2BF70) that Alloc 0x00E2BEC0 stores; Ego_r.pdb and FableWin.pdb both hold
//   ?Main@CAS_Buddy@CV_AmbushScamScript@NScript@@EAEXXZ.
#define FABLE_RETAIL_NSCRIPT
#define FABLE_RETAIL_CLEVELSCRIPTBASE
#define FABLE_RETAIL_BASE_OBJECT
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CCHARSTRING_CONCAT_STRING
#define FABLE_RETAIL_SUSPENDABLE_PROCESS
#define FABLE_RETAIL_CDEFSTRINGTABLE
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CRGBCOLOUR
#define FABLE_RETAIL_CWIDESCREENMAGICPAUSEENTITIES_FORCEINLINE_CTOR
#define FABLE_RETAIL_CV_AMBUSHSCAMSCRIPT_BUDDY
#include <stdlib.h>
#include "fable_string.h"
#include "engine/CCountedPointer.h"
#include "engine/CScriptThing.h"
#include "engine/C3DVector.h"
#include "engine/CRGBColour.h"
#include "engine/EScriptingStateGroups.h"
#include "engine/CQ_SunnyvaleMasterData.h"
#include "engine/CV_AmbushScamScript.h"
#include "engine/CScriptGameResourceObjectScriptedThingBase.h"
class CScriptGameResourceObjectMovieBase;
template <> CCountedPointer<CScriptGameResourceObjectMovieBase>::~CCountedPointer();   // 0x006E7AB0
#include "engine/CScriptGameResourceObjectMovieBase.h"
#include "engine/CGameScriptInterfaceBase.h"
#include "engine/CWideScreenMagicPauseEntities.h"
#include "engine/CScriptDef.h"
#include "engine/CDefPointer.h"
#include "engine/CSpawnedFunc1.h"
#include "engine/fable_script_text.h"
enum EScriptAIPriority { SCRIPT_AI_PRIORITY_LOWEST=0, SCRIPT_AI_PRIORITY_LOW=1, SCRIPT_AI_PRIORITY_MEDIUM=2, SCRIPT_AI_PRIORITY_HIGH=3, SCRIPT_AI_PRIORITY_HIGHEST=4 };
enum ETextGroupSelectionMethod { GROUP_SELECT_NONE=0, GROUP_SELECT_RANDOM=1, GROUP_SELECT_RANDOM_NO_REPEAT=2, GROUP_SELECT_SEQUENTIAL=3, GROUP_SELECT_FIRST_ONLY=4 };
enum EScriptEntityMoveType { MOVE_TYPE_WALKING=0, MOVE_TYPE_RUNNING=1 };
extern CDefPointer<const CScriptDef> pGlobalScriptDef;     // 0x0143E90C
bool FABLE_FASTCALL IsDistanceBetweenThingsUnder(const CScriptThing&, const CScriptThing&, float);   // 0x00CBE2FF
void FABLE_FASTCALL FadeOutAndKillEntity(const CScriptThing&, float, bool, bool);                    // 0x00CBE716
inline CScriptThing::CScriptThing() { }
__forceinline CScriptThing::CScriptThing(const CScriptThing& other) : CBaseClass(other), PImp(other.PImp) { }
inline CScriptThing::~CScriptThing() { }
inline CScriptThing& CScriptThing::operator=(const CScriptThing& other) { PImp = other.PImp; return *this; }
inline bool CScriptThing::IsAlive() const { return PImp.Data && PImp.Data->IsAlive(); }
inline const C3DVector& CScriptThing::GetPos() const { if (PImp.IsNull()) return NullCoord; return PImp->GetPos(); }
#define T IsActiveThreadTerminating()
#define NSF Game->NewScriptFrame()
#define AB_START \
    if (seh_me.IsValid()) seh_me.Release(); \
    while (!Game->StartScriptingEntity(Me,seh_me,SCRIPT_AI_PRIORITY_HIGHEST)) { \
        NSF; \
        if (T) return; \
    } \
    if (T) return;
#define AB_SAY(TEXT) \
    if (Game->GetHealth(seh_me.GetScriptThing())>0.0f) { \
        seh_me.Speak(Game->GetHero(),TEXT,GROUP_SELECT_RANDOM_NO_REPEAT,false,true,false); \
        while (seh_me.IsPerformingScriptTask()) { \
            NSF; \
            if (T) return; \
        } \
        if (T) return; \
    }
#define AB_MOVIE_SAY(GUARD,BANDIT) \
    CScriptGameResourceObjectMovieBase movie; \
    Game->StartMovieSequence("",movie); \
    CWideScreenMagicPauseEntities PauseMyEntitiesPlease(*Game); \
    if (ParentClass.MoralitySwitch) { \
        if (T) return; \
        AB_SAY(GUARD) \
    } \
    else { \
        if (T) return; \
        AB_SAY(BANDIT) \
    }
#define AB_SPAWN(FUNC) \
    ParentClass.AddSpawnedFunction(new CSpawnedFunc1<CV_AmbushScamScript,CScriptThing>(CCharString("ParentClass.")+CCharString(#FUNC),0,&ParentClass,&CV_AmbushScamScript::FUNC,Me),CCharString(""));
void NScript::CV_AmbushScamScript::CAS_Buddy::Main()
{
    CScriptGameResourceObjectScriptedThingBase seh_me;
    AB_SPAWN(WatchForExpressions)
    AB_SPAWN(WatchForHeroAttack)
    AB_SPAWN(WatchForTricksterComplete)
    CScriptThing trickster;
    while (!T) {
        trickster=ParentClass.GetTrickster();
        if (trickster.IsAlive()) break;
        NSF;
    }
    if (T) return;
    if (T) return;
    Game->EntitySetThingAsAllyOfThing(Me,trickster);
    Game->EntitySetThingAsAllyOfThing(trickster,Me);
    for (; !T; NSF) {
        if (!BuddyFollowSpeechComplete && Game->IsEntityFollowingHero(Me)) {
            if (T) return;
            BuddyFollowSpeechComplete=true;
            long conversation=Game->AddNewConversation(Me,false,false);
            Game->AddPersonToConversation(conversation,Game->GetHero());
            if (ParentClass.MoralitySwitch) {
                if (T) return;
                Game->AddLineToConversation(conversation,"TEXT_QST_002_GUARD_FOLLOW",false,Me,Game->GetHero());
            }
            else {
                if (T) return;
                Game->AddLineToConversation(conversation,"TEXT_QST_002_BANDIT_FOLLOW",false,Me,Game->GetHero());
            }
        }
        if (ParentClass.TricksterComplete && seh_me.IsValid() && seh_me.IsFollowingThing()) {
            if (T) return;
            AB_START
            seh_me.StopFollowingThing(Game->GetHero());
            if (seh_me.IsValid()) seh_me.Release();
        }
        if (ParentClass.DoAmbush && !BuddyHeroApproachComplete) {
            if (T) return;
            std::vector<CScriptThing> ambushers;
            long count=Game->GetAllThingsWithScriptName("AS_Ambusher",ambushers);
            for (long i=0; i<count; ++i) {
                Game->EntitySetThingAsEnemyOfThing(Me,ambushers[i]);
                Game->EntitySetThingAsEnemyOfThing(ambushers[i],Me);
            }
            AB_START
            seh_me.FollowThing(Game->GetHero(),1.0f,true);
            BuddyHeroApproachComplete=true;
        }
        else if (ParentClass.DoAmbush && !BuddyHeroApproachSpeechComplete && IsDistanceBetweenThingsUnder(Me,Game->GetHero(),3.0f)) {
            if (T) return;
            BuddyHeroApproachSpeechComplete=true;
            seh_me.StopFollowingThing(Game->GetHero());
            if (seh_me.IsValid()) seh_me.Release();
            long conversation=Game->AddNewConversation(Me,false,false);
            Game->AddPersonToConversation(conversation,Game->GetHero());
            CCharString text="TEXT_QST_002_";
            long line=10;
            if (ParentClass.BuddyCounter>1) {
                if (T) return;
                line=20;
            }
            else {
                if (T) return;
                ParentClass.BuddyCounter++;
            }
            if (ParentClass.MoralitySwitch) {
                if (T) return;
                text+="GUARD_ASSIST_"+GFIntToCharString(line);
            }
            else {
                if (T) return;
                text+="BANDIT_ASSIST_"+GFIntToCharString(line);
            }
            Game->AddLineToConversation(conversation,text,false,Me,Game->GetHero());
        }
        else if (ParentClass.AllAmbushersDead && BuddyHeroApproachSpeechComplete) {
            if (T) return;
            Game->SetWanderCentrePoint(Me,BuddyStartPos);
            Game->SetWanderMinDistance(Me,0.0f);
            Game->SetWanderMaxDistance(Me,pGlobalScriptDef->AmbushScamBuddyWanderMax);
            Game->SetScriptingStateGroup(Me,ESSG_WANDER_NEAR);
        }
        if (ParentClass.HeroAttackOnTricksterAfterAmbush && trickster.IsAlive() && !ParentClass.TricksterResetAllegience) {
            if (T) return;
            Game->EntityUnsetThingAsAllyOfThing(Me,trickster);
            Game->EntityUnsetThingAsAllyOfThing(trickster,Me);
            Game->EntitySetThingAsEnemyOfThing(Me,trickster);
            Game->EntitySetThingAsEnemyOfThing(trickster,Me);
            ParentClass.TricksterResetAllegience=true;
        }
        else if (ParentClass.HeroAttackOnBuddy) {
            if (T) return;
            Game->SetScriptingStateGroup(Me,ESSG_NONE);
            Game->EntityUnsetThingAsAllyOfThing(Me,Game->GetHero());
            Game->EntityUnsetThingAsAllyOfThing(Game->GetHero(),Me);
            Game->EntitySetThingAsEnemyOfThing(Me,Game->GetHero());
            Game->EntitySetThingAsEnemyOfThing(Game->GetHero(),Me);
            Game->GiveThingBestEnemyTarget(Me,Game->GetHero());
            if (seh_me.IsValid()) seh_me.Release();
            for (;;) {
                NSF;
                if (T) return;
            }
        }
        else if (Me.MsgIsTalkedToBy("SCRIPT_NAME_HERO")) {
            if (T) return;
            AB_START
            if (ParentClass.TricksterComplete) {
                if (T) return;
                if (ParentClass.BuddyHelped) {
                    if (T) return;
                    AB_MOVIE_SAY("TEXT_QST_002_GUARD_HELPED","TEXT_QST_002_BANDIT_HELPED")
                }
                else {
                    if (T) return;
                    AB_MOVIE_SAY("TEXT_QST_002_GUARD_END","TEXT_QST_002_BANDIT_END")
                }
            }
            else if (ParentClass.DoAmbush) {
                if (T) return;
                if (ParentClass.AllAmbushersDead) {
                    if (T) return;
                    AB_MOVIE_SAY("TEXT_QST_002_GUARD_AMBUSH_FINISHED","TEXT_QST_002_BANDIT_AMBUSH_FINISHED")
                }
                else {
                    if (T) return;
                    AB_MOVIE_SAY("TEXT_QST_002_GUARD_AMBUSH_STARTED","TEXT_QST_002_BANDIT_AMBUSH_STARTED")
                }
            }
            else if (ParentClass.TricksterApproachComplete) {
                if (T) return;
                AB_MOVIE_SAY("TEXT_QST_002_GUARD_CAUTION","TEXT_QST_002_BANDIT_CAUTION")
            }
            else {
                if (T) return;
                if (!ParentClass.BuddyIntroComplete) {
                    if (T) return;
                    CScriptGameResourceObjectMovieBase movie;
                    Game->StartMovieSequence("",movie);
                    CWideScreenMagicPauseEntities PauseMyEntitiesPlease(*Game);
                    if (ParentClass.MoralitySwitch) {
                        if (T) return;
                        AB_SAY("TEXT_QST_002_GUARD_NOTHING")
                        AB_SAY("TEXT_QST_002_GUARD_BORED")
                    }
                    else {
                        if (T) return;
                        AB_SAY("TEXT_QST_002_BANDIT_NOTHING")
                        AB_SAY("TEXT_QST_002_BANDIT_BORED")
                    }
                    ParentClass.BuddyIntroComplete=true;
                }
                else {
                    if (T) return;
                    AB_MOVIE_SAY("TEXT_QST_002_GUARD_OFFER","TEXT_QST_002_BANDIT_OFFER")
                }
            }
            if (seh_me.IsValid()) seh_me.Release();
        }
        if (ParentClass.QuestFinished || ParentClass.TricksterKilledBeforeAmbush) {
            if (T) return;
            CScriptThing exit_point=Game->GetThingWithScriptName("M_AS_BuddyExitPoint");
            AB_START
            Game->ClearThingHasInformation(Me);
            if (seh_me.IsValid() && seh_me.IsFollowActionRunning(Game->GetHero())) {
                if (T) return;
                seh_me.StopFollowingThing(Game->GetHero());
            }
            if (exit_point.IsAlive()) {
                if (T) return;
                if (seh_me.IsValid()) {
                    if (T) return;
                    seh_me.MoveToPosition(exit_point.GetPos(),1.0f,MOVE_TYPE_WALKING,false,true);
                }
                while (!IsDistanceBetweenThingsUnder(Me,exit_point,2.0f)) {
                    NSF;
                    if (T) return;
                }
                if (T) return;
                Game->ClearThingHasInformation(Me);
                FadeOutAndKillEntity(Me,1.0f,true,true);
            }
        }
        else if (ParentClass.ExpressionDetectedWait) {
            if (T) return;
            ParentClass.ExpressionDetectedWait=false;
            Game->SetScriptingStateGroup(Me,ESSG_WANDER_NEAR);
        }
        else if (ParentClass.ExpressionDetectedFollow) {
            if (T) return;
            ParentClass.ExpressionDetectedFollow=false;
            Game->SetScriptingStateGroup(Me,ESSG_NONE);
        }
    }
}
