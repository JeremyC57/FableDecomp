struct CCharStringArray;
// Name fix (agentNF7 2026-11-10): 0x013B89FC bound as the PDB's CTBaseSingletonNoDefaultAlloc<CQuestManager>::PClass through the shared engine/CTBaseSingletonNoDefaultAlloc.h (was a private stand-in global).
struct CQuestMgr {
    void Activate(CCharStringArray const& quests, int a, int b);
};
#include "engine/CTBaseSingletonNoDefaultAlloc.h"
class CQuestManager;
// 0x013B89FC = CTBaseSingletonNoDefaultAlloc<CQuestManager>::PClass (private static; reached through the public inline Get())

void __stdcall CGameScriptInterface_ActivateMultipleQuestsWithoutLoadingResources(CCharStringArray const& quests)
{
    reinterpret_cast<CQuestMgr*>(&CTBaseSingletonNoDefaultAlloc<CQuestManager>::Get())->Activate(quests, 1, 0);
}