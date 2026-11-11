// NScript::CV_AmbushScamScript::CAS_Buddy::Main() -- the ambush-scam "buddy" (guard or bandit by MoralitySwitch): starts the parent's watcher threads, waits for the trickster and allies with him, follows the hero into the ambush, talks to the hero, turns hostile when attacked, and walks off to its exit point when the quest ends.
// Grade: RELOCATION_MATCH -- actual COFF operands checked against their native targets.
// Inline-budget: T66 plus scoped depth0 at four cold returns, ambusher acquisition, nested spawn copies and Exit.IsAlive. RTM traces/native sites prove expansion choices; pragma placement is reconstruction.
// Flags: /d2inlT66
// Leaf: ?Main@CAS_Buddy@CV_AmbushScamScript@NScript@@EAEXXZ
// Retail: 0x00E2FF00, 8567 bytes, thiscall.
// Name: slot 1 of the entity vtable 0x012E286C (0 scalar deleting dtor 0x00E2BF80, 2 Init 0x00E2BFA0, 3 GetParentScript 0x00E2BF60,
//   4 OnPersist 0x00E2FEA0, 5 OnPredicateFail 0x00E2BF70) that Alloc 0x00E2BEC0 stores; Ego_r.pdb and FableWin.pdb both hold
//   ?Main@CAS_Buddy@CV_AmbushScamScript@NScript@@EAEXXZ.
#define FABLE_RETAIL_CLEVELSCRIPTBASE_ORIGINAL_MEMBERS
#define FABLE_RETAIL_CISTHINGALIVE_ORIGINAL_ACCESS
#define FABLE_RETAIL_CENTITYSCRIPTBASE_ORIGINAL_MEMBERS
#define FABLE_RETAIL_CPREDICATEBASE_ORIGINAL_MEMBERS
#define FABLE_RETAIL_C3DVECTOR_PACKED
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
#define FABLE_RETAIL_CSPAWNEDFUNC1_ORIGINAL_ACCESS
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
#include "engine/TRetVoid.h"
enum EScriptAIPriority { SCRIPT_AI_PRIORITY_LOWEST=0, SCRIPT_AI_PRIORITY_LOW=1, SCRIPT_AI_PRIORITY_MEDIUM=2, SCRIPT_AI_PRIORITY_HIGH=3, SCRIPT_AI_PRIORITY_HIGHEST=4 };
enum ETextGroupSelectionMethod { GROUP_SELECT_NONE=0, GROUP_SELECT_RANDOM=1, GROUP_SELECT_RANDOM_NO_REPEAT=2, GROUP_SELECT_SEQUENTIAL=3, GROUP_SELECT_FIRST_ONLY=4 };
#include "engine/EScriptEntityMoveType.h"
extern CDefPointer<const CScriptDef> pGlobalScriptDef;     // 0x0143E90C
bool FABLE_FASTCALL IsDistanceBetweenThingsUnder(const CScriptThing&, const CScriptThing&, float);   // 0x00CBE2FF
void FABLE_FASTCALL FadeOutAndKillEntity(const CScriptThing&, float, bool, bool);                    // 0x00CBE716
inline CScriptThing::CScriptThing() { }
inline CScriptThing::CScriptThing(const CScriptThing& other) : CBaseClass(other), PImp(other.PImp) { }
inline CScriptThing::~CScriptThing() { }
inline CScriptThing& CScriptThing::operator=(const CScriptThing& other) { CBaseClass::operator=(other); PImp = other.PImp; return *this; }
inline bool CScriptThing::IsAlive() const { return PImp.Data && PImp.Data->IsAlive(); }
inline const C3DVector& CScriptThing::GetPos() const { if (PImp.IsNull()) return NullCoord; return PImp->GetPos(); }
#ifdef _MSC_VER
#pragma inline_depth(0)
#endif
template<> FABLE_FORCEINLINE CSpawnedFunc1<NScript::CV_AmbushScamScript,CScriptThing>::CSpawnedFunc1(const CCharString& name, unsigned long id, NScript::CV_AmbushScamScript* pclass, void (NScript::CV_AmbushScamScript::*func)(CScriptThing), CScriptThing par1)
    : CSpawnedFuncBase(name, id), Func(func), PClass(pclass), Par1(par1) { }
#ifdef _MSC_VER
#pragma inline_depth()
#endif
#define T IsActiveThreadTerminating()
#define NSF Game.NewScriptFrame()
#define AB_START \
    if (sehMe.IsValid()) sehMe.Release(); \
    while (!Game.StartScriptingEntity(Me,sehMe,SCRIPT_AI_PRIORITY_HIGHEST)) { \
        NSF; \
        if (T) return _Fail_Val.Return(); \
    } \
    if (T) return _Fail_Val.Return();
