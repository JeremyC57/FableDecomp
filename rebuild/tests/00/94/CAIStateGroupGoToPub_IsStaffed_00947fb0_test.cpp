#include <cstdio>
// ---- test model: a thing's interface bitset + sorted {type, component} table, intelligent pointers ----
struct CThing;
struct CInterfaceMap {
    struct SEntry { long type; void* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type)
    {
        SEntry* it = first;
        while (it != last && it->type < *type) ++it;
        return it;
    }
};
struct CThing {
    unsigned long m_presentBits[9];
    CInterfaceMap m_interfaces;
    unsigned char m_flags91;
    CInterfaceMap::SEntry m_table[8];
    int m_count;
    void Init() { for (int i = 0; i < 9; ++i) m_presentBits[i] = 0; m_flags91 = 0; m_count = 0; Sync(); }
    void Sync() { m_interfaces.first = m_table; m_interfaces.last = m_table + m_count; }
    void Add(long type, void* comp)       // keep the table sorted
    {
        int i = m_count;
        while (i > 0 && m_table[i - 1].type > type) { m_table[i] = m_table[i - 1]; --i; }
        m_table[i].type = type; m_table[i].component = comp; ++m_count;
        m_presentBits[type >> 5] |= 1UL << (type & 31);
        Sync();
    }
    template<class T> bool GetInterface(T** ppOut, long interfaceType)
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
    CThing* m_pItem;
    CThing* GetPItem() const { return m_pItem; }
    void SetPItem(CThing* p) { m_pItem = p; }
};
inline bool IS_THING_ALIVE(const CBaseIntelligentPointer& p)
{
    if (p.GetPItem() != 0) {
        if (!(p.GetPItem()->m_flags91 & 1))
            return true;
    }
    return false;
}
struct CAIBrain { CThing* PCreature; };

struct CListNode { CListNode* next; CListNode* prev; CBaseIntelligentPointer value; };
struct CThingLinked { CBaseIntelligentPointer m_linkedThing; };
struct CTCVillage { CListNode* m_head; };
struct CTCVillager { CTCVillage* village; };
CTCVillage* GFGetPVillageInterface(CTCVillager* v) { return v->village; }

static CThingLinked g_linked[3];
static CThing g_things[3];
CThingLinked* LinkOf(CThing* t) { return &g_linked[t - g_things]; }

struct CAIStateGroup_GoToPub {
    CAIBrain* PBrain;
    CBaseIntelligentPointer PPub;
    bool IsStaffed()
    {
        CTCVillager* villager;
        if (PBrain->PCreature->GetInterface(&villager, 0x23)) {
            CTCVillage* village = GFGetPVillageInterface(villager);
            if (village) {
                for (CListNode* node = village->m_head->next; node != village->m_head; node = node->next) {
                    if (IS_THING_ALIVE(node->value)
                        && LinkOf(node->value.GetPItem())->m_linkedThing.GetPItem() == PPub.GetPItem())
                        return true;
                }
            }
        }
        return false;
    }
};

int main()
{
    CThing creature, pub, other;
    creature.Init(); pub.Init(); other.Init();
    CListNode head, n0, n1;
    head.next = &n0; n0.next = &n1; n1.next = &head;
    for (int i = 0; i < 3; ++i) { g_things[i].Init(); g_linked[i].m_linkedThing.SetPItem(&other); }
    n0.value.SetPItem(&g_things[0]); n1.value.SetPItem(&g_things[1]);
    CTCVillage village = { &head };
    CTCVillager villager = { &village };
    CAIBrain brain = { &creature };
    CAIStateGroup_GoToPub g; g.PBrain = &brain; g.PPub.SetPItem(&pub);
    bool ok = !g.IsStaffed();                   // no villager interface
    creature.Add(0x23, &villager);
    ok = ok && !g.IsStaffed();                  // nobody linked to the pub
    g_linked[1].m_linkedThing.SetPItem(&pub);
    ok = ok && g.IsStaffed();
    g_things[1].m_flags91 = 1;                  // dead worker
    ok = ok && !g.IsStaffed();
    if (!ok) { std::printf("00947fb0_TEST FAIL\n"); return 1; }
    std::printf("00947fb0_TEST PASS\n");
    return 0;
}