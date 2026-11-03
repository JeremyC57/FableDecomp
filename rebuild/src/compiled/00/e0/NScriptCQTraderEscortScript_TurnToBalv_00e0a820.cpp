// NScript::CQ_TraderEscortScript::TurnToBalv(CScriptThing) -- stage the infected trader transformation and wait for the traders' response.
// Leaf: ?TurnToBalv@CQ_TraderEscortScript@NScript@@QAEXVCScriptThing@@@Z
// Flags: /Gr
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_STLP_MAP
// TU prelude (Codex pass 12 "fable_hanging_tree.h" context, inlined here: the header-contract inline bodies and the
// same-TU CCountedPointer<CScriptGameResourceObjectMovieBase> destructor stand-in this quest TU defines)
#define FABLE_RETAIL_NSCRIPT
#define FABLE_RETAIL_BASE_OBJECT
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
#include "engine/NScript_CQ_TraderEscortScript.h"
inline const C3DVector& CScriptThing::GetPos()const { if(PImp.IsNull())return NullCoord;return PImp->GetPos(); }
void NScript::CQ_TraderEscortScript::TurnToBalv(CScriptThing Me)
{
 CScriptGameResourceObjectScriptedThingBase seh_trader;
 CScriptGameResourceObjectScriptedThingBase seh_balv;
 CScriptThing Balv=Game->CreateCreature("CREATURE_BALVERINE_01",Me.GetPos(),"DarkwoodBalverineTrader",false);
 Game->EntitySetAsDrawable(Balv,false);
 if(seh_balv.IsValid())seh_balv.Release();
 while(!Game->StartScriptingEntity(Balv,seh_balv,SCRIPT_AI_PRIORITY_HIGHEST)) {
  Game->NewScriptFrame();
  if(IsActiveThreadTerminating())return;
 }
 if(IsActiveThreadTerminating())return;
 if(seh_trader.IsValid())seh_trader.Release();
 while(!Game->StartScriptingEntity(Me,seh_trader,SCRIPT_AI_PRIORITY_HIGHEST)) {
  Game->NewScriptFrame();
  if(IsActiveThreadTerminating())return;
 }
 if(IsActiveThreadTerminating())return;
 {
  CScriptGameResourceObjectScriptedThingBase seh_hero;
  Game->StartScriptingEntity(Game->GetHero(),seh_hero,SCRIPT_AI_PRIORITY_HIGHEST);
  std::map<CCharString,CScriptGameResourceObjectScriptedThingBase> mymap;
  mymap["HERO"]=seh_hero;
  mymap["BALV"]=seh_balv;
  mymap["TRADER"]=seh_trader;
  CScriptGameResourceObjectMovieBase speech_movie;
  Game->StartMovieSequence("",speech_movie);
  CWideScreenMagicPauseEntities PauseMyEntitiesPlease(*Game);
  RunCutsceneMacro("CS_DARKWOOD_TRADER_TRANSFORM",mymap,0,0,false,true);
 }
 Game->Pause(0.3f);
 seh_balv.Release();
 while(!MakeTraderComment("TRANSFORMED",CScriptThing(),TRADER_COMMENT_MULTIPLE)) {
  Game->NewScriptFrame();
  if(IsActiveThreadTerminating())return;
 }
 if(IsActiveThreadTerminating())return;
}