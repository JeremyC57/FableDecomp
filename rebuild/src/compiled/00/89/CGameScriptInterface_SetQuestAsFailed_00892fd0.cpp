#include "engine/CGameScriptInterface.h"
// Name fix (agentNF7 2026-11-10): 0x013B89FC bound as the PDB's CTBaseSingletonNoDefaultAlloc<CQuestManager>::PClass through the shared engine/CTBaseSingletonNoDefaultAlloc.h (was a private stand-in global).

struct CQuestMgr { void doFailQuest(int a1,char a2,int a3,int a4); void notify(int a1); };
#include "engine/CTBaseSingletonNoDefaultAlloc.h"
class CQuestManager;
// 0x013B89FC = CTBaseSingletonNoDefaultAlloc<CQuestManager>::PClass (private static; reached through the public inline Get())
extern char g_someFlag;
struct CGameScriptInterface_Methods : CGameScriptInterface { void applyFloat(float v); };
void __fastcall CGameScriptInterface_SetQuestAsFailed(CGameScriptInterface* self,int,int a1,char a2,int a3,int a4)
{
 reinterpret_cast<CQuestMgr*>(&CTBaseSingletonNoDefaultAlloc<CQuestManager>::Get())->doFailQuest(a1,a2,a3,a4);
 if(!a2&&g_someFlag)reinterpret_cast<CQuestMgr*>(&CTBaseSingletonNoDefaultAlloc<CQuestManager>::Get())->notify(a1);
 ((CGameScriptInterface_Methods*)self)->applyFloat(1.0f);
}
