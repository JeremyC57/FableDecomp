// NScript::CV_FisticuffsClubScript::CTyler::FightWarmupRounds() -- stage the club opponents and count scored hits.
// Leaf: ?FightWarmupRounds@CTyler@CV_FisticuffsClubScript@NScript@@AAEXXZ
// Flags: /Gr
#define FABLE_RETAIL_CRESOURCE_PRIVATE_MEMBERS
#define FABLE_RETAIL_IVREFCOUNT_PRIVATE
#define FABLE_RETAIL_VENDOR_VECTOR_ORDINARY
#define FABLE_RETAIL_DEFPTR_ORDINARY_INLINE
#define FABLE_RETAIL_FISTICUFFS
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_PDCB_TYPED_MEMBERS
#include <string.h>
#include "engine/EOpinionDeedType.h"
#include "engine/fable_hanging_tree.h"
#include "engine/NScript_CV_FisticuffsClubScript.h"
#include "engine/CDefString.h"
#include "engine/CVectorMap.h"
#include "engine/CDefPointer.h"
#include "engine/CScriptDef.h"
#include "engine/TRetVoid.h"
extern CDefPointer<const CScriptDef> pGlobalScriptDef;
inline CScriptThing::CScriptThing(const CScriptThing& other):CBaseClass(other),PImp(other.PImp){}
inline CScriptThing& CScriptThing::operator=(const CScriptThing& other){PImp=other.PImp;return *this;}
void NScript::CV_FisticuffsClubScript::CTyler::FightWarmupRounds()
{
 TRetVoid _Fail_Val;
 CCharString FightList[5]={"CREATURE_BS_FISTICUFFS_FIGHTER_01","CREATURE_BS_FISTICUFFS_FIGHTER_02","CREATURE_BS_FISTICUFFS_FIGHTER_03","CREATURE_BS_FISTICUFFS_FIGHTER_04","CREATURE_BS_FISTICUFFS_FIGHTER_05"};
 switch(ParentClass.RelevantRegion){
 case REGION_KNOTHOLE:
  if(IsActiveThreadTerminating())return _Fail_Val.Return();
  FightList[0]="CREATURE_KHG_FISTICUFFS_FIGHTER_01";FightList[1]="CREATURE_KHG_FISTICUFFS_FIGHTER_02";FightList[2]="CREATURE_KHG_FISTICUFFS_FIGHTER_03";FightList[3]="CREATURE_KHG_FISTICUFFS_FIGHTER_04";FightList[4]="CREATURE_KHG_FISTICUFFS_FIGHTER_05";break;
 case REGION_BANDITCAMP:
  if(IsActiveThreadTerminating())return _Fail_Val.Return();
  FightList[0]="CREATURE_BC_FISTICUFFS_FIGHTER_01";FightList[1]="CREATURE_BC_FISTICUFFS_FIGHTER_02";FightList[2]="CREATURE_BC_FISTICUFFS_FIGHTER_03";FightList[3]="CREATURE_BC_FISTICUFFS_FIGHTER_04";FightList[4]="CREATURE_BC_FISTICUFFS_FIGHTER_05";break;
 case REGION_OAKVALE:
  if(IsActiveThreadTerminating())return _Fail_Val.Return();
  FightList[0]="CREATURE_OV_FISTICUFFS_FIGHTER_01";FightList[1]="CREATURE_OV_FISTICUFFS_FIGHTER_02";FightList[2]="CREATURE_OV_FISTICUFFS_FIGHTER_03";FightList[3]="CREATURE_OV_FISTICUFFS_FIGHTER_04";FightList[4]="CREATURE_OV_FISTICUFFS_FIGHTER_05";break;
 }
 CScriptGameResourceObjectScriptedThingBase seh_fighter;
 long FighterNumber=0;
 ParentClass.SetAllowedHits(0);
 ParentClass.AddQuestInfo();
 while(!ParentClass.TimeOut&&FighterNumber<ParentClass.FightCount){
  Game.NewScriptFrame();if(IsActiveThreadTerminating())return _Fail_Val.Return();
  while(!ParentClass.RingCleared){Game.NewScriptFrame();if(IsActiveThreadTerminating())return _Fail_Val.Return();}
  if(IsActiveThreadTerminating())return _Fail_Val.Return();
  ParentClass.RingCleared=false;
  ParentClass.RoundStarted=false;
  ParentClass.BooNumber=0.0f;
  ParentClass.CheerNumber=8.0f;
  CScriptThing Fighter;
  if(FighterNumber>4){
   if(IsActiveThreadTerminating())return _Fail_Val.Return();
   Fighter=Game.CreateCreature(FightList[4],Game.GetThingWithScriptName("NewFighterPos").GetPos(),"RingFighter",false);
  }else{
   if(IsActiveThreadTerminating())return _Fail_Val.Return();
   Fighter=Game.CreateCreature(FightList[FighterNumber],Game.GetThingWithScriptName("NewFighterPos").GetPos(),"RingFighter",false);
  }
  Game.EntitySetOpinionDeedTypeEnabled(Fighter,OPINION_DEED_TYPE_CRIME_ASSAULT,false);
  Game.StartScriptingEntity(Fighter,seh_fighter,SCRIPT_AI_PRIORITY_HIGHEST);
  {
   std::map<CCharString,CScriptGameResourceObjectScriptedThingBase> mymap_new;
   mymap_new["FIGHTER"]=seh_fighter;
   if(FighterNumber==0){
    if(IsActiveThreadTerminating())return _Fail_Val.Return();
    CScriptGameResourceObjectMovieBase speech_movie;
    Game.StartMovieSequence("",speech_movie);
    CWideScreenMagicPauseEntities PauseMyEntitiesPlease(Game);
    RunCutsceneMacro(ParentClass.GetCutsceneName("ROUNDNEW"),mymap_new,0,0,false,true);
   }else{
    if(IsActiveThreadTerminating())return _Fail_Val.Return();
    RunCutsceneMacro(ParentClass.GetCutsceneName("ROUNDNEW"),mymap_new,0,0,false,true);
   }
   while(seh_fighter.IsPerformingScriptTask()!=false){Game.NewScriptFrame();if(IsActiveThreadTerminating())return _Fail_Val.Return();}
   if(IsActiveThreadTerminating())return _Fail_Val.Return();
   ParentClass.RoundStarted=true;
   Game.Pause(0.6f);
   Game.PlayCriteriaSoundOnThing(Me,"BOXING_ROUND_BELL");
   Game.Pause(0.4f);
   seh_fighter.Release();
  }
  while(!ParentClass.TimeOut&&Game.GetHealth(Fighter)>pGlobalScriptDef->FC_MinimumFighterHealth&&Fighter.IsAlive()!=false){
   Game.NewScriptFrame();if(IsActiveThreadTerminating())return _Fail_Val.Return();
   if(Game.GetHero().MsgIsHitBy("RingFighter")){
    if(IsActiveThreadTerminating())return _Fail_Val.Return();
    if(!Game.IsPlayerCreatureBlocking()){
     if(IsActiveThreadTerminating())return _Fail_Val.Return();
     ++ParentClass.HitCount;
     Game.UpdateQuestInfoBar(ParentClass.HitID,(float)(ParentClass.AllowedHits-ParentClass.HitCount),-1.0f,-1.0f);
    }
   }
   if(ParentClass.HitCount>=ParentClass.AllowedHits){
    if(IsActiveThreadTerminating())return _Fail_Val.Return();
    ParentClass.TimeOut=true;
   }
  }
  if(IsActiveThreadTerminating())return _Fail_Val.Return();
  Game.EntitySetOpinionDeedTypeEnabled(Fighter,OPINION_DEED_TYPE_CRIME_ASSAULT,true);
  if(Fighter.IsAlive()==false){
   if(IsActiveThreadTerminating())return _Fail_Val.Return();
   ParentClass.RingCleared=true;
  }
  Money+=pGlobalScriptDef->FC_MoneyAddedPerFight;
  ++FighterNumber;
  if(ParentClass.HitCount==0&&!ParentClass.TimeOut){
   if(IsActiveThreadTerminating())return _Fail_Val.Return();
   Game.AddScreenTitleMessage(ParentClass.GetQuestText("PERFECT_ROUND"),2.0f,false);
   Money+=pGlobalScriptDef->FC_MoneyAddedPerFight;
  }
  ParentClass.HitCount=0;
  ParentClass.SetAllowedHits(FighterNumber);
  Game.UpdateQuestInfoBar(ParentClass.HitID,(float)(ParentClass.AllowedHits-ParentClass.HitCount),(float)ParentClass.AllowedHits,-1.0f);
 }
 if(IsActiveThreadTerminating())return _Fail_Val.Return();
 ParentClass.HitCount=0;
 Game.RemoveQuestInfoElement(ParentClass.HitID);
}
