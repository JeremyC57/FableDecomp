// NScript::CV_ParanoidWhispersScript::CSecretWhisperingMan::Main() -- coordinate whispered clues, suspicion and hostility.
// Leaf: ?Main@CSecretWhisperingMan@CV_ParanoidWhispersScript@NScript@@EAEXXZ
#define FABLE_RETAIL_CTIMER
#define FABLE_RETAIL_CISTHINGALIVE
#define FABLE_RETAIL_NSCRIPT
#define FABLE_RETAIL_CENTITYSCRIPTBASE
#define FABLE_RETAIL_CLEVELSCRIPTBASE
#define FABLE_RETAIL_BASE_OBJECT
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CSCRIPTTHING
#define FABLE_RETAIL_CSCRIPTGAMERESOURCEOBJECTSCRIPTEDTHINGBASE
#include "engine/CV_ParanoidWhispersScript.h"
#include "engine/CScriptGameResourceObjectScriptedThingBase.h"
#include "engine/CGameScriptInterfaceBase.h"
#include "engine/CIsThingAlive.h"
#include "engine/CTimer.h"
#include "engine/TRetVoid.h"
#include "engine/fable_script_distance.h"
enum EScriptAIPriority { SCRIPT_AI_PRIORITY_LOWEST=0, SCRIPT_AI_PRIORITY_LOW=1, SCRIPT_AI_PRIORITY_MEDIUM=2, SCRIPT_AI_PRIORITY_HIGH=3, SCRIPT_AI_PRIORITY_HIGHEST=4 };
enum EScriptEntityMoveType { ENTITY_MOVE_WALK=0, ENTITY_MOVE_RUN=1, ENTITY_MOVE_SNEAK=2 };
extern CGameScriptInterfaceBase* pDSTGame;
inline CTimer::CTimer() { TimerIndex=pDSTGame->RegisterTimer(); }
inline CTimer::~CTimer() { pDSTGame->DeregisterTimer(TimerIndex); }
__forceinline CTimer& CTimer::operator=(long time) { pDSTGame->SetTimer(TimerIndex,time); return *this; }
inline long CTimer::GetTime() const { return pDSTGame->GetTimer(TimerIndex); }
__forceinline bool CTimer::operator==(long time) const { return GetTime()==time; }
inline NScript::CIsThingAlive::CIsThingAlive(const CScriptThing& thing):Thing(thing) {}
inline CScriptThing::~CScriptThing() {}
bool FABLE_FASTCALL IsDistanceFromThingToPositionOver(const CScriptThing&,const C3DVector&,float);
void NScript::CV_ParanoidWhispersScript::CSecretWhisperingMan::Main()
{
    TRetVoid _Fail_Val;
    CScriptThing Friend=Game.GetThingWithScriptName("SecretWhisperingFriend");
    SetPredicate(CIsThingAlive(Me));
    Game.NewScriptFrame();
    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
    CScriptGameResourceObjectScriptedThingBase seh_me;
    if (seh_me.IsValid()) seh_me.Release();
    while (!Game.StartScriptingEntity(Me,seh_me,SCRIPT_AI_PRIORITY_HIGHEST)) {
        Game.NewScriptFrame();
        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
    }
    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
    CTimer TalkTimer;
    if (Game.IsQuestActive("Q_HeroSoulsMother")) {
        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
        Game.RemoveThing(Me,false,false);
    }
    for (; !false; Game.NewScriptFrame()) {
        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
        if (ParentClass.ParanoidState==PS_ANGRY) {
            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
            Game.GiveThingBestEnemyTarget(Me,Game.GetHero());
            if (seh_me.IsValid()) seh_me.Release();
            while (ParentClass.ParanoidState==PS_ANGRY) {
                Game.NewScriptFrame();
                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
            }
            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
            if (seh_me.IsValid()) seh_me.Release();
            while (!Game.StartScriptingEntity(Me,seh_me,SCRIPT_AI_PRIORITY_HIGHEST)) {
                Game.NewScriptFrame();
                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
            }
            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
        }
        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
        while (ParentClass.ParanoidState==PS_REPOSITION) {
            Game.NewScriptFrame();
            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
            C3DVector HomePos=Me.GetHomePos();
            C3DVector MyPos=Me.GetPos();
            while (IsDistanceFromThingToPositionOver(Me,StartPos,0.5f) && ParentClass.ParanoidState==PS_REPOSITION) {
                Game.NewScriptFrame();
                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                if (!seh_me.IsPerformingScriptTask()) {
                    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                    seh_me.MoveToPosition(StartPos,0,ENTITY_MOVE_WALK,false,false);
                }
            }
            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
            if (ParentClass.ParanoidState!=PS_ANGRY) {
                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                seh_me.ClearCommands();
                Game.EntitySetFacingAngleTowardsThing(Me,Game.GetThingWithScriptName("M_SecretWhisperLookAtPos"),false);
                while (ParentClass.ParanoidState==PS_REPOSITION) {
                    Game.NewScriptFrame();
                    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                }
                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
            }
        }
        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
        while (ParentClass.ParanoidState==PS_WHISPERING) {
            Game.NewScriptFrame();
            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
            if (TalkTimer==0 && IsDistanceBetweenThingsUnder(Me,Game.GetHero(),12.0f)) {
                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                TalkTimer=5;
                long HLStyleConversationIndex=Game.AddNewConversation(Me,false,false);
                Game.AddPersonToConversation(HLStyleConversationIndex,Friend);
                Game.AddLineToConversation(HLStyleConversationIndex,"TEXT_QST_054_WHISPERING",false,Me,Friend);
            }
            if (IsDistanceBetweenThingsUnder(Me,Game.GetHero(),5.0f)) {
                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                if (Game.CanThingBe_Seen_ByOtherThing(Game.GetHero(),Me)) {
                    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                    long HLStyleConversationIndex=Game.AddNewConversation(Me,false,false);
                    Game.AddPersonToConversation(HLStyleConversationIndex,Friend);
                    Game.AddLineToConversation(HLStyleConversationIndex,"TEXT_QST_054_HERO_SPOTTED",false,Me,Friend);
                    ParentClass.ParanoidState=PS_WHISTLING;
                } else if (Game.CanThingBe_Heard_ByOtherThing(Game.GetHero(),Me)) {
                    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                    long HLStyleConversationIndex=Game.AddNewConversation(Me,false,false);
                    Game.AddPersonToConversation(HLStyleConversationIndex,Friend);
                    Game.AddLineToConversation(HLStyleConversationIndex,"TEXT_QST_054_HERO_HEARD",false,Me,Friend);
                    ParentClass.ParanoidState=PS_WHISTLING;
                } else {
                    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                    ParentClass.ParanoidState=PS_EXPLAINING_ALL;
                    ParentClass.HideDiggingSpot=false;
                    Game.SetQuestCardObjective("V_MayorsInvitation","TEXT_QUEST_MAYORS_INVITATION_OBJECTIVE_05","OakBay","");
                }
            }
        }
        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
        TalkTimer=9;
        while (ParentClass.ParanoidState==PS_WHISTLING) {
            Game.NewScriptFrame();
            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
            if (TalkTimer==0 && IsDistanceBetweenThingsUnder(Me,Game.GetHero(),10.0f)) {
                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                TalkTimer=9;
                long HLStyleConversationIndex=Game.AddNewConversation(Me,false,false);
                Game.AddPersonToConversation(HLStyleConversationIndex,Friend);
                Game.AddLineToConversation(HLStyleConversationIndex,"TEXT_QST_054_PRETENDING",false,Me,Friend);
            } else if (IsDistanceBetweenThingsOver(Me,Game.GetHero(),10.0f)) {
                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                long HLStyleConversationIndex=Game.AddNewConversation(Me,false,false);
                Game.AddPersonToConversation(HLStyleConversationIndex,Friend);
                Game.AddLineToConversation(HLStyleConversationIndex,"TEXT_QST_054_REVERT_TO_WHISPERING",false,Me,Friend);
                ParentClass.ParanoidState=PS_WHISPERING;
            }
        }
        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
        while (ParentClass.ParanoidState==PS_EXPLAINING_ALL) {
            Game.NewScriptFrame();
            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
            long HLStyleConversationIndex=Game.AddNewConversation(Me,false,false);
            Game.AddPersonToConversation(HLStyleConversationIndex,Friend);
            Game.AddLineToConversation(HLStyleConversationIndex,"TEXT_QST_054_GIVING_CLUES_01",false,Me,Friend);
            Game.AddLineToConversation(HLStyleConversationIndex,"TEXT_QST_054_GIVING_CLUES_02",false,Friend,Me);
            Game.AddLineToConversation(HLStyleConversationIndex,"TEXT_QST_054_GIVING_CLUES_03",false,Me,Friend);
            Game.AddLineToConversation(HLStyleConversationIndex,"TEXT_QST_054_GIVING_CLUES_04",false,Friend,Me);
            Game.AddLineToConversation(HLStyleConversationIndex,"TEXT_QST_054_GIVING_CLUES_05",false,Me,Friend);
            Game.AddLineToConversation(HLStyleConversationIndex,"TEXT_QST_054_GIVING_CLUES_06",false,Friend,Me);
            Game.AddLineToConversation(HLStyleConversationIndex,"TEXT_QST_054_GIVING_CLUES_07",false,Me,Friend);
            Game.Pause(2.0f);
            while (ParentClass.ParanoidState==PS_EXPLAINING_ALL && !IsDistanceBetweenThingsOver(Me,Game.GetHero(),10.0f) && !Game.CanThingBe_Seen_ByOtherThing(Game.GetHero(),Me) && !Game.CanThingBe_Heard_ByOtherThing(Game.GetHero(),Me)) {
                Game.NewScriptFrame();
                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
            }
            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
            if (ParentClass.ParanoidState!=PS_ANGRY) {
                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                ParentClass.ParanoidState=PS_WHISPERING;
            }
            Game.RemoveConversation(HLStyleConversationIndex,false);
        }
        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
    }
    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
}
