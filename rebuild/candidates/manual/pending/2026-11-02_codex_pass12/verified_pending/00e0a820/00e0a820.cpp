// NScript::CQ_TraderEscortScript::TurnToBalv(CScriptThing) -- stage the infected trader transformation and wait for the traders' response.
// Leaf: ?TurnToBalv@CQ_TraderEscortScript@NScript@@QAEXVCScriptThing@@@Z
// Flags: /Gr
#define FABLE_RETAIL_VENDOR_VECTOR_ORDINARY
#define FABLE_RETAIL_ENDGAME_CUTSCENE
#include "engine/fable_hanging_tree.h"
#include "engine/NScript_CQ_TraderEscortScript.h"
inline const C3DVector& CScriptThing::GetPos()const { if(PImp.IsNull())return NullCoord;return PImp->GetPos(); }
void NScript::CQ_TraderEscortScript::TurnToBalv(CScriptThing Me)
{
 CScriptGameResourceObjectScriptedThingBase seh_trader;
 CScriptGameResourceObjectScriptedThingBase seh_balv;
 CScriptThing Balv=Game.CreateCreature("CREATURE_BALVERINE_01",Me.GetPos(),"DarkwoodBalverineTrader",false);
 Game.EntitySetAsDrawable(Balv,false);
 if(seh_balv.IsValid())seh_balv.Release();
 while(!Game.StartScriptingEntity(Balv,seh_balv,SCRIPT_AI_PRIORITY_HIGHEST)) {
  Game.NewScriptFrame();
  if(IsActiveThreadTerminating())return;
 }
 if(IsActiveThreadTerminating())return;
 if(seh_trader.IsValid())seh_trader.Release();
 while(!Game.StartScriptingEntity(Me,seh_trader,SCRIPT_AI_PRIORITY_HIGHEST)) {
  Game.NewScriptFrame();
  if(IsActiveThreadTerminating())return;
 }
 if(IsActiveThreadTerminating())return;
 {
  CScriptGameResourceObjectScriptedThingBase seh_hero;
  Game.StartScriptingEntity(Game.GetHero(),seh_hero,SCRIPT_AI_PRIORITY_HIGHEST);
  std::map<CCharString,CScriptGameResourceObjectScriptedThingBase> mymap;
  mymap["HERO"]=seh_hero;
  mymap["BALV"]=seh_balv;
  mymap["TRADER"]=seh_trader;
  CScriptGameResourceObjectMovieBase speech_movie;
  Game.StartMovieSequence("",speech_movie);
  CWideScreenMagicPauseEntities PauseMyEntitiesPlease(Game);
  RunCutsceneMacro("CS_DARKWOOD_TRADER_TRANSFORM",mymap,0,0,false,true);
 }
 Game.Pause(0.3f);
 seh_balv.Release();
 while(!MakeTraderComment("TRANSFORMED",CScriptThing(),TRADER_COMMENT_MULTIPLE)) {
  Game.NewScriptFrame();
  if(IsActiveThreadTerminating())return;
 }
 if(IsActiveThreadTerminating())return;
}
