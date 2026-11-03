// NScript::CV_ChickenKickingScript::CSpectator::Main() -- greet spectators and flee when attacked.
// Leaf: ?Main@CSpectator@CV_ChickenKickingScript@NScript@@EAEXXZ
// Flags: /Gr
#define FABLE_RETAIL_CCHARSTRING
#include "engine/EHeroAbility.h"
#include "engine/EScriptingStateGroups.h"
#include "engine/ETextGroupSelectionMethod.h"
#include "engine/ECutsceneBehaviour.h"
// TU prelude (Codex pass 12 "fable_hanging_tree.h" context, inlined here: the header-contract inline bodies and the
// same-TU CCountedPointer<CScriptGameResourceObjectMovieBase> destructor stand-in this quest TU defines)
#define FABLE_RETAIL_NSCRIPT
#define FABLE_RETAIL_BASE_OBJECT
#define FABLE_STLPORT_HTTP
#define FABLE_STLPORT_CONSTRUCTORS
#define FABLE_RETAIL_SUSPENDABLE_PROCESS
enum EScriptAIPriority { SCRIPT_AI_PRIORITY_LOWEST,SCRIPT_AI_PRIORITY_LOW,SCRIPT_AI_PRIORITY_MEDIUM,SCRIPT_AI_PRIORITY_HIGH,SCRIPT_AI_PRIORITY_HIGHEST };
#include "engine/C3DVector.h"
#include "fable_string.h"
#include "engine/CCountedPointer.h"
#include "engine/CScriptThing.h"
#include "engine/CScriptGameResourceObjectScriptedThingBase.h"
class CScriptGameResourceObjectMovieBase;
#pragma auto_inline(off)
template<> CCountedPointer<CScriptGameResourceObjectMovieBase>::~CCountedPointer()
{
    if(Info){--Info->RefCount;if(Info->RefCount==0){Info->DeleteFunc(Info->Data);delete Info;}}
    Data=0;Info=0;
}
#pragma auto_inline(on)
#include "engine/CScriptGameResourceObjectMovieBase.h"
#include "engine/CRGBColour.h"
#include "engine/CGameScriptInterfaceBase.h"
#include "engine/CWideScreenMagicPauseEntities.h"
#include "engine/CSpawnedFunc.h"
#include "stlport/fable_map.h"
inline CScriptThing::CScriptThing() { }
inline CScriptThing::~CScriptThing() { }
inline bool CScriptThing::IsAlive()const{if(PImp.IsNull())return false;return PImp->IsAlive();}
void __fastcall RunCutsceneMacro(const CCharString&,std::map<CCharString,CScriptGameResourceObjectScriptedThingBase>&,std::map<CCharString,bool>*,std::map<CCharString,CCharString>*,bool,bool);
#include "engine/NScript_CV_ChickenKickingScript.h"
#include "engine/TRetVoid.h"
CCharString __fastcall GFIntToCharString(long);
CCharString __fastcall operator+(const char*,const CCharString&);
CCharString __fastcall operator+(const CCharString&,const char*);
inline CScriptThing::CScriptThing(const CScriptThing& other):CBaseClass(other),PImp(other.PImp){}
inline const C3DVector& CScriptThing::GetPos()const{if(PImp.IsNull())return NullCoord;return PImp->GetPos();}
void NScript::CV_ChickenKickingScript::CSpectator::Main()
{
 CScriptGameResourceObjectScriptedThingBase seh_me;
 TRetVoid _Fail_Val;
 bool HaveTalked=false;
 Game->SetIsPushableByHero(Me,false);
 Game->SetCreatureBrain(Me,"BRAIN_PASSIVE_OVERRIDE");
 Game->SetWanderCentrePoint(Me,Me.GetPos());
 Game->SetWanderMinDistance(Me,0.0f);
 Game->SetWanderMaxDistance(Me,10.0f);
 Game->SetScriptingStateGroup(Me,ESSG_WANDER_NEAR);
 while(!ParentClass.SpectatorsUnderAttack) {
  Game->NewScriptFrame();if(IsActiveThreadTerminating())return _Fail_Val.Return();
  if(Me.MsgIsTalkedToBy("SCRIPT_NAME_HERO")) {
   if(IsActiveThreadTerminating())return _Fail_Val.Return();
   Game->EntitySetCutsceneBehaviour(Me,CUTSCENE_BEHAVIOUR_NOT_PAUSED);
   if(seh_me.IsValid())seh_me.Release();
   while(!Game->StartScriptingEntity(Me,seh_me,SCRIPT_AI_PRIORITY_HIGHEST)) {
    Game->NewScriptFrame();if(IsActiveThreadTerminating())return _Fail_Val.Return();
   }
   if(IsActiveThreadTerminating())return _Fail_Val.Return();
   CScriptGameResourceObjectMovieBase speech_movie;
   Game->StartMovieSequence("",speech_movie);
   CWideScreenMagicPauseEntities PauseMyEntitiesPlease(*Game);
   if(!HaveTalked) {
    if(IsActiveThreadTerminating())return _Fail_Val.Return();
    CCharString Dialogue="TEXT_QST_B17_SPECTATOR_"+GFIntToCharString(SpectatorNumber);
    Dialogue=Dialogue+"_GREETING";
    if(Game->GetHealth(seh_me.GetScriptThing())>0.0f) {
     seh_me.Speak(Game->GetHero(),Dialogue,GROUP_SELECT_NONE,false,true,false);
     while(seh_me.IsPerformingScriptTask()!=false){Game->NewScriptFrame();if(IsActiveThreadTerminating())return _Fail_Val.Return();}
     if(IsActiveThreadTerminating())return _Fail_Val.Return();
    }
    HaveTalked=true;
   } else {
    if(IsActiveThreadTerminating())return _Fail_Val.Return();
    CCharString Dialogue="TEXT_QST_B17_SPECTATOR_"+GFIntToCharString(SpectatorNumber);
    Dialogue=Dialogue+"_TIP";
    if(Game->GetHealth(seh_me.GetScriptThing())>0.0f) {
     seh_me.Speak(Game->GetHero(),Dialogue,GROUP_SELECT_RANDOM_NO_REPEAT,false,true,false);
     while(seh_me.IsPerformingScriptTask()!=false){Game->NewScriptFrame();if(IsActiveThreadTerminating())return _Fail_Val.Return();}
     if(IsActiveThreadTerminating())return _Fail_Val.Return();
    }
   }
   if(seh_me.IsValid())seh_me.Release();
  }
  if(Me.MsgIsHitBy("SCRIPT_NAME_HERO")||(Me.MsgIsHitByAnySpecialAbilityFrom("SCRIPT_NAME_HERO")&&!Me.MsgIsHitBySpecialAbilityFrom(HERO_ABILITY_HEAL_LIFE_SPELL,"SCRIPT_NAME_HERO"))) {
   if(IsActiveThreadTerminating())return _Fail_Val.Return();
   ParentClass.SpectatorsUnderAttack=true;
  }
 }
 if(IsActiveThreadTerminating())return _Fail_Val.Return();
 Game->EntitySetAsScared(Me,true);
 Game->SetIsPushableByHero(Me,true);
 Game->SetCreatureBrain(Me,"BRAIN_GOOD_VILLAGER_BASE");
 for(;;){Game->NewScriptFrame();if(IsActiveThreadTerminating())return _Fail_Val.Return();}
}