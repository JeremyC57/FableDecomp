// Naming wave 18: the global stand-in g_QuestManager (0x013B89FC) is the private static CCountedPointer<CQuestManager>
// Plan phase 5 (2026-11-05), CCountedPointer wave: the local CCountedPointer / count-block stand-ins replaced by the shared engine/CCountedPointer.h (Ego_r.pdb: CCountedPointer<T> Data +0x0 / Info +0x4; CCPPointerInfo RefCount +0x0 / DeleteFunc +0x4 / Data +0x8).
//   CTBaseSingletonNoDefaultAlloc<CQuestManager>::PClass, read through the inline static accessor Get() (name-only: the bytes are unchanged).
class CCharString;

#include "engine/CCountedPointer.h"
template <class T> class CTBaseSingletonNoDefaultAlloc {              // the CRTP singleton base of CQuestManager
    static CCountedPointer<T> PClass;                                // 0x013B89FC ?PClass@?$CTBaseSingletonNoDefaultAlloc@VCQuestManager@@@@0V?$CCountedPointer@VCQuestManager@@@@A (private static)
public:
    static T& __fastcall Get() { return *PClass; }                   // Ego_r / FableWin ?Get@?$CTBaseSingletonNoDefaultAlloc@VCQuestManager@@@@SIAAVCQuestManager@@XZ (inline)
};
class CQuestManager
{
public:
    void __fastcall SetCreatureGeneratorState(const CCharString& generatorName, bool enabled);
};


class CGameScriptInterface
{
public:
    void __fastcall SetCreatureGeneratorsEnabledDuringScript(const CCharString& generatorName, bool enabled) const;
};

void __fastcall CGameScriptInterface::SetCreatureGeneratorsEnabledDuringScript(const CCharString& generatorName, bool enabled) const
{
    CTBaseSingletonNoDefaultAlloc<CQuestManager>::Get().SetCreatureGeneratorState(generatorName, enabled);
}