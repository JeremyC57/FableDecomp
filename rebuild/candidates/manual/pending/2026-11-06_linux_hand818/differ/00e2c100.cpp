// NScript::CV_AmbushScamScript::CAS_Trickster::Main() -- the ambush-scam "trickster" villager: watches for hero attacks, waits for the hero at the approach triggers, follows him to the no-entry marker, gives his plea, runs off and calls the hero over, then loops (random speech, hero-attack warnings, retreat when the ambush springs, the good / semi / evil endings and the exit walk).
// Grade: DIFFER(8945v8945) -- one scheduling swap in the shared PauseMyEntitiesPlease return pad 0x00E2E38E (the last pad, at esp+0xf0, of the HELP_GOOD / HELP_EVIL scene, falling into the shared tail): retail `mov edx,[ebp]; mov ecx,ebp; push 0`, ours `mov edx,[ebp]; push 0; mov ecx,ebp` (2 lines); every other pad (0x00E2E378 etc.) has push-first in both; all other bytes match up to relocations
// Next idea: the HELP_GOOD / HELP_EVIL scene spelling whose cross-jumped exit pad gets the this-first schedule; tried and unchanged: chunk 816 a4-a10 (TRetVoid returns, nested movie block, else-if(!T), named morality bool, inverted MoralitySwitch, hoisted shared T, trailing continue) and chunk 818 t1-t10 (hand-expanded TS_SAY arms, TRetVoid / do-while(0) / goto exits in either arm, Release duplicated per arm); a movie per arm (t11) or one TS_SAY on a ?: text (t12) are worse
// Leaf: ?Main@CAS_Trickster@CV_AmbushScamScript@NScript@@EAEXXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00E2C100, 8945 bytes, thiscall, plain `ret`, no EH frame.
// Identity (proven): slot 1 (Main) of the CAS_Trickster entity vtable 0x012E2850 that Alloc 0x00E2BC90 stores (slot 0 the landed
//   scalar deleting dtor, slot 2 Init); Ego_r.pdb / FableWin.pdb ?Main@CAS_Trickster@CV_AmbushScamScript@NScript@@EAEXXZ.
#define FABLE_RETAIL_NSCRIPT
#define FABLE_RETAIL_CLEVELSCRIPTBASE
#define FABLE_RETAIL_BASE_OBJECT
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CCHARSTRING_CONCAT_STRING
#define FABLE_RETAIL_SUSPENDABLE_PROCESS
#define FABLE_RETAIL_CDEFSTRINGTABLE
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CDEFSTRING_OUTOFLINE_CONVERSION
#define FABLE_RETAIL_CRGBCOLOUR
#define FABLE_RETAIL_CWIDESCREENMAGICPAUSEENTITIES_FORCEINLINE_CTOR
#define FABLE_RETAIL_CSCRIPTGAMERESOURCEOBJECTSCRIPTEDTHINGBASE_OUTOFLINE_DTOR
#define FABLE_RETAIL_CSCRIPTGAMERESOURCEOBJECTMOVIEBASE_OUTOFLINE_CTOR
#define FABLE_RETAIL_CSCRIPTGAMERESOURCEOBJECTMOVIEBASE_OUTOFLINE_DTOR
#include "fable_string.h"
#include "engine/CCountedPointer.h"
#include "engine/CScriptThing.h"
#include "engine/C3DVector.h"
#include "engine/CRGBColour.h"
#include "engine/EScriptingStateGroups.h"
#include "engine/CQ_SunnyvaleMasterData.h"
#include "engine/CV_AmbushScamScript.h"
#include "engine/CScriptGameResourceObjectScriptedThingBase.h"
#include "engine/CScriptGameResourceObjectMovieBase.h"
#include "engine/CGameScriptInterfaceBase.h"
#include "engine/CWideScreenMagicPauseEntities.h"
#include "engine/CTimer.h"
#include "engine/CIsThingAlive.h"
#include "engine/CSpawnedFunc1.h"
#include "engine/fable_script_def_global.h"
#include "engine/fable_script_distance.h"
#include "engine/fable_script_fade.h"
enum EScriptAIPriority { SCRIPT_AI_PRIORITY_LOWEST=0, SCRIPT_AI_PRIORITY_LOW=1, SCRIPT_AI_PRIORITY_MEDIUM=2, SCRIPT_AI_PRIORITY_HIGH=3, SCRIPT_AI_PRIORITY_HIGHEST=4 };
enum ETextGroupSelectionMethod { GROUP_SELECT_NONE=0, GROUP_SELECT_RANDOM=1, GROUP_SELECT_RANDOM_NO_REPEAT=2, GROUP_SELECT_SEQUENTIAL=3, GROUP_SELECT_FIRST_ONLY=4 };
enum EScriptEntityMoveType { MOVE_TYPE_WALKING=0, MOVE_TYPE_RUNNING=1 };
extern CGameScriptInterfaceBase* pDSTGame;   // 0x0143E8F8
inline NScript::CIsThingAlive::CIsThingAlive(const CScriptThing& thing):Thing(thing) {}
__forceinline long CTimer::GetTime() const { return pDSTGame->GetTimer(TimerIndex); }
__forceinline const C3DVector& CScriptThing::GetPos() const { if (!PImp.Data) return NullCoord; return PImp.Data->GetPos(); }
#define T IsActiveThreadTerminating()
#define NSF Game->NewScriptFrame()
#define NSF2 \
    NSF; \
    if (T) return; \
    NSF; \
    if (T) return;
