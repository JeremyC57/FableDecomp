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

struct CEventBase { long m_eventType; CBaseIntelligentPointer m_child; virtual ~CEventBase() { } };
static int g_events = 0; static CThing* g_lastChild = 0;
struct CTCEntityEvents { void AddEvent(CEventBase* e) { ++g_events; g_lastChild = e ? e->m_child.GetPItem() : 0; delete e; } };

struct CAIStateGroup_KidHurtSelfRunToMother {
    CAIBrain* PBrain;
    CBaseIntelligentPointer m_mother;
    void SendMessageToMother()
    {
        CThing* mother = m_mother.GetPItem();
        if (mother && !(mother->m_flags91 & 1)) {
            CTCEntityEvents* events;
            mother->GetInterface(&events, 0x42);
            CEventBase* e = new CEventBase; e->m_eventType = 0; e->m_child.SetPItem(PBrain->PCreature);
            events->AddEvent(e);
        }
    }
};

int main()
{
    CThing kid, mother; kid.Init(); mother.Init();
    CTCEntityEvents events;
    mother.Add(0x42, &events);
    CAIBrain brain = { &kid };
    CAIStateGroup_KidHurtSelfRunToMother g; g.PBrain = &brain; g.m_mother.SetPItem(0);
    g.SendMessageToMother();
    bool ok = g_events == 0;
    g.m_mother.SetPItem(&mother); mother.m_flags91 = 1;
    g.SendMessageToMother(); ok = ok && g_events == 0;
    mother.m_flags91 = 0;
    g.SendMessageToMother(); ok = ok && g_events == 1 && g_lastChild == &kid;
    if (!ok) { std::printf("00915290_TEST FAIL\n"); return 1; }
    std::printf("00915290_TEST PASS\n");
    return 0;
}
