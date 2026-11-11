class CCharString;
// Name fix (agentNF7 2026-11-10): 0x013B89FC bound as the PDB's CTBaseSingletonNoDefaultAlloc<CQuestManager>::PClass through the shared engine/CTBaseSingletonNoDefaultAlloc.h (was a private stand-in global).

class CQuestManager
{
public:
    bool IsQuestActive(const CCharString& questName);
};

class CGameScriptInterface
{
public:
    virtual bool IsQuestActive(
        const CCharString& questName) const;
};

#include "engine/CTBaseSingletonNoDefaultAlloc.h"
// 0x013B89FC = CTBaseSingletonNoDefaultAlloc<CQuestManager>::PClass (private static; reached through the public inline Get())

bool CGameScriptInterface::IsQuestActive(
    const CCharString& questName) const
{
    return CTBaseSingletonNoDefaultAlloc<CQuestManager>::Get().IsQuestActive(questName);
}