#define TS_START \
    if (seh_me.IsValid()) seh_me.Release(); \
    while (!Game->StartScriptingEntity(Me,seh_me,SCRIPT_AI_PRIORITY_HIGHEST)) { \
        NSF; \
        if (T) return; \
    } \
    if (T) return;
#define TS_SAY(TEXT,G) \
    if (Game->GetHealth(seh_me.GetScriptThing())>0.0f) { \
        seh_me.Speak(Game->GetHero(),TEXT,G,false,true,false); \
        while (seh_me.IsPerformingScriptTask()) { \
            NSF; \
            if (T) return; \
        } \
        if (T) return; \
    }
#define TS_MOVIE \
    CScriptGameResourceObjectMovieBase movie; \
    Game->StartMovieSequence("",movie); \
    CWideScreenMagicPauseEntities PauseMyEntitiesPlease(*Game);
#define TS_FACE_HERO \
    NSF2 \
    Game->EntitySetFacingAngleTowardsThing(Me,Game->GetHero(),false); \
    NSF2 \
    Game->EntitySetFacingAngleTowardsThing(Game->GetHero(),Me,false); \
    NSF2
#define TS_SPAWN(FUNC) \
    ParentClass.AddSpawnedFunction(new CSpawnedFunc1<CV_AmbushScamScript,CScriptThing>(CCharString("ParentClass.")+CCharString(#FUNC),0,&ParentClass,&CV_AmbushScamScript::FUNC,Me),CCharString(""));
void NScript::CV_AmbushScamScript::CAS_Trickster::Main()
{
    CCharString region;
    SetPredicate(CIsThingAlive(Me));
    NSF;
    if (T) return;
    CScriptGameResourceObjectScriptedThingBase seh_me;
    TS_START
    TS_SPAWN(WatchForHeroAttack)
    CScriptThing trigger1=Game->GetThingWithScriptName("M_AS_ApproachTrigger1");
    CScriptThing trigger2=Game->GetThingWithScriptName("M_AS_ApproachTrigger2");
    CScriptThing trigger3=Game->GetThingWithScriptName("M_AS_ApproachTrigger3");
    while (!IsDistanceBetweenThingsUnder(trigger1,Game->GetHero(),pGlobalScriptDef->AmbushScamApproachTriggerProximity)
        && !IsDistanceBetweenThingsUnder(trigger2,Game->GetHero(),pGlobalScriptDef->AmbushScamApproachTriggerProximity)
        && !IsDistanceBetweenThingsUnder(trigger3,Game->GetHero(),pGlobalScriptDef->AmbushScamApproachTriggerProximity)) {
        NSF;
        if (T) return;
    }
    if (T) return;
    seh_me.FollowThing(Game->GetHero(),1.0f,true);
    CScriptThing no_entry=Game->GetThingWithScriptName("M_AS_TricksterNoEntry");
    if (T) return;
    for (;;) {
        if (ParentClass.DoTricksterIntro) break;
        if (IsDistanceBetweenThingsUnder(no_entry,Me,2.0f) && !ParentClass.TricksterWaitHere) {
            if (T) return;
            ParentClass.TricksterApproachProximity=pGlobalScriptDef->AmbushScamTricksterApproachProximityHigh;
            seh_me.StopFollowingThing(Game->GetHero());
            if (seh_me.IsValid()) seh_me.Release();
            Game->SetWanderCentrePoint(Me,Me.GetPos());
            Game->SetWanderMinDistance(Me,0.0f);
            Game->SetWanderMaxDistance(Me,2.0f);
            Game->SetScriptingStateGroup(Me,ESSG_WANDER_NEAR);
            ParentClass.TricksterWaitHere=true;
            Game->SetThingHasInformation(Me,true,true,false);
        }
        else if (IsDistanceBetweenThingsUnder(no_entry,Game->GetHero(),2.0f) && ParentClass.TricksterWaitHere) {
            if (T) return;
            Game->SetScriptingStateGroup(Me,ESSG_NONE);
            TS_START
            seh_me.FollowThing(Game->GetHero(),1.0f,true);
            ParentClass.TricksterWaitHere=false;
        }
        NSF;
        if (!T) continue;
        return;
    }
    if (T) return;
    if (T) return;
    if (seh_me.IsValid() && seh_me.IsFollowActionRunning(Game->GetHero())) {
        if (T) return;
        seh_me.StopFollowingThing(Game->GetHero());
    }
    TS_START
    if (!ParentClass.TricksterSpeechComplete) {
        if (T) return;
        TS_FACE_HERO
        TS_MOVIE
        if (ParentClass.MoralitySwitch) {
            if (T) return;
            TS_SAY("TEXT_QST_002_TRICKSTER_HELP_WIFE",GROUP_SELECT_NONE)
        }
        else {
            if (T) return;
            TS_SAY("TEXT_QST_002_TRICKSTER_ATTACK",GROUP_SELECT_NONE)
        }
        ParentClass.TricksterSpeechComplete=true;
    }
    CScriptThing run_away=Game->GetThingWithScriptName("M_AS_TricksterRunAway");
    seh_me.MoveToPosition(run_away.GetPos(),1.0f,MOVE_TYPE_RUNNING,false,true);
    while (!IsDistanceBetweenThingsUnder(run_away,Me,pGlobalScriptDef->AmbushScamTricksterRunAwayProximity)) {
        NSF;
        if (T) return;
    }
    if (T) return;
    while (seh_me.IsPerformingScriptTask()) {
        NSF;
        if (T) return;
    }
    if (T) return;
    NSF2
    Game->EntitySetFacingAngleTowardsThing(Me,Game->GetHero(),false);
    NSF2
    {
        long conversation=Game->AddNewConversation(Me,false,false);
        Game->AddPersonToConversation(conversation,Game->GetHero());
        Game->AddLineToConversation(conversation,"TEXT_QST_002_VILLAGER_OVER_HERE",false,Me,Game->GetHero());
    }
    while (!T) {
        if (ParentClass.RandomSpeechTimer.GetTime()==0 && ParentClass.DoAmbush && !ParentClass.HeroAttackOnTricksterAfterAmbush && !ParentClass.HeroTalkToTrickster) {
            if (T) return;
            CCharString speech=GetRandomSpeech();
            NSF2
            Game->EntitySetFacingAngleTowardsThing(Game->GetHero(),Me,false);
            NSF2
            long conversation=Game->AddNewConversation(Me,false,false);
            Game->AddPersonToConversation(conversation,Game->GetHero());
            Game->AddLineToConversation(conversation,speech,false,Me,Game->GetHero());
            ParentClass.ResetRandomSpeechTime();
        }
        if (ParentClass.HeroAttachOnTricksterWarning) {
            if (T) return;
            TS_START
            {
                TS_MOVIE
                TS_SAY("TEXT_QST_002_HERO_ATTACK_ON_TRICKSTER_WARNING",GROUP_SELECT_RANDOM_NO_REPEAT)
            }
            if (seh_me.IsValid()) seh_me.Release();
            ParentClass.HeroAttachOnTricksterWarning=false;
        }
        else if (ParentClass.HeroAttackOnTricksterBeforeAmbush) {
            if (T) return;
            TS_START
            {
                TS_MOVIE
                TS_SAY("TEXT_QST_002_HERO_ATTACK_ON_TRICKSTER",GROUP_SELECT_RANDOM_NO_REPEAT)
            }
            if (seh_me.IsValid()) seh_me.Release();
            ParentClass.HeroAttackOnTricksterBeforeAmbush=false;
        }
        else if (ParentClass.HeroAttackOnTricksterAfterAmbush) {
            if (T) return;
            if (!ParentClass.TricksterWhiningComplete) {
                if (T) return;
                TS_START
                {
                    TS_MOVIE
                    TS_SAY("TEXT_QST_002_VILLAGER_ENDING_EVIL",GROUP_SELECT_RANDOM)
                }
                ParentClass.TricksterWhiningComplete=true;
                Game->EntitySetAsScared(Me,false);
                Game->EntityUnsetThingAsAllyOfThing(Me,Game->GetHero());
                Game->EntityUnsetThingAsAllyOfThing(Game->GetHero(),Me);
                Game->EntitySetThingAsEnemyOfThing(Me,Game->GetHero());
                Game->EntitySetThingAsEnemyOfThing(Game->GetHero(),Me);
                Game->ClearThingHasInformation(Me);
                Game->MiniMapRemoveMarker(Me);
                ParentClass.TricksterComplete=true;
                if (seh_me.IsValid()) seh_me.Release();
                for (;;) {
                    NSF;
                    if (T) return;
                }
            }
        }
        else if (ParentClass.DoAmbush && !ParentClass.TricksterRetreatComplete) {
            if (T) return;
            Game->MiniMapRemoveMarker(Me);
            TS_START
            Game->EntitySetAsScared(Me,true);
            Game->EntitySetPersonalityOverride(Me,"OPINION_PERSONALITY_AS_TRICKSTER");
            Game->EntityGiveGold(Me,pGlobalScriptDef->AmbushScamGoldReward);
            Game->AddItemToContainer(Me,pGlobalScriptDef->AmbushScamBonusObject);
            CScriptThing retreat=Game->GetThingWithScriptName("M_AS_TricksterRetreat");
            seh_me.MoveToPosition(retreat.GetPos(),1.0f,MOVE_TYPE_RUNNING,false,true);
            while (!IsDistanceBetweenThingsUnder(Me,retreat,2.0f)) {
                NSF;
                if (T) return;
            }
            if (T) return;
            if (seh_me.IsValid()) seh_me.Release();
            Game->SetWanderCentrePoint(Me,retreat.GetPos());
            Game->SetWanderMinDistance(Me,0.0f);
            Game->SetWanderMaxDistance(Me,pGlobalScriptDef->AmbushScamTricksterWanderMax);
            Game->SetScriptingStateGroup(Me,ESSG_WANDER_NEAR);
            ParentClass.TricksterRetreatComplete=true;
        }
        else if (ParentClass.AllAmbushersDead && !ParentClass.TricksterQuestComplete) {
            if (T) return;
            ParentClass.TricksterQuestComplete=true;
        }
        else if (Me.MsgIsTalkedToBy("SCRIPT_NAME_HERO")) {
            if (T) return;
            if (ParentClass.AllAmbushersDead) {
                if (T) return;
                Game->EntityClearPersonalityOverride(Me);
                Game->EntitySetAsScared(Me,false);
                TS_START
                Game->MiniMapRemoveMarker(Me);
                TS_FACE_HERO
                if (ParentClass.HitCount==0) {
                    if (T) return;
                    {
                        TS_MOVIE
                        TS_SAY("TEXT_QST_002_VILLAGER_ENDING_GOOD",GROUP_SELECT_NONE)
                    }
                    Game->EntityGiveGold(Me,-pGlobalScriptDef->AmbushScamGoldReward);
                    Game->GiveHeroGold(pGlobalScriptDef->AmbushScamGoldReward);
                    Game->RemoveItemFromContainer(Me,pGlobalScriptDef->AmbushScamBonusObject);
                    Game->GiveHeroObject(pGlobalScriptDef->AmbushScamBonusObject,-1,false);
                }
                else {
                    if (T) return;
                    {
                        TS_MOVIE
                        TS_SAY("TEXT_QST_002_VILLAGER_ENDING_SEMI",GROUP_SELECT_NONE)
                    }
                    Game->EntityGiveGold(Me,-pGlobalScriptDef->AmbushScamGoldReward);
                    Game->GiveHeroGold(pGlobalScriptDef->AmbushScamGoldReward);
                }
                ParentClass.TricksterComplete=true;
                ParentClass.HeroTalkToTrickster=true;
                TS_SPAWN(WatchForTricksterKilledAfterBeingSpared)
                CScriptThing exit_point=Game->GetThingWithScriptName("M_AS_TricksterExitPoint");
                if (exit_point.IsAlive()) {
                    if (T) return;
                    if (seh_me.IsValid()) {
                        if (T) return;
                        seh_me.MoveToPosition(exit_point.GetPos(),1.0f,MOVE_TYPE_RUNNING,false,true);
                    }
                    while (!IsDistanceBetweenThingsUnder(Me,exit_point,2.0f) && !Game->MsgOnRegionLoaded(region)) {
                        NSF;
                        if (T) return;
                    }
                    if (T) return;
                    Game->MiniMapRemoveMarker(Me);
                    FadeOutAndKillEntity(Me,1.0f,true,true);
                    ParentClass.TricksterDeadOrLeft=true;
                }
            }
            else if (ParentClass.DoAmbush) {
                if (T) return;
                TS_FACE_HERO
                TS_START
                TS_MOVIE
                TS_SAY("TEXT_QST_002_VILLAGER_HELP",GROUP_SELECT_RANDOM_NO_REPEAT)
                if (seh_me.IsValid()) seh_me.Release();
            }
            else {
                if (T) return;
                TS_START
                TS_MOVIE
                if (ParentClass.MoralitySwitch) {
                    if (T) return;
                    TS_SAY("TEXT_QST_002_VILLAGER_HELP_GOOD",GROUP_SELECT_RANDOM_NO_REPEAT)
                }
                else {
                    if (T) return;
                    TS_SAY("TEXT_QST_002_VILLAGER_HELP_EVIL",GROUP_SELECT_RANDOM_NO_REPEAT)
                }
                if (seh_me.IsValid()) seh_me.Release();
            }
        }
        NSF;
    }
}
