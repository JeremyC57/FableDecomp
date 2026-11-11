struct CQuestMgr {
// Name fix (agentNF7 2026-11-10): 0x013B89FC bound as the PDB's CTBaseSingletonNoDefaultAlloc<CQuestManager>::PClass through the shared engine/CTBaseSingletonNoDefaultAlloc.h (was a private stand-in global).
    void Activate(int quest, int a, int b);
};
#include "engine/CTBaseSingletonNoDefaultAlloc.h"
class CQuestManager;
// 0x013B89FC = CTBaseSingletonNoDefaultAlloc<CQuestManager>::PClass (private static; reached through the public inline Get())

void __stdcall CGameScriptInterface_ActivateQuest(int quest)
{
    reinterpret_cast<CQuestMgr*>(&CTBaseSingletonNoDefaultAlloc<CQuestManager>::Get())->Activate(quest, 1, 1);
}