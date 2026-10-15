#include <cstdio>
struct CThing {
    bool m_scan; int m_def; int m_script; int m_tc;
    bool IsIncludedInThingScans() { return m_scan; }
};
struct CThingList {
    struct SNode { SNode* m_pNext; SNode* m_pPrev; CThing* m_value; };
    SNode* m_pHead;
    struct iterator {
        SNode* m_pNode;
        iterator(SNode* node) : m_pNode(node) {}
        CThing* operator*() const { return m_pNode->m_value; }
        iterator& operator++() { m_pNode = m_pNode->m_pNext; return *this; }
        bool operator!=(const iterator& o) const { return m_pNode != o.m_pNode; }
    };
    iterator begin() { return iterator(m_pHead->m_pNext); }
    iterator end() { return iterator(m_pHead); }
};
static CThingList::SNode g_head, g_nodes[3];
static CThing g_things[3];
static CThingList g_list;
static long g_peeked = -1;
static void BuildList()
{
    int i;
    g_head.m_pNext = &g_nodes[0]; g_head.m_pPrev = &g_nodes[2]; g_head.m_value = 0;
    for (i = 0; i < 3; ++i) {
        g_nodes[i].m_value = &g_things[i];
        g_nodes[i].m_pNext = i < 2 ? &g_nodes[i + 1] : &g_head;
        g_nodes[i].m_pPrev = i > 0 ? &g_nodes[i - 1] : &g_head;
        g_things[i].m_scan = true; g_things[i].m_def = 0; g_things[i].m_script = 0; g_things[i].m_tc = 0;
    }
    g_list.m_pHead = &g_head;
}
struct CThingSearchTools { void* WorldMap; };
struct CThingMarker { enum { ThingListType = 0x0b }; };
struct CThingFilter_HasDefName { bool operator()(CThing* thing) const { return thing->m_def != 0; } };
struct CThingFilter_HasScriptData { bool operator()(CThing* thing) const { return thing->m_script != 0; } };
template <class A, class B> struct CFilter_Bind2 {
    const A& m_first;
    const B& m_second;
    CFilter_Bind2(const A& a, const B& b) : m_first(a), m_second(b) {}
    bool operator()(CThing* thing) const { return m_first(thing) && m_second(thing); }
};
struct CThingSearchToolsMethods : CThingSearchTools {
    CThingList* PeekTypeList(long type) { g_peeked = type; return &g_list; }
    template <class TFilter, class TThing> bool GetPFirstLeafThing(const TFilter& filter, TThing** out);
};
template <class TFilter, class TThing>
bool CThingSearchToolsMethods::GetPFirstLeafThing(const TFilter& filter, TThing** out)
{
    CThingList* things = PeekTypeList(TThing::ThingListType);
    for (CThingList::iterator it = things->begin(); it != things->end(); ++it)
    {
        CThing* thing = *it;
        if (thing->IsIncludedInThingScans() && filter(thing))
        {
            *out = (TThing*)thing;
            return true;
        }
    }
    return false;
}
int main()
{
    int bad = 0;
    BuildList();
    CThingFilter_HasDefName a; CThingFilter_HasScriptData b;
    CFilter_Bind2<CThingFilter_HasDefName, CThingFilter_HasScriptData> f(a, b);
    CThingSearchToolsMethods st; CThingMarker* out = 0;
    if (st.GetPFirstLeafThing(f, &out) || out != 0 || g_peeked != 0x0b) bad = 1;
    g_things[0].m_def = 1;                                 /* def only */
    g_things[1].m_def = 1; g_things[1].m_script = 1; g_things[1].m_scan = false;  /* hidden */
    g_things[2].m_def = 1; g_things[2].m_script = 1;
    if (!st.GetPFirstLeafThing(f, &out) || out != (CThingMarker*)&g_things[2]) bad = 2;
    g_things[1].m_scan = true;
    if (!st.GetPFirstLeafThing(f, &out) || out != (CThingMarker*)&g_things[1]) bad = 3;
    if (bad) { std::printf("005800d6_TEST FAIL\n"); return 1; }
    std::printf("005800d6_TEST PASS\n");
    return 0;
}