#define AB_SAY(TEXT,SELECTION) \
    if (Game.GetHealth(sehMe.GetScriptThing())>0.0f) { \
        sehMe.Speak(Game.GetHero(),TEXT,SELECTION,false,true,false); \
        while (sehMe.IsPerformingScriptTask()) { \
            NSF; \
            if (T) return _Fail_Val.Return(); \
        } \
        if (T) return _Fail_Val.Return(); \
    }
#define AB_MOVIE_SAY(GUARD,BANDIT) \
    CScriptGameResourceObjectMovieBase speech_movie; \
    Game.StartMovieSequence("",speech_movie); \
    CWideScreenMagicPauseEntities PauseMyEntitiesPlease(Game); \
    if (ParentClass.MoralitySwitch) { \
        if (T) return _Fail_Val.Return(); \
        AB_SAY(GUARD,GROUP_SELECT_RANDOM_NO_REPEAT) \
    } \
    else { \
        if (T) return _Fail_Val.Return(); \
        AB_SAY(BANDIT,GROUP_SELECT_RANDOM_NO_REPEAT) \
    }
#define AB_SPAWN(FUNC) \
    ParentClass.AddSpawnedFunction(new CSpawnedFunc1<CV_AmbushScamScript,CScriptThing>(CCharString("ParentClass.")+CCharString(#FUNC),0,&ParentClass,&CV_AmbushScamScript::FUNC,Me),CCharString(""));
void NScript::CV_AmbushScamScript::CAS_Buddy::Main()
{
    TRetVoid _Fail_Val;
    CScriptGameResourceObjectScriptedThingBase sehMe;
    AB_SPAWN(WatchForExpressions)
    AB_SPAWN(WatchForHeroAttack)
    AB_SPAWN(WatchForTricksterComplete)
    CScriptThing trickster;
    for (; !false; NSF) {
        if (T) return _Fail_Val.Return();
        trickster=ParentClass.GetTrickster();
        if (trickster.IsAlive()) {
            if (T) return _Fail_Val.Return();
            break;
        }
    }
    if (T) return _Fail_Val.Return();
    Game.EntitySetThingAsAllyOfThing(Me,trickster);
    Game.EntitySetThingAsAllyOfThing(trickster,Me);
    for (; !false; NSF) {
        if (T) return _Fail_Val.Return();
        if (!BuddyFollowSpeechComplete && Game.IsEntityFollowingHero(Me)) {
            // Native E304A1 -> E3205A calls both Thing/STB destructors.
#ifdef _MSC_VER
#pragma inline_depth(0)
#endif
            if (T) return _Fail_Val.Return();
#ifdef _MSC_VER
#pragma inline_depth()
#endif
            BuddyFollowSpeechComplete=true;
            long HLStyleConversationIndex=Game.AddNewConversation(Me,false,false);
            Game.AddPersonToConversation(HLStyleConversationIndex,Game.GetHero());
            if (ParentClass.MoralitySwitch) {
                if (T) return _Fail_Val.Return();
                Game.AddLineToConversation(HLStyleConversationIndex,"TEXT_QST_002_GUARD_FOLLOW",false,Me,Game.GetHero());
            }
            else {
                if (T) return _Fail_Val.Return();
                Game.AddLineToConversation(HLStyleConversationIndex,"TEXT_QST_002_BANDIT_FOLLOW",false,Me,Game.GetHero());
            }
        }
        if (ParentClass.TricksterComplete && sehMe.IsValid() && sehMe.IsFollowingThing()) {
            if (T) return _Fail_Val.Return();
            AB_START
            sehMe.StopFollowingThing(Game.GetHero());
            if (sehMe.IsValid()) sehMe.Release();
        }
        if (ParentClass.DoAmbush && !BuddyHeroApproachComplete) {
            // Native E30657 -> E3205A calls both Thing/STB destructors.
#ifdef _MSC_VER
#pragma inline_depth(0)
#endif
            if (T) return _Fail_Val.Return();
#ifdef _MSC_VER
#pragma inline_depth()
#endif
            std::vector<CScriptThing> ambushers;
            long numAmbushers=Game.GetAllThingsWithScriptName("AS_Ambusher",ambushers);
            if (!false) {
            for (long idx=0; idx<numAmbushers; ++idx) {
                Game.EntitySetThingAsEnemyOfThing(Me,ambushers[idx]);
                Game.EntitySetThingAsEnemyOfThing(ambushers[idx],Me);
            }
            }
#ifdef _MSC_VER
#pragma inline_depth(0)
#endif
            AB_START
#ifdef _MSC_VER
#pragma inline_depth()
#endif
            sehMe.FollowThing(Game.GetHero(),1.0f,true);
            BuddyHeroApproachComplete=true;
        }
        else if (BuddyHeroApproachComplete && !BuddyHeroApproachSpeechComplete && IsDistanceBetweenThingsUnder(Me,Game.GetHero(),3.0f)) {
            if (T) return _Fail_Val.Return();
            BuddyHeroApproachSpeechComplete=true;
            sehMe.StopFollowingThing(Game.GetHero());
            if (sehMe.IsValid()) sehMe.Release();
            long HLStyleConversationIndex=Game.AddNewConversation(Me,false,false);
            Game.AddPersonToConversation(HLStyleConversationIndex,Game.GetHero());
            long suffix=10;
            CCharString assistText="TEXT_QST_002_";
            if (ParentClass.BuddyCounter>1) {
                if (T) return _Fail_Val.Return();
                suffix=20;
            }
            else {
                if (T) return _Fail_Val.Return();
                ParentClass.BuddyCounter++;
            }
            if (ParentClass.MoralitySwitch) {
                if (T) return _Fail_Val.Return();
                assistText+="GUARD_ASSIST_"+GFIntToCharString(suffix);
            }
            else {
                if (T) return _Fail_Val.Return();
                assistText+="BANDIT_ASSIST_"+GFIntToCharString(suffix);
            }
            Game.AddLineToConversation(HLStyleConversationIndex,assistText,false,Me,Game.GetHero());
        }
        else if (ParentClass.AllAmbushersDead && BuddyHeroApproachSpeechComplete) {
            if (T) return _Fail_Val.Return();
            Game.SetWanderCentrePoint(Me,BuddyStartPos);
            Game.SetWanderMinDistance(Me,0.0f);
            Game.SetWanderMaxDistance(Me,pGlobalScriptDef->AmbushScamBuddyWanderMax);
            Game.SetScriptingStateGroup(Me,ESSG_WANDER_NEAR);
        }
        if (ParentClass.HeroAttackOnTricksterAfterAmbush && trickster.IsAlive() && !ParentClass.TricksterResetAllegience) {
            if (T) return _Fail_Val.Return();
            Game.EntityUnsetThingAsAllyOfThing(Me,trickster);
            Game.EntityUnsetThingAsAllyOfThing(trickster,Me);
            Game.EntitySetThingAsEnemyOfThing(Me,trickster);
            Game.EntitySetThingAsEnemyOfThing(trickster,Me);
            ParentClass.TricksterResetAllegience=true;
        }
        else if (ParentClass.HeroAttackOnBuddy) {
            // Native E31EBD -> E3205A calls both Thing/STB destructors.
#ifdef _MSC_VER
#pragma inline_depth(0)
#endif
            if (T) return _Fail_Val.Return();
#ifdef _MSC_VER
#pragma inline_depth()
#endif
            Game.SetScriptingStateGroup(Me,ESSG_NONE);
            Game.EntityUnsetThingAsAllyOfThing(Me,Game.GetHero());
            Game.EntityUnsetThingAsAllyOfThing(Game.GetHero(),Me);
            Game.EntitySetThingAsEnemyOfThing(Me,Game.GetHero());
            Game.EntitySetThingAsEnemyOfThing(Game.GetHero(),Me);
            Game.GiveThingBestEnemyTarget(Me,Game.GetHero());
            if (sehMe.IsValid()) sehMe.Release();
            for (;;) {
                NSF;
                // Native E31F8A -> E3205A calls both Thing/STB destructors.
#ifdef _MSC_VER
#pragma inline_depth(0)
#endif
                if (T) return _Fail_Val.Return();
#ifdef _MSC_VER
#pragma inline_depth()
#endif
            }
        }
        else if (Me.MsgIsTalkedToBy("SCRIPT_NAME_HERO")) {
            if (T) return _Fail_Val.Return();
            AB_START
            if (ParentClass.TricksterComplete) {
                if (T) return _Fail_Val.Return();
                if (ParentClass.BuddyHelped) {
                    if (T) return _Fail_Val.Return();
                    AB_MOVIE_SAY("TEXT_QST_002_GUARD_HELPED","TEXT_QST_002_BANDIT_HELPED")
                }
                else {
                    if (T) return _Fail_Val.Return();
                    AB_MOVIE_SAY("TEXT_QST_002_GUARD_END","TEXT_QST_002_BANDIT_END")
                }
            }
            else if (ParentClass.DoAmbush) {
                if (T) return _Fail_Val.Return();
                if (ParentClass.AllAmbushersDead) {
                    if (T) return _Fail_Val.Return();
                    AB_MOVIE_SAY("TEXT_QST_002_GUARD_AMBUSH_FINISHED","TEXT_QST_002_BANDIT_AMBUSH_FINISHED")
                }
                else {
                    if (T) return _Fail_Val.Return();
                    AB_MOVIE_SAY("TEXT_QST_002_GUARD_AMBUSH_STARTED","TEXT_QST_002_BANDIT_AMBUSH_STARTED")
                }
            }
            else if (ParentClass.TricksterApproachComplete) {
                if (T) return _Fail_Val.Return();
                AB_MOVIE_SAY("TEXT_QST_002_GUARD_CAUTION","TEXT_QST_002_BANDIT_CAUTION")
            }
            else {
                if (T) return _Fail_Val.Return();
                if (!ParentClass.BuddyIntroComplete) {
                    if (T) return _Fail_Val.Return();
                    CScriptGameResourceObjectMovieBase speech_movie;
                    Game.StartMovieSequence("",speech_movie);
                    CWideScreenMagicPauseEntities PauseMyEntitiesPlease(Game);
                    if (ParentClass.MoralitySwitch) {
                        if (T) return _Fail_Val.Return();
                        AB_SAY("TEXT_QST_002_GUARD_NOTHING",GROUP_SELECT_NONE)
                        AB_SAY("TEXT_QST_002_GUARD_BORED",GROUP_SELECT_NONE)
                    }
                    else {
                        if (T) return _Fail_Val.Return();
                        AB_SAY("TEXT_QST_002_BANDIT_NOTHING",GROUP_SELECT_NONE)
                        AB_SAY("TEXT_QST_002_BANDIT_BORED",GROUP_SELECT_NONE)
                    }
                    ParentClass.BuddyIntroComplete=true;
                }
                else {
                    if (T) return _Fail_Val.Return();
                    AB_MOVIE_SAY("TEXT_QST_002_GUARD_OFFER","TEXT_QST_002_BANDIT_OFFER")
                }
            }
            if (sehMe.IsValid()) sehMe.Release();
        }
        if (ParentClass.QuestFinished || ParentClass.TricksterKilledBeforeAmbush) {
            if (T) return _Fail_Val.Return();
            CScriptThing Exit=Game.GetThingWithScriptName("M_AS_BuddyExitPoint");
            AB_START
            Game.ClearThingHasInformation(Me);
            if (sehMe.IsValid() && sehMe.IsFollowActionRunning(Game.GetHero())) {
                if (T) return _Fail_Val.Return();
                sehMe.StopFollowingThing(Game.GetHero());
            }
#ifdef _MSC_VER
#pragma inline_depth(0)
#endif
            if (Exit.IsAlive()) {
#ifdef _MSC_VER
#pragma inline_depth()
#endif
                if (T) return _Fail_Val.Return();
                if (sehMe.IsValid()) {
                    if (T) return _Fail_Val.Return();
                    sehMe.MoveToPosition(Exit.GetPos(),1.0f,ENTITY_MOVE_WALK,false,true);
                }
                while (!IsDistanceBetweenThingsUnder(Me,Exit,2.0f)) {
                    NSF;
                    if (T) return _Fail_Val.Return();
                }
                if (T) return _Fail_Val.Return();
                Game.ClearThingHasInformation(Me);
                FadeOutAndKillEntity(Me,1.0f,true,true);
            }
        }
        else if (ParentClass.ExpressionDetectedWait) {
            if (T) return _Fail_Val.Return();
            ParentClass.ExpressionDetectedWait=false;
            Game.SetScriptingStateGroup(Me,ESSG_WANDER_NEAR);
        }
        else if (ParentClass.ExpressionDetectedFollow) {
            if (T) return _Fail_Val.Return();
            ParentClass.ExpressionDetectedFollow=false;
            Game.SetScriptingStateGroup(Me,ESSG_NONE);
        }
    }
    if (T) return _Fail_Val.Return();
    while (true) {
        NSF;
        if (T) return _Fail_Val.Return();
    }
    if (T) return _Fail_Val.Return();
}