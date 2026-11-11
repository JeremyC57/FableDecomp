class CCharString;
// Name fix (agentNF7 2026-11-10): 0x013B89FC bound as the PDB's CTBaseSingletonNoDefaultAlloc<CQuestManager>::PClass through the shared engine/CTBaseSingletonNoDefaultAlloc.h (was a private stand-in global).
class C2DCoordI;

class CQuestManager
{
public:
    void SetQuestWorldMapOffset(
        const CCharString& questName,
        const C2DCoordI& offset);
};

class CGameScriptInterface
{
public:
    virtual void SetQuestWorldMapOffset(
        const CCharString& questName,
        const C2DCoordI& offset) const;
};

#include "engine/CTBaseSingletonNoDefaultAlloc.h"
// 0x013B89FC = CTBaseSingletonNoDefaultAlloc<CQuestManager>::PClass (private static; reached through the public inline Get())

void CGameScriptInterface::SetQuestWorldMapOffset(
    const CCharString& questName,
    const C2DCoordI& offset) const
{
    CTBaseSingletonNoDefaultAlloc<CQuestManager>::Get().SetQuestWorldMapOffset(
        questName,
        offset);
}
