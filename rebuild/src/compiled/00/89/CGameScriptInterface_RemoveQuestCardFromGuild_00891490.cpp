// CGameScriptInterface::RemoveQuestCardFromGuild @ 00891490
// Literal (NF415, 2026-11-25): was the bare integer 0x122d70e (the retail VA of the pooled ""); now the literal is spelled, cast to the
//   stand-in's existing int parameter so no callee or table line changes (the callee port is T3). Retail 0x0122D70E: 00 (retail's one pooled "", NF388 / NF413 b2 / NF414 b1).
// Name fix (agentNF7 2026-11-10): 0x013B89FC bound as the PDB's CTBaseSingletonNoDefaultAlloc<CQuestManager>::PClass through the shared engine/CTBaseSingletonNoDefaultAlloc.h (was a private stand-in global).
struct Str {
    void* p;
    void ctor(int v);
    char check(int c);
    void dtor();
};
struct Guild {
    void* vt;
    char pad[0x100];
    int  map(int id);
    void remove(Str* s);
};
#include "engine/CTBaseSingletonNoDefaultAlloc.h"
class CQuestManager;
// 0x013B89FC = CTBaseSingletonNoDefaultAlloc<CQuestManager>::PClass (private static; reached through the public inline Get())

void __stdcall CGameScriptInterface_RemoveQuestCardFromGuild(int questId)
{
    int v = reinterpret_cast<Guild*>(&CTBaseSingletonNoDefaultAlloc<CQuestManager>::Get())->map(questId);
    Str tmp;
    tmp.ctor(v);
    if (tmp.check((int)"")) {
        reinterpret_cast<Guild*>(&CTBaseSingletonNoDefaultAlloc<CQuestManager>::Get())->remove(&tmp);
    }
    tmp.dtor();
}