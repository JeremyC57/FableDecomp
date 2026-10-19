// Behaviour test for 0x00845850 CCreatureAction_ShowTrophy::FinishAction: runs the reconstructed body over stubbed
// callees; checks the emitter kill guard, the trophy component call with the owner, and the base FinishAction.
#include <cstdio>
struct CTCBase;
struct CInterfaceMap {
    struct SEntry { long type; CTCBase* component; };
    SEntry* first; SEntry* last;
    SEntry* LowerBound(const long* type) { SEntry* e = first; while (e != last && e->type < *type) ++e; return e; }
};
static int g_kills = 0; static bool g_killArg = false;
struct CThing {
    unsigned long m_presentBits[9];
    CInterfaceMap m_interfaces;
    unsigned char m_flags91;
    void Kill(bool silent) { ++g_kills; g_killArg = silent; }
    template<class T> bool GetInterface(T** ppOut, long interfaceType)
    {
        if (m_presentBits[interfaceType >> 5] & (1UL << (interfaceType & 31))) {
            long type = interfaceType; CInterfaceMap* map = &m_interfaces;
            CInterfaceMap::SEntry* it = map->LowerBound(&type);
            if (it == map->last || interfaceType < it->type) it = map->last;
            *ppOut = (T*)it->component; return true;
        }
        return false;
    }
};
static CThing* g_shownBy = 0; static int g_shown = 0;
struct CTCTrophy { void OnShownByCreature(CThing* c) { g_shownBy = c; ++g_shown; } };
struct CBaseIntelligentPointer { CThing* m_pItem; CThing* GetPItem() const { return m_pItem; } };
static int g_base = 0;
class CCreatureActionBase {
public:
    virtual void FinishAction() { ++g_base; }
    CBaseIntelligentPointer PCreature;
};
class CCreatureAction_ShowTrophy : public CCreatureActionBase {
public:
    CBaseIntelligentPointer PTrophyBeingShown; unsigned long ShowingTrophyDeedHandle; bool PostedDeed; CBaseIntelligentPointer PEmitter;
    virtual void FinishAction();
    CTCTrophy* GetTrophy() const { CTCTrophy* trophy = 0; PTrophyBeingShown.GetPItem()->GetInterface(&trophy, 0x39); return trophy; }
};
void CCreatureAction_ShowTrophy::FinishAction()
{
    if (PEmitter.GetPItem() && !(PEmitter.GetPItem()->m_flags91 & 1))
        PEmitter.GetPItem()->Kill(true);
    if (PTrophyBeingShown.GetPItem() && !(PTrophyBeingShown.GetPItem()->m_flags91 & 1))
        GetTrophy()->OnShownByCreature(PCreature.GetPItem());
    CCreatureActionBase::FinishAction();
}
int main()
{
    CTCTrophy tc; CInterfaceMap::SEntry e[1] = { { 0x39, (CTCBase*)&tc } };
    CThing owner = CThing(), trophy = CThing(), emitter = CThing();
    trophy.m_presentBits[1] = 1UL << (0x39 & 31); trophy.m_interfaces.first = e; trophy.m_interfaces.last = e + 1;
    CCreatureAction_ShowTrophy a; a.PCreature.m_pItem = &owner; a.PTrophyBeingShown.m_pItem = &trophy; a.PEmitter.m_pItem = &emitter;
    a.FinishAction();
    bool ok = g_kills == 1 && g_killArg && g_shown == 1 && g_shownBy == &owner && g_base == 1;
    emitter.m_flags91 = 1; trophy.m_flags91 = 1;               // both dead: only the base runs
    a.FinishAction();
    ok = ok && g_kills == 1 && g_shown == 1 && g_base == 2;
    a.PEmitter.m_pItem = 0; a.PTrophyBeingShown.m_pItem = 0;  // both absent
    a.FinishAction();
    ok = ok && g_kills == 1 && g_shown == 1 && g_base == 3;
    if (!ok) { std::printf("00845850_TEST FAIL\n"); return 1; }
    std::printf("00845850_TEST PASS\n");
    return 0;
}
