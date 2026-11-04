// NScript::CQ_WaspBossScript::Outro() -- Wasp Menace outro: flag PanickedVillagersScene, take the hero under scripted control inside a paused movie sequence, wait 3 s, fade out and in, remove the dead Queen Hornet at QueenDepositPos1, play CS_WASPBOSS_OUTRO, then put VILL1..VILL3 in FACTION_PICNIC_AREA
// Leaf: ?Outro@CQ_WaspBossScript@NScript@@QAEXXZ
// Flags: none
// Retail: Fable.exe (TLC, VC7.1) @ 0x00E137B0, 1080 bytes, thiscall, plain `ret`, no EH frame. Literals "" 0x0122D70E,
//   "CREATURE_HORNET_QUEEN_01" 0x012E074C, "QueenDepositPos1" 0x012E0738, "HERO" 0x01255174, "CS_WASPBOSS_OUTRO" 0x012E0724,
//   "FACTION_PICNIC_AREA" 0x012E05E4, "VILL1" 0x012E071C, "VILL2" 0x012E0714, "VILL3" 0x012E070C.
// Identity (two sources): Ego_r.pdb / FableWin.pdb ?Outro@CQ_WaspBossScript@NScript@@QAEXXZ; its only caller is the landed
//   CQ_WaspBossScript::EndMission 0x00E13310 (call at 0x00E135B4, already bound to this symbol); the WaspBoss seed-corpus Lua port
//   Outro() has the same statement list (3 s pause, 0.5/0.5 fade, QueenDepositPos1 / CREATURE_HORNET_QUEEN_01 removal,
//   CS_WASPBOSS_OUTRO, PanickedVillagersScene, VILL1..3 in FACTION_PICNIC_AREA). Catalog name DoMission_WaspMenaceOutro is wrong.
// Layout: engine/NScript_CQ_WaspBossScript.h (Game +0x40, PanickedVillagersScene +0x4e).
// Callees: CScriptBase::IsActiveThreadTerminating 0x00CB7940; CBaseObject() 0x0099A380 / operator= 0x0099A3B0 / ~ 0x0099A430;
//   ~CScriptGameResourceObjectScriptedThingBase 0x007E74D0; movie PImp release 0x006E7AB0; CCharString 0x0099EBF0 / ~ 0x0099EAE0;
//   ~CScriptThing 0x004AA840 (named local) and ~CBaseClass 0x0099A2E0 (the expanded temporary dtor); operator delete 0x00BFE9BC,
//   malloc 0x00BFEA0E, map operator[] 0x00CD3D2E / ~map 0x00CDBFB0, RunCutsceneMacro 0x00CBFB7D; CGameScriptInterfaceBase slots
//   7, 8, 70, 72, 81, 108, 370, 373, 376, 379 (CWideScreenMagicPauseEntities), 601.
// Shape: the landed same-TU EndMission 0x00E13310 prelude verbatim (auto_inline(off) movie CCountedPointer dtor, inline
//   CScriptThing members) + FABLE_RETAIL_STLP_MAP as the landed DuelLost cutscene row. Retail expands the cast assignment
//   (CBaseObject::operator= call + inline PImp copy): the operator is declared by the header's existing OUTOFLINE_ASSIGN switch
//   and defined `__forceinline` here, as the landed same-TU WaspIntro 0x00E12F20 does (plain inline stays a call, 1004 bytes).
//   IsAlive in the `&&` form (the if-form compares the result with the cached zero, `cmp al,bl`).
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_NSCRIPT
#define FABLE_RETAIL_BASE_OBJECT
#define FABLE_RETAIL_STLP_MAP
#define FABLE_RETAIL_CSCRIPTGAMERESOURCEOBJECTSCRIPTEDTHINGBASE_OUTOFLINE_ASSIGN
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
#include "engine/fable_script_cutscene.h"
__forceinline CScriptGameResourceObjectScriptedThingBase& CScriptGameResourceObjectScriptedThingBase::operator=(const CScriptGameResourceObjectScriptedThingBase& other) { CScriptGameResourceObjectBase::operator=(other); PImp=other.PImp; return *this; }
inline CScriptThing::CScriptThing() { }
inline CScriptThing::~CScriptThing() { }
inline CScriptThing& CScriptThing::operator=(const CScriptThing& other) { PImp=other.PImp; return *this; }
inline bool CScriptThing::IsAlive()const{return !PImp.IsNull() && PImp->IsAlive();}
void NScript::CQ_WaspBossScript::Outro()
{
 Game->NewScriptFrame();
 if(IsActiveThreadTerminating())return;
 PanickedVillagersScene=true;
 {
  CScriptGameResourceObjectScriptedThingBase seh_hero;
  Game->StartScriptingEntity(Game->GetHero(),seh_hero,SCRIPT_AI_PRIORITY_HIGHEST);
  CScriptGameResourceObjectMovieBase movie;
  Game->StartMovieSequence("",movie);
  CWideScreenMagicPauseEntities pause(*Game);
  Game->Pause(3.0f);
  Game->FadeScreenOut(0.5f,0.5f,CRGBColour(0,0,0,255));
  Game->Pause(0.5f);
  CScriptThing queen=Game->GetNearestWithDefName(Game->GetThingWithScriptName("QueenDepositPos1"),"CREATURE_HORNET_QUEEN_01");
  if(queen.IsAlive()) {
   if(IsActiveThreadTerminating())return;
   Game->RemoveThing(queen,false,true);
  }
  std::map<CCharString,CScriptGameResourceObjectScriptedThingBase> cast;
  cast["HERO"]=seh_hero;
  RunCutsceneMacro("CS_WASPBOSS_OUTRO",cast,0,0,false,true);
 }
 Game->EntitySetInFaction(Game->GetThingWithScriptName("VILL1"),"FACTION_PICNIC_AREA");
 Game->EntitySetInFaction(Game->GetThingWithScriptName("VILL2"),"FACTION_PICNIC_AREA");
 Game->EntitySetInFaction(Game->GetThingWithScriptName("VILL3"),"FACTION_PICNIC_AREA");
}