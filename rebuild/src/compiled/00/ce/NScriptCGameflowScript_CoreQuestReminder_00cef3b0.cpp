// 00cef3b0 NScript::CGameflowScript::CoreQuestReminder
// Revisit: inline-depth -- byte-exact, but reached with #pragma inline_depth regions retail most likely never wrote; redo pragma-free once the retail mechanism is found (owner 2026-11-02; docs/pipeline/INLINE_DEPTH_REVISIT.md).
// Leaf: ?CoreQuestReminder@CGameflowScript@NScript@@QAEXXZ
// Flags:
#define FABLE_RETAIL_NSCRIPT
#define FABLE_RETAIL_DEF_CLASS_BASE
#include "engine/CLevelScriptBase.h"
#include "engine/CGameScriptInterfaceBase.h"
#include "engine/CTimer.h"
#include "engine/CDefPointer.h"
#include "engine/CParentDefClassBase.h"
class CCharStringData;
#define FABLE_RETAIL_CCHARSTRING
#include "fable_string.h"                              // CCharString (agentNF60: was a local stand-in)
// Layout: donor member0x1084 minus46 vector differences(4 each), and base difference4, equals native0xfc8.
extern CGameScriptInterfaceBase*pDSTGame;
#define FABLE_RETAIL_CDEFSTRINGTABLE
#define FABLE_RETAIL_DEF_CLASS_BASE
#include "engine/fable_script_def_global.h"          // pGlobalScriptDef 0x0143E90C (agentNF60: was a local extern over a local CScriptDef model)
inline CTimer::CTimer():TimerIndex(pDSTGame->RegisterTimer()){}
inline CTimer::~CTimer(){pDSTGame->DeregisterTimer(TimerIndex);}
inline CTimer&CTimer::operator=(long value){pDSTGame->SetTimer(TimerIndex,value);return *this;}
inline long CTimer::GetTime()const{return pDSTGame->GetTimer(TimerIndex);}
namespace NScript {
class CQ_SunnyvaleMasterData;
// Layout: shared level-script base0x48; original public CoreQuestWaiting at native0x48, donor0x5c.
class CGameflowScript:public CLevelScriptBase<CQ_SunnyvaleMasterData>{public:unsigned long CoreQuestWaiting;void CoreQuestReminder();};
void CGameflowScript::CoreQuestReminder(){
 while(!IsActiveThreadTerminating()){
  while(!CoreQuestWaiting){Game->NewScriptFrame();if(IsActiveThreadTerminating())return;}
  if(IsActiveThreadTerminating())return;
  {
   CTimer timer;timer=pGlobalScriptDef->CoreQuestReminderIntervalSeconds;
   while(timer.GetTime()){Game->NewScriptFrame();if(IsActiveThreadTerminating())return;}
   if(IsActiveThreadTerminating())return;
   if(CoreQuestWaiting&&!Game->IsHeroOnQuest()){
    if(IsActiveThreadTerminating())
#pragma inline_depth(0)
      return;
#pragma inline_depth(255)
    CCharString empty("");CCharString name("TEXT_QST_078_GM_MSG_NEW_QUEST_AT_GUILD");
    Game->HeroReceiveMessageFromGuildMaster(name,empty,true,false);
   }
  }
  Game->NewScriptFrame();
 }
}
}