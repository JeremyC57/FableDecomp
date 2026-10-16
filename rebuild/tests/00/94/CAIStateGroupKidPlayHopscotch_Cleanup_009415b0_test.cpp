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

struct CMessageEvent { long type; unsigned long long uid; bool removed; };
struct CMessageEventManager {
    CMessageEvent* list; int n;
    CMessageEvent* FindMostRecentMessage(long type, unsigned long long uid)
    { for (int i = n - 1; i >= 0; --i) if (!list[i].removed && list[i].type == type && list[i].uid == uid) return &list[i]; return 0; }
    void RemoveMessage(CMessageEvent* e) { e->removed = true; }
};
struct CTCUserActionPointBase { bool held; long released; bool IsActionPointFree() { return !held; } void ReleaseSlot(long s) { released = s; held = false; } };

struct CAIStateGroup_KidPlayHopscotch {
    CAIBrain* PBrain;
    CBaseIntelligentPointer m_pMarker;
    long m_slot;
    unsigned long long m_kidUID;
    CMessageEventManager* m_mgr;
    void Cleanup()
    {
        if (IS_THING_ALIVE(m_pMarker)) {
            CTCUserActionPointBase* point;
            m_pMarker.GetPItem()->GetInterface(&point, 0x2d);
            if (!point->IsActionPointFree())
                point->ReleaseSlot(m_slot);
            CMessageEvent* event = m_mgr->FindMostRecentMessage(0x18, m_kidUID);
            if (event)
                m_mgr->RemoveMessage(event);
        }
    }
};

int main()
{
    CThing kid, marker; kid.Init(); marker.Init();
    CTCUserActionPointBase point = { true, -1 };
    marker.Add(0x2d, &point);
    CMessageEvent msgs[3] = { { 0x18, 7, false }, { 0x18, 8, false }, { 0x19, 7, false } };
    CMessageEventManager mgr = { msgs, 3 };
    CAIBrain brain = { &kid };
    CAIStateGroup_KidPlayHopscotch g; g.PBrain = &brain; g.m_slot = 3; g.m_kidUID = 7; g.m_mgr = &mgr;
    g.m_pMarker.SetPItem(&marker); marker.m_flags91 = 1;
    g.Cleanup();
    bool ok = point.held && !msgs[0].removed;
    marker.m_flags91 = 0;
    g.Cleanup();
    ok = ok && !point.held && point.released == 3 && msgs[0].removed && !msgs[1].removed && !msgs[2].removed;
    if (!ok) { std::printf("009415b0_TEST FAIL\n"); return 1; }
    std::printf("009415b0_TEST PASS\n");
    return 0;
}