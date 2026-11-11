class CCharString;
// Name fix (agentNF7 2026-11-10): 0x013B89FC bound as the PDB's CTBaseSingletonNoDefaultAlloc<CQuestManager>::PClass through the shared engine/CTBaseSingletonNoDefaultAlloc.h (was a private stand-in global).

class CQuestManager
{
public:
    void AddFeatCard(
        long featCardId,
        const CCharString& title,
        const CCharString& description);
};

class CGameScriptInterface
{
public:
    virtual void AddFeatCard(
        long featCardId,
        const CCharString& title,
        const CCharString& description) const;
};

#include "engine/CTBaseSingletonNoDefaultAlloc.h"
// 0x013B89FC = CTBaseSingletonNoDefaultAlloc<CQuestManager>::PClass (private static; reached through the public inline Get())

void CGameScriptInterface::AddFeatCard(
    long featCardId,
    const CCharString& title,
    const CCharString& description) const
{
    CTBaseSingletonNoDefaultAlloc<CQuestManager>::Get().AddFeatCard(
        featCardId,
        title,
        description);
}
