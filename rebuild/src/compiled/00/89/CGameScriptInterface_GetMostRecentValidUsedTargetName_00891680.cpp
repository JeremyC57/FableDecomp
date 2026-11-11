#include "engine/CGameScriptInterface.h"  // retyped onto the PDB layout; byte parity re-verified
// Name fix (agentNF7 2026-11-10): 0x013B89FC bound as the PDB's CTBaseSingletonNoDefaultAlloc<CQuestManager>::PClass through the shared engine/CTBaseSingletonNoDefaultAlloc.h (was a private stand-in global).
class CCharString { public:
    void* p;
    CCharString(const CCharString& o);
};

struct CGlobal { char pad[0xb0]; CCharString name; };
#include "engine/CTBaseSingletonNoDefaultAlloc.h"
class CQuestManager;
// 0x013B89FC = CTBaseSingletonNoDefaultAlloc<CQuestManager>::PClass (private static; reached through the public inline Get())

struct CGameScriptInterface_Methods : CGameScriptInterface {
    CCharString GetMostRecentValidUsedTargetName();
};

CCharString CGameScriptInterface_Methods::GetMostRecentValidUsedTargetName()
{
    return reinterpret_cast<CGlobal*>(&CTBaseSingletonNoDefaultAlloc<CQuestManager>::Get())->name;
}