// NScript::CQ_MinionCampScript::CBriarRose::Main() -- Briar Rose in the minion camp: play the intro cut scene nearer the hero, start the parent's CheckBriarHit thread, show her health bar; in stage 2 follow her target until it is gone, then lead the hero to the altar (waiting for him when he falls behind); in stage 3 walk back to the altar when knocked off it, start the chant (cut scene, chant timer bar, RunChantTimer thread) once, and chant until the timer runs out (portal sealed), the hero hits her (pause the chant), she is hit (force push every few hits) or talked to; in stage 4 shatter and remove the minions, then the outro cut scene (or mission failed if she died)
// Leaf: ?Main@CBriarRose@CQ_MinionCampScript@NScript@@EAEXXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00DA0EF0, thiscall, no EH frame.
// Identity (proven, two sources): the address appears once in .rdata, at slot 1 of the entity vtable 0x012D6D0C stored by the
//   landed CBriarRose ctor 0x00D9E930 (Alloc 0x00D9E8B0; slot 0 the landed scalar deleting dtor 0x00D9FEB0, slot 2 the landed Init
//   0x00D9E9E0); Ego_r.pdb and FableWin.pdb both hold ?Main@CBriarRose@CQ_MinionCampScript@NScript@@EAEXXZ.
// Callees: SetPredicate 0x00F35B10 with the CIsThingAlive temporary (expanded ctor / dtor, CScriptThing copy 0x004ABE90 / ~
//   0x004AA840), IsActiveThreadTerminating 0x00F35B30, seh_me (IsValid 0x00CD23B9, Release 0x00CD2770, virtual MoveToPosition /
//   ClearCommands / PlayAnimation / PlayLoopingAnimation / ClearAllActions / IsPerformingScriptTask), the CBriarRose helpers
//   PlayCutscene 0x00DA2CA0, DoSpeech 0x00DA2610, DoOnHitSpeech 0x00DA26B0, DoOnSpeakToSpeech 0x00DA27B0, UpdateTarget 0x00DA2A20,
//   the parent threads CheckBriarHit 0x00DA2860 (CSpawnedFunc1, vtable 0x012D7064), RunChantTimer 0x00DA24F0 / PauseChantTimer
//   0x00DA2490 (CSpawnedFunc, vtable 0x012D6CAC) through CSpawnedFuncBase 0x00CDD450 and CScriptBase::AddSpawnedFunction 0x00CB7E50,
//   GetDistanceBetweenThings 0x00CBE512 / IsDistanceBetweenThingsUnder 0x00CBE2FF, CScriptThing operator= 0x008AB980,
//   ~vector<CScriptThing> 0x008AC970, rand 0x00BFEB16, CCharString 0x0099EBF0 / 0x0099E4B0 / ~ 0x0099EAE0, operator+ 0x0099F570;
//   interface slots through Game.
// Data: engine/CQ_MinionCampScript.h (new, chunk 809).
// Shape: generated entity script (T-check at the start of every nested block and after every WAIT loop); DoChant and
//   DoOnHeroHitSpeech (both PDB name sets, no retail copy) expanded.
#include "fable_compiler.h"
#define FABLE_RETAIL_NSCRIPT
#define FABLE_RETAIL_CLEVELSCRIPTBASE
#define FABLE_RETAIL_BASE_OBJECT
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CCHARSTRING_CONCAT_STRING
#define FABLE_RETAIL_SUSPENDABLE_PROCESS
#include "engine/C3DVector.h"
#include "engine/CRGBColour.h"
#include "engine/CQ_MinionCampScript.h"
#include "engine/CScriptGameResourceObjectScriptedThingBase.h"
#include "engine/CGameScriptInterfaceBase.h"
#include "engine/fable_script_distance.h"
#include "engine/fable_script_animation_defaults.h"
#include "engine/CSpawnedFunc.h"
#include "engine/CSpawnedFunc1.h"
#include "engine/CTimer.h"
#include "engine/TRetVoid.h"
#include "engine/CIsThingAlive.h"
enum EScriptAIPriority { SCRIPT_AI_PRIORITY_LOWEST=0, SCRIPT_AI_PRIORITY_LOW=1, SCRIPT_AI_PRIORITY_MEDIUM=2, SCRIPT_AI_PRIORITY_HIGH=3, SCRIPT_AI_PRIORITY_HIGHEST=4 };
enum EHeroAbility { HERO_ABILITY_NULL=0, HERO_ABILITY_HEAL_LIFE_SPELL=14 };
enum EScriptEntityMoveType { MOVE_TYPE_WALKING=0, MOVE_TYPE_RUNNING=1 };
extern "C" int __cdecl rand();
extern CGameScriptInterfaceBase* pDSTGame;
float FABLE_FASTCALL GetDistanceBetweenThings(const CScriptThing&, const CScriptThing&);    // 0x00CBE512
namespace std {
template<class T> class allocator { };
template<class T, class A> class vector {
public:
    T* _M_start;
    T* _M_finish;
    T* _M_end_of_storage;
    vector() : _M_start(0), _M_finish(0), _M_end_of_storage(0) { }
    ~vector();                                                           // 0x008AC970 for CScriptThing
    unsigned int size() const { return (unsigned int)(_M_finish - _M_start); }
    T& operator[](unsigned int n) { return _M_start[n]; }
};
}
FABLE_FORCEINLINE long CTimer::GetTime() const { return pDSTGame->GetTimer(TimerIndex); }
inline NScript::CIsThingAlive::CIsThingAlive(const CScriptThing& thing):Thing(thing) {}
inline NScript::CIsThingAlive::~CIsThingAlive() { }
FABLE_FORCEINLINE void NScript::CQ_MinionCampScript::CBriarRose::DoChant()
{
    seh_me.PlayAnimation("ST_CHANTING_INTO",false,false,false,true,NGlobalConsole::GDefaultToUsingPhysicsInCutsceneAnimations,false,false);
    seh_me.PlayLoopingAnimation("ST_CHANTING_LOOP",-1,false,false,false,true,NGlobalConsole::GDefaultToUsingPhysicsInCutsceneAnimations,false,false);
    IsPlayingChantAnim=true;
}
inline void NScript::CQ_MinionCampScript::CBriarRose::DoOnHeroHitSpeech()
{
    DoSpeech("TEXT_QST_B08_BRIAR_ONHEROHIT");
}
void NScript::CQ_MinionCampScript::CBriarRose::Main()
{
    TRetVoid _Fail_Val;
    SetPredicate(CIsThingAlive(Me));
    Game->NewScriptFrame();
    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
    if (seh_me.IsValid()) seh_me.Release();
    while (!Game->StartScriptingEntity(Me,seh_me,SCRIPT_AI_PRIORITY_HIGHEST)) {
        Game->NewScriptFrame();
        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
    }
    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
    if (!ParentClass.IntroDone) {
        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
        if (GetDistanceBetweenThings(Game->GetHero(),Game->GetThingWithScriptName("MK_MCIU_ROSE1"))>GetDistanceBetweenThings(Game->GetHero(),Game->GetThingWithScriptName("MK_MCIL_ROSE"))) {
            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
            PlayCutscene("CS_MINIONCAMP_INTRO_LOWER");
        } else {
            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
            PlayCutscene("CS_MINIONCAMP_INTRO_UPPER");
        }
        seh_me.Release();
        ParentClass.IntroDone=true;
    }
    ParentClass.AddSpawnedFunction(new CSpawnedFunc1<CQ_MinionCampScript,CScriptThing>(CCharString("ParentClass.")+CCharString("CheckBriarHit"),0,&ParentClass,&CQ_MinionCampScript::CheckBriarHit,Me),CCharString(""));
    while (!(ParentClass.CurrentStage>1)) {
        Game->NewScriptFrame();
        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
    }
    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
    Game->AddQuestInfoBarHealth(Me,CRGBColour(255,0,0,255),"HUD_QUEST_ICON_BRIAR_ROSE",1.0f);
    Game->DisplayQuestInfo(true);
    if (ParentClass.CurrentStage==2) {
        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
        while (UpdateTarget()) {
            Game->NewScriptFrame();
            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
            if (Me.MsgIsHitBy("SCRIPT_NAME_HERO") || (Me.MsgIsHitByAnySpecialAbilityFrom("SCRIPT_NAME_HERO") && !Me.MsgIsHitBySpecialAbilityFrom(HERO_ABILITY_HEAL_LIFE_SPELL,"SCRIPT_NAME_HERO"))) {
                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                CCharString Line("TEXT_QST_B08_BRIAR_ONHIT_STAGEONE");
                long conversation=Game->AddNewConversation(Me,false,false);
                Game->AddPersonToConversation(conversation,Game->GetHero());
                Game->AddLineToConversation(conversation,Line,false,Me,Game->GetHero());
            }
        }
        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
        MyDestination=Game->GetThingWithScriptName("MC_Altar");
        ParentClass.BriarState=(BriarRoseState)1;
        if (seh_me.IsValid()) seh_me.Release();
        while (!Game->StartScriptingEntity(Me,seh_me,SCRIPT_AI_PRIORITY_HIGHEST)) {
            Game->NewScriptFrame();
            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
        }
        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
        Game->EntitySheatheWeapons(Me,false);
        bool SaidFollow=false;
        long MoveState=0;
        seh_me.ClearCommands();
        seh_me.MoveToPosition(MyDestination.GetPos(),1.0f,MOVE_TYPE_RUNNING,false,true);
        while (!IsDistanceBetweenThingsUnder(Me,MyDestination,2.0f)) {
            Game->NewScriptFrame();
            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
            bool HeroFurther=GetDistanceBetweenThings(Me,MyDestination)<GetDistanceBetweenThings(Game->GetHero(),MyDestination);
            if (!IsDistanceBetweenThingsUnder(Me,Game->GetHero(),9.0f) && HeroFurther) {
                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                seh_me.ClearCommands();
                seh_me.ClearAllActions();
                if (!SaidFollow) {
                    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                    Game->EntitySetFacingAngleTowardsThing(Me,Game->GetHero(),false);
                    long conversation=Game->AddNewConversation(Me,false,false);
                    Game->AddPersonToConversation(conversation,Game->GetHero());
                    Game->AddLineToConversation(conversation,"TEXT_QST_B08_BRIAR_FOLLOW",false,Me,Game->GetHero());
                    Game->Pause(1.0f);
                    SaidFollow=true;
                    MoveState=2;
                }
            } else {
                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                if (!IsDistanceBetweenThingsUnder(Me,Game->GetHero(),5.0f) && HeroFurther) {
                    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                    if (MoveState!=1) {
                        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                        seh_me.ClearCommands();
                        seh_me.MoveToPosition(MyDestination.GetPos(),1.0f,MOVE_TYPE_WALKING,false,true);
                        MoveState=1;
                    }
                } else {
                    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                    if (MoveState!=0) {
                        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                        seh_me.ClearCommands();
                        seh_me.MoveToPosition(MyDestination.GetPos(),1.0f,MOVE_TYPE_RUNNING,false,true);
                        MoveState=0;
                    }
                }
                SaidFollow=false;
            }
        }
        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
        ParentClass.BriarRoseAtAltar=true;
        while (ParentClass.CurrentStage==2) {
            Game->NewScriptFrame();
            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
        }
        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
    }
    CScriptThing Shrine=Game->GetThingWithScriptName("MK_MCO_SHRINE");
    CCharString UnusedString;
    while (ParentClass.CurrentStage==3) {
        Game->NewScriptFrame();
        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
        if (ParentClass.BriarState==1) {
            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
            MyDestination=Game->GetThingWithScriptName("MC_Altar");
            seh_me.MoveToPosition(MyDestination.GetPos(),0.5f,MOVE_TYPE_RUNNING,false,true);
            while (seh_me.IsPerformingScriptTask()) {
                Game->NewScriptFrame();
                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
            }
            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
            Game->EntitySetFacingAngleTowardsThing(Me,Shrine,true);
            ParentClass.BriarState=(BriarRoseState)3;
        }
        if (!ParentClass.MinionsSummoned) {
            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
            while (!IsDistanceBetweenThingsUnder(Game->GetHero(),Me,4.0f)) {
                Game->NewScriptFrame();
                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
            }
            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
            if (Game->GetHealth(Me)>0.0f) {
                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                PlayCutscene("CS_MINIONCAMP_CHANT_START");
            }
            ParentClass.ChantTimerBar=Game->AddQuestInfoBar(72.0f,0.0f,CRGBColour(0,255,0,255),CRGBColour(255,0,0,255),"","",1.0f);
            Game->UpdateQuestInfoBar(ParentClass.ChantTimerBar,0.0f,-1.0f,-1.0f);
            ParentClass.AddSpawnedFunction(new CSpawnedFunc<CQ_MinionCampScript>(CCharString("ParentClass.")+CCharString("RunChantTimer"),0,&ParentClass,&CQ_MinionCampScript::RunChantTimer),CCharString(""));
            DoChant();
            DoSpeech("TEXT_QST_B08_BRIAR_BEGIN_CHANTING_10");
            ParentClass.MinionsSummoned=true;
        }
        Game->EntityForceToLookAtThing(Me,Shrine);
        long HitCount=0;
        while (ParentClass.BriarState==3) {
            Game->NewScriptFrame();
            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
            if (ParentClass.ChantTimer.GetTime()<=0 && !ParentClass.ChantTimerPaused) {
                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                Game->EntitySetAsKillable(Me,false,true);
                Game->EntitySetAsDamageable(Me,false);
                ParentClass.PortalSealed=true;
                ParentClass.BriarState=(BriarRoseState)4;
                IsPlayingChantAnim=false;
            } else if (Me.MsgIsHitBy("SCRIPT_NAME_HERO") || (Me.MsgIsHitByAnySpecialAbilityFrom("SCRIPT_NAME_HERO") && !Me.MsgIsHitBySpecialAbilityFrom(HERO_ABILITY_HEAL_LIFE_SPELL,"SCRIPT_NAME_HERO"))) {
                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                DoOnHeroHitSpeech();
                Game->EntitySetThingAsAllyOfThing(Me,Game->GetHero());
                Game->EntitySetThingAsAllyOfThing(Game->GetHero(),Me);
                Game->Pause(1.0f);
                ParentClass.BriarState=(BriarRoseState)1;
                IsPlayingChantAnim=false;
                ParentClass.AddSpawnedFunction(new CSpawnedFunc<CQ_MinionCampScript>(CCharString("ParentClass.")+CCharString("PauseChantTimer"),0,&ParentClass,&CQ_MinionCampScript::PauseChantTimer),CCharString(""));
            } else if (Me.MsgIsHitBy("")) {
                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                DoOnHitSpeech();
                if (++HitCount>=NumHitsBetweenForcePushes) {
                    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                    Game->EntitySetAsDamageable(Me,false);
                    Game->EntitySetAsKillable(Me,false,true);
                    while (!Game->EntityCastForcePush(Me,false)) {
                        Game->NewScriptFrame();
                        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                    }
                    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                    Game->Pause(1.0f);
                    Game->EntitySetAsDamageable(Me,true);
                    Game->EntitySetAsKillable(Me,true,true);
                    HitCount=0;
                }
                ParentClass.BriarState=(BriarRoseState)1;
                IsPlayingChantAnim=false;
                ParentClass.AddSpawnedFunction(new CSpawnedFunc<CQ_MinionCampScript>(CCharString("ParentClass.")+CCharString("PauseChantTimer"),0,&ParentClass,&CQ_MinionCampScript::PauseChantTimer),CCharString(""));
            } else if (Me.MsgIsTalkedToBy("SCRIPT_NAME_HERO")) {
                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                DoOnSpeakToSpeech();
                IsPlayingChantAnim=false;
            } else if (rand()%400==0) {
                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                DoSpeech("TEXT_QST_B08_BRIAR_CHANTING");
                IsPlayingChantAnim=false;
            } else if (!IsPlayingChantAnim) {
                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                DoChant();
            }
        }
        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
    }
    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
    if (ParentClass.CurrentStage==4) {
        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
        std::vector<CScriptThing,std::allocator<CScriptThing> > Minions;
        Game->GetAllThingsWithScriptName("MC_Minion",Minions);
        for (unsigned int i=0;i<Minions.size();i++) {
            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
            Game->CreateEffect("MINION_DREADWING_SHATTER_01",Minions[i].GetPos(),"",0.0f,false,false);
            Game->RemoveThing(Minions[i],false,true);
        }
        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
        Game->Pause(1.0f);
        if (Game->GetHealth(Me)>0.0f) {
            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
            PlayCutscene("CS_MINIONCAMP_OUTRO");
            ParentClass.CurrentStage=(MissionStage)5;
        } else {
            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
            ParentClass.MissionFailed=true;
        }
    }
    while (true) {
        Game->NewScriptFrame();
        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
    }
}