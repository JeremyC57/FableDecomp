// Behaviour test for CTCDivineWrath::ResetCreatureHitState (0x0078E900): a live creature loses mode 0x5f (when it has a mode
// manager) and its 0x8b component is armed with CurrentValue = ResetValue; dead or null creatures are untouched; the
// by-value pointer is destroyed.
#include <cstdio>
static int g_dtors = 0;
struct CTCBase;
struct CInterfaceMap {
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type) { SEntry* p = first; while (p != last && p->type < *type) ++p; return p; }
};
struct CThing {
    unsigned long m_presentBits[9];
    CInterfaceMap m_interfaces;
    unsigned char Flags91;
    bool IsDead() const { return (Flags91 & 1) != 0; }
    template <class T> bool GetInterface(T** ppOut, long interfaceType)
    {
        if (m_presentBits[interfaceType >> 5] & (1UL << (interfaceType & 31))) {
            long type = interfaceType;
            CInterfaceMap* map = &m_interfaces;
            CInterfaceMap::SEntry* it = map->LowerBound(&type);
            if (it == map->last || interfaceType < it->type)
                it = map->last;
            *ppOut = (T*)it->component;
            return true;
        }
        return false;
    }
};
struct CBaseIntelligentPointer {
    virtual void v00() { }
    CThing* m_pItem;
    CBaseIntelligentPointer() : m_pItem(0) { }
    CBaseIntelligentPointer(const CBaseIntelligentPointer& o) : m_pItem(o.m_pItem) { }
    ~CBaseIntelligentPointer() { ++g_dtors; }
    CThing* GetPItem() const { return m_pItem; }
};
struct CIntelligentPointerCreature : CBaseIntelligentPointer { };
inline bool IS_THING_ALIVE(const CBaseIntelligentPointer& p)
{
    if (p.GetPItem() != 0) {
        if (!p.GetPItem()->IsDead())
            return true;
    }
    return false;
}
struct CTCCreatureModeManager { unsigned long Removed; int Calls; void RemoveMode(unsigned long m) { Removed = m; ++Calls; } };
struct CTCHitReaction { long ResetValue; bool Armed; long CurrentValue; };
struct CTCDivineWrath {
    virtual void v00() { }
    void ResetCreatureHitState(CIntelligentPointerCreature creature);
};

void CTCDivineWrath::ResetCreatureHitState(CIntelligentPointerCreature creature)
{
    if (IS_THING_ALIVE(creature)) {
        CTCCreatureModeManager* modes;
        if (creature.GetPItem()->GetInterface(&modes, 0x31))
            modes->RemoveMode(0x5f);
        CTCHitReaction* hit;
        if (creature.GetPItem()->GetInterface(&hit, 0x8b)) {
            hit->Armed = true;
            hit->CurrentValue = hit->ResetValue;
        }
    }
}

int main()
{
    int bad = 0;
    CTCCreatureModeManager modes; modes.Removed = 0; modes.Calls = 0;
    CTCHitReaction hit; hit.ResetValue = 40; hit.Armed = false; hit.CurrentValue = 3;
    CThing c; for (int i = 0; i < 9; ++i) c.m_presentBits[i] = 0; c.Flags91 = 0;
    CInterfaceMap::SEntry e[2] = { { 0x31, (CTCBase*)&modes }, { 0x8b, (CTCBase*)&hit } };
    c.m_interfaces.first = e; c.m_interfaces.last = e + 2; c.m_presentBits[1] = 1UL << 17; c.m_presentBits[4] = 1UL << 11;
    CTCDivineWrath w;
    CIntelligentPointerCreature p; p.m_pItem = &c;
    int d0 = g_dtors;
    w.ResetCreatureHitState(p);
    if (modes.Calls != 1 || modes.Removed != 0x5f || !hit.Armed || hit.CurrentValue != 40 || g_dtors - d0 != 1) bad |= 1;
    hit.Armed = false; hit.CurrentValue = 1; c.m_presentBits[1] = 0;
    w.ResetCreatureHitState(p);                      // no mode manager
    if (modes.Calls != 1 || !hit.Armed || hit.CurrentValue != 40) bad |= 2;
    hit.Armed = false; c.Flags91 = 1;
    w.ResetCreatureHitState(p);                      // dead
    if (hit.Armed) bad |= 4;
    p.m_pItem = 0;
    w.ResetCreatureHitState(p);                      // null
    if (hit.Armed) bad |= 8;
    if (bad) { std::printf("0078e900_TEST FAIL\n"); return 1; }
    std::printf("0078e900_TEST PASS\n");
    return 0;
}