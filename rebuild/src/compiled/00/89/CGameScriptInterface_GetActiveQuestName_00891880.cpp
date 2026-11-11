#include "engine/CGameScriptInterface.h"  // retyped onto the PDB layout; byte parity re-verified
// Name fix (agentNF7 2026-11-10): 0x013B89FC bound as the PDB's CTBaseSingletonNoDefaultAlloc<CQuestManager>::PClass through the shared engine/CTBaseSingletonNoDefaultAlloc.h (was a private stand-in global).
class CCharString { public:
    char* p;
    CCharString(const char* s, long len);
    CCharString(const CCharString& o);
};

struct QuestObj {
    char _pad[0x30];
    CCharString name; // +0x30
};

struct GameGlobal {
    char _pad[0x88];
    QuestObj* active; // +0x88
};

#include "engine/CTBaseSingletonNoDefaultAlloc.h"
class CQuestManager;
// 0x013B89FC = CTBaseSingletonNoDefaultAlloc<CQuestManager>::PClass (private static; reached through the public inline Get())

struct CGameScriptInterface_Methods : CGameScriptInterface {
    CCharString GetActiveQuestName();
};

CCharString CGameScriptInterface_Methods::GetActiveQuestName()
{
    QuestObj* q = reinterpret_cast<GameGlobal*>(&CTBaseSingletonNoDefaultAlloc<CQuestManager>::Get())->active;
    if (q)
        return q->name;
    return CCharString("", -1);
}