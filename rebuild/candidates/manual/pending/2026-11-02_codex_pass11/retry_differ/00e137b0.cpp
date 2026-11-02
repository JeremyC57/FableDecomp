// NScript::CQ_WaspBossScript::Outro() -- clean up the queen encounter and restore picnic-area villagers.
// Leaf: ?Outro@CQ_WaspBossScript@NScript@@QAEXXZ
// Grade: DIFFER(1004v1080) -- the CCountedPointer resource assignment stays an out-of-line call where retail expands it (76 bytes short), so frame and handle slots shift; 10 shapes (cons11 wasp TU, in-place ShareData operator= diagnostic, TRetVoid/plain returns, map slot reference/placement, IsAlive form, nested else-return, pause-entities order, default-then-assign) leave it
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_NSCRIPT
#define FABLE_RETAIL_BASE_OBJECT
enum EScriptAIPriority { SCRIPT_AI_PRIORITY_LOWEST,SCRIPT_AI_PRIORITY_LOW,SCRIPT_AI_PRIORITY_MEDIUM,SCRIPT_AI_PRIORITY_HIGH,SCRIPT_AI_PRIORITY_HIGHEST };
#include "fable_string.h"
#include "engine/CCountedPointer.h"
#include "engine/CScriptThing.h"
#include "engine/CScriptGameResourceObjectScriptedThingBase.h"
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
#include "engine/NScript_CQ_WaspBossScript.h"
#include "stlport/fable_map.h"
inline CScriptThing::CScriptThing() { }
inline CScriptThing::~CScriptThing() { }
inline CScriptThing& CScriptThing::operator=(const CScriptThing& other) { PImp=other.PImp; return *this; }
inline bool CScriptThing::IsAlive()const{return !PImp.IsNull() && PImp->IsAlive();}
inline bool CScriptThing::MsgIsKilledBy(const CCharString& name)const{if(PImp.IsNull())return false;return PImp->MsgIsKilledBy(name);}
void __fastcall RunCutsceneMacro(const CCharString&,std::map<CCharString,CScriptGameResourceObjectScriptedThingBase>&,std::map<CCharString,bool>*,std::map<CCharString,CCharString>*,bool,bool);
#include "engine/TRetVoid.h"
void NScript::CQ_WaspBossScript::Outro()
{
 TRetVoid _Fail_Val;
 Game->NewScriptFrame();
 if(IsActiveThreadTerminating())return _Fail_Val.Return();
 PanickedVillagersScene=true;
 {
  CScriptGameResourceObjectScriptedThingBase seh_hero;
  Game->StartScriptingEntity(Game->GetHero(),seh_hero,SCRIPT_AI_PRIORITY_HIGHEST);
  CScriptGameResourceObjectMovieBase speech_movie;
  Game->StartMovieSequence("",speech_movie);
  CWideScreenMagicPauseEntities PauseMyEntitiesPlease(*Game);
  Game->Pause(3.0f);
  Game->FadeScreenOut(0.5f,0.5f,CRGBColour(0,0,0,255));
  Game->Pause(0.5f);
  CScriptThing Queen=Game->GetNearestWithDefName(Game->GetThingWithScriptName("QueenDepositPos1"),"CREATURE_HORNET_QUEEN_01");
  if(Queen.IsAlive()) {
   if(IsActiveThreadTerminating())return _Fail_Val.Return();
   Game->RemoveThing(Queen,false,true);
  }
  std::map<CCharString,CScriptGameResourceObjectScriptedThingBase> mymap;
  mymap["HERO"]=seh_hero;
  RunCutsceneMacro("CS_WASPBOSS_OUTRO",mymap,0,0,false,true);
 }
 Game->EntitySetInFaction(Game->GetThingWithScriptName("VILL1"),"FACTION_PICNIC_AREA");
 Game->EntitySetInFaction(Game->GetThingWithScriptName("VILL2"),"FACTION_PICNIC_AREA");
 Game->EntitySetInFaction(Game->GetThingWithScriptName("VILL3"),"FACTION_PICNIC_AREA");
}
