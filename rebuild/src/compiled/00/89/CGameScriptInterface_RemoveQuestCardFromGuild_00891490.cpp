// CGameScriptInterface::RemoveQuestCardFromGuild @ 00891490
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
    if (tmp.check(0x122d70e)) {
        reinterpret_cast<Guild*>(&CTBaseSingletonNoDefaultAlloc<CQuestManager>::Get())->remove(&tmp);
    }
    tmp.dtor();
}