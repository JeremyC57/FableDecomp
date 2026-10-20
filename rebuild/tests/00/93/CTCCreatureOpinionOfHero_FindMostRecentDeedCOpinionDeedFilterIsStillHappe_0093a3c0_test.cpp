// Behaviour test for CTCCreatureOpinionOfHero::FindMostRecentDeed<IsStillHappening> (0x0093A3C0): pending deeds (of the type,
// still happening) are checked first when asked, then the known deeds of the type; the handle goes to outHandle when given.
#include <cstdio>
struct CTCBase;
struct CInterfaceMap {                                  // CVectorMap<ETCInterfaceType, CTCBase*>
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    const SEntry* LowerBound(const long* type) const { const SEntry* p = first; while (p != last && p->type < *type) ++p; return p; }
};
struct CThing {
    unsigned long m_presentBits[9];                     // +0x20 interface-present bitset
    CInterfaceMap m_interfaces;                         // +0x44
    unsigned long HasTC(long type) const { return m_presentBits[type >> 5] & (1UL << (type & 31)); }
    CTCBase* GetTC(long interfaceType) const
    {
        long type = interfaceType;
        const CInterfaceMap* map = &m_interfaces;
        const CInterfaceMap::SEntry* it = map->LowerBound(&type);
        if (it == map->last || interfaceType < it->type)
            it = map->last;
        return it->component;
    }
    template <class T> bool GetInterface(T** ppOut, long interfaceType) const
    {
        if (HasTC(interfaceType)) {
            *ppOut = (T*)GetTC(interfaceType);
            return true;
        }
        return false;
    }
};
struct CPlayer {
    CThing* Creature;
    CThing* GetControlledCreature() { return Creature; }
};
struct CPlayerManager {
    CPlayer* Player;
    CPlayer* GetMainPlayer() const { return Player; }
};
struct COpinionDeed {
    long          DeedType;                             // +0x00 EOpinionDeedType
    long          DeedHandle;                           // +0x04
    bool          Other;
    bool          IsStillHappening;                     // +0x21
};
struct COpinionDeedFilter_IsStillHappening {
    bool operator()(const COpinionDeed& deed) const { return deed.IsStillHappening; }
};
struct CTCHeroOpinionDeedLog {
    COpinionDeed* Deeds; int Count;
    const COpinionDeed* GetDeedFromHandle(long handle) const { for (int i = 0; i < Count; ++i) if (Deeds[i].DeedHandle == handle) return &Deeds[i]; return 0; }
};
struct SHandleNode {                                    // STLport list<long> node
    SHandleNode* next;
    SHandleNode* prev;
    long         value;                                 // +0x08
};
struct CHandleList {                                    // retail list: the sentinel node pointer
    SHandleNode* head;
};
struct CTCCreatureOpinionOfHero {
    virtual void v00() { }
    const CPlayerManager* PM;
    CHandleList PendingDeeds;                           // +0x54 (PDB +0x70)
    long        LastFrameAppliedDeedEffect[92];         // +0x58
    CHandleList KnownDeeds[92];                         // +0x1c8 (PDB +0x1ec)
    const CPlayerManager* GetPlayerManager() const { return PM; }
    template <class TFilter>
    const COpinionDeed* FindMostRecentDeed(long deedType, const TFilter& filter, long* outHandle, bool includePending) const;
};

template <class TFilter>
const COpinionDeed* CTCCreatureOpinionOfHero::FindMostRecentDeed(long deedType, const TFilter& filter, long* outHandle, bool includePending) const
{
    CThing* hero = GetPlayerManager()->GetMainPlayer()->GetControlledCreature();
    if (hero) {
        const CTCHeroOpinionDeedLog* log;
        hero->GetInterface(&log, 0x51);
        if (includePending) {
            SHandleNode* end = PendingDeeds.head;
            for (SHandleNode* node = end->next; node != end; node = node->next) {
                const COpinionDeed* deed = log->GetDeedFromHandle(node->value);
                if (deed && deed->DeedType == deedType && filter(*deed)) {
                    if (outHandle)
                        *outHandle = node->value;
                    return deed;
                }
            }
        }
        SHandleNode* end = KnownDeeds[deedType].head;
        for (SHandleNode* node = end->next; node != end; node = node->next) {
            const COpinionDeed* deed = log->GetDeedFromHandle(node->value);
            if (deed && filter(*deed)) {
                if (outHandle)
                    *outHandle = node->value;
                return deed;
            }
        }
    }
    return 0;
}

template const COpinionDeed* CTCCreatureOpinionOfHero::FindMostRecentDeed<COpinionDeedFilter_IsStillHappening>(
    long, const COpinionDeedFilter_IsStillHappening&, long*, bool) const;

static void Link(SHandleNode* head, SHandleNode* nodes, long* values, int n)
{
    SHandleNode* prev = head;
    for (int i = 0; i < n; ++i) { nodes[i].value = values[i]; prev->next = &nodes[i]; nodes[i].prev = prev; prev = &nodes[i]; }
    prev->next = head; head->prev = prev;
}
int main()
{
    int bad = 0;
    COpinionDeed deeds[4];
    deeds[0].DeedType = 3; deeds[0].DeedHandle = 10; deeds[0].IsStillHappening = true;
    deeds[1].DeedType = 5; deeds[1].DeedHandle = 11; deeds[1].IsStillHappening = true;
    deeds[2].DeedType = 5; deeds[2].DeedHandle = 12; deeds[2].IsStillHappening = false;
    deeds[3].DeedType = 5; deeds[3].DeedHandle = 13; deeds[3].IsStillHappening = true;
    CTCHeroOpinionDeedLog log; log.Deeds = deeds; log.Count = 4;
    CThing hero; for (int i = 0; i < 9; ++i) hero.m_presentBits[i] = 0;
    CInterfaceMap::SEntry e[1] = { { 0x51, (CTCBase*)&log } };
    hero.m_interfaces.first = e; hero.m_interfaces.last = e + 1; hero.m_presentBits[2] = 1UL << 17;
    CPlayer player; player.Creature = &hero;
    CPlayerManager pm; pm.Player = &player;
    static CTCCreatureOpinionOfHero o; o.PM = &pm;
    SHandleNode pendHead, pend[3]; long pv[3] = { 10, 12, 11 }; Link(&pendHead, pend, pv, 3); o.PendingDeeds.head = &pendHead;
    SHandleNode knownHeads[92], known[2]; for (int i = 0; i < 92; ++i) { knownHeads[i].next = knownHeads[i].prev = &knownHeads[i]; o.KnownDeeds[i].head = &knownHeads[i]; }
    long kv[2] = { 12, 13 }; Link(&knownHeads[5], known, kv, 2);
    COpinionDeedFilter_IsStillHappening f;
    long h = -1;
    if (o.FindMostRecentDeed(5, f, &h, true) != &deeds[1] || h != 11) bad |= 1;      // pending: 10 wrong type, 12 not happening, 11
    h = -1;
    if (o.FindMostRecentDeed(5, f, &h, false) != &deeds[3] || h != 13) bad |= 2;     // known only: 12 not happening, 13
    if (o.FindMostRecentDeed(5, f, 0, false) != &deeds[3]) bad |= 4;                 // no out pointer
    h = -1;
    if (o.FindMostRecentDeed(7, f, &h, true) != 0 || h != -1) bad |= 8;
    player.Creature = 0;
    if (o.FindMostRecentDeed(5, f, &h, true) != 0) bad |= 16;
    if (bad) { std::printf("0093a3c0_TEST FAIL\n"); return 1; }
    std::printf("0093a3c0_TEST PASS\n");
    return 0;
}