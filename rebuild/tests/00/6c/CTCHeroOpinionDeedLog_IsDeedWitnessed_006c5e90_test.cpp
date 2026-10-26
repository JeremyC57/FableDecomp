// Behaviour test for CTCHeroOpinionDeedLog::IsDeedWitnessed (0x006C5E90): only live things carrying the opinion
// component (0x4F) are asked; any one knowing the deed answers true; the found list is freed on both paths.
#include <cstdio>
#include <cstdlib>

static int g_allocs = 0;
static void* Alloc(unsigned n) { ++g_allocs; return std::malloc(n); }
static void Free(void* p) { --g_allocs; std::free(p); }
class CTCBase;
struct CInterfaceMap {
    struct SEntry { long type; CTCBase* component; };
    SEntry* first; SEntry* last;
    SEntry* LowerBound(const long* type) { SEntry* it = first; while (it != last && it->type < *type) ++it; return it; }
};
class CThing { public:
    unsigned long m_presentBits[9]; CInterfaceMap m_interfaces; unsigned char m_flags91; CInterfaceMap::SEntry e[2];
    void Init() { for (int i = 0; i < 9; ++i) m_presentBits[i] = 0; m_interfaces.first = m_interfaces.last = e; m_flags91 = 0; }
    void Add(long type, void* c) { m_presentBits[type >> 5] |= 1UL << (type & 31); m_interfaces.last->type = type; m_interfaces.last->component = (CTCBase*)c; ++m_interfaces.last; }
    unsigned long HasTC(long t) const { return m_presentBits[t >> 5] & (1UL << (t & 31)); }
    template<class T> T* GetTC(long t)
    {
        long type = t; CInterfaceMap* map = &m_interfaces; CInterfaceMap::SEntry* it = map->LowerBound(&type);
        return (T*)((it == map->last || t < it->type) ? map->last : it)->component;
    }
};
inline bool IS_THING_ALIVE(const CThing* thing) { if (thing != 0) { if (!(thing->m_flags91 & 1)) return true; } return false; }
struct SThingListNode { SThingListNode* _M_next; SThingListNode* _M_prev; CThing* _M_data; };
namespace N_004ab4f0 { struct CThingList {
    SThingListNode* _M_node;
    CThingList() { SThingListNode* node = (SThingListNode*)Alloc(sizeof(SThingListNode)); node->_M_next = node; node->_M_prev = node; _M_node = node; }
    ~CThingList()
    {
        SThingListNode* cur = _M_node->_M_next;
        while (cur != _M_node) { SThingListNode* victim = cur; cur = cur->_M_next; Free(victim); }
        _M_node->_M_next = _M_node; _M_node->_M_prev = _M_node; Free(_M_node);
    }
}; } using N_004ab4f0::CThingList;
class CThingFilter_None { public:};
class CThingFilter_HasTC { public: long Type; };
template <class A, class B> class CFilter_Bind2 : public A { public: B Second; };
class CThingSearchTools { public:
    CThing** m_pFirst; CThing** m_pLast;
    template <class T, class C, class F> void GetAllThings(const F& filter, C& found)
    {
        for (CThing** it = m_pFirst; it != m_pLast; ++it) {
            if ((*it)->HasTC(filter.Second.Type)) {
                SThingListNode* node = (SThingListNode*)Alloc(sizeof(SThingListNode));
                SThingListNode* head = found._M_node;
                node->_M_data = *it; node->_M_next = head; node->_M_prev = head->_M_prev;
                head->_M_prev->_M_next = node; head->_M_prev = node;
            }
        }
    }
};
static long g_asked = 0;
class CTCCreatureOpinionOfHero { public: long Knows; bool KnowsOfDeed(long deed, bool) { ++g_asked; return deed == Knows; } };
static CThingSearchTools g_tools;
class CTCHeroOpinionDeedLog { public:
    CThingSearchTools* GetThingSearchTools() const { return &g_tools; }
    bool IsDeedWitnessed(long deed, bool flag);
};
bool CTCHeroOpinionDeedLog::IsDeedWitnessed(long deed, bool flag)
{
    CThingList creatures;
    CFilter_Bind2<CThingFilter_None, CThingFilter_HasTC> filter;
    filter.Second.Type = 0x4F;
    GetThingSearchTools()->GetAllThings<CThing>(filter, creatures);
    for (SThingListNode* node = creatures._M_node->_M_next; node != creatures._M_node; node = node->_M_next) {
        CThing* creature = node->_M_data;
        if (IS_THING_ALIVE(creature) && creature->HasTC(0x4F)) {
            if (creature->GetTC<CTCCreatureOpinionOfHero>(0x4F)->KnowsOfDeed(deed, flag))
                return true;
        }
    }
    return false;
}
int main()
{
    CThing a, b, c; a.Init(); b.Init(); c.Init();
    CTCCreatureOpinionOfHero oa, ob; oa.Knows = 7; ob.Knows = 9;
    a.Add(0x4F, &oa); b.Add(0x4F, &ob); b.m_flags91 = 1;
    CThing* things[3] = { &a, &b, &c };
    g_tools.m_pFirst = things; g_tools.m_pLast = things + 3;
    CTCHeroOpinionDeedLog log;
    if (!log.IsDeedWitnessed(7, true) || g_allocs != 0) { std::printf("006c5e90_TEST FAIL\n"); return 1; }
    g_asked = 0;
    if (log.IsDeedWitnessed(9, false) || g_asked != 1 || g_allocs != 0) { std::printf("006c5e90_TEST FAIL\n"); return 1; }
    std::printf("006c5e90_TEST PASS\n");
    return 0;
}