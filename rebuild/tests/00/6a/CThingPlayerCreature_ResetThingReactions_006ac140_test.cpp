// Behaviour test for 006ac140: the model below is a copy of src/006ac140.cpp (compiled separately), driven by test doubles.
#include <cstdio>
#include <cstring>
#include <cstdlib>
#include <new>
static int g_mallocs = 0, g_frees = 0, g_sets = 0, g_gets = 0;
static void* my_malloc(unsigned int size) { ++g_mallocs; return std::malloc(size); }
static void my_free(void* p) { if (p) ++g_frees; std::free(p); }
#define malloc my_malloc
#define free my_free

class CBaseObject { public:};
struct CBaseIntelligentPointer {
    CBaseObject* m_pItem;
    CBaseIntelligentPointer() : m_pItem(0) {}
    ~CBaseIntelligentPointer() {}
    void SetPItem(CBaseObject* item) { ++g_sets; m_pItem = item; }
    CBaseObject* GetPItem() const { ++g_gets; return m_pItem; }
};
template <class T> class CIntelligentPointer : public CBaseIntelligentPointer { public:
    CIntelligentPointer(T* item) { SetPItem((CBaseObject*)item); }
    CIntelligentPointer& operator=(const CIntelligentPointer& other) { CBaseObject* item = other.GetPItem(); SetPItem(item); return *this; }
};
class CThing { public:
    unsigned char _pad00[0x91];
    unsigned char m_flags;
    unsigned char _pad92[0xf4 - 0x92];
    CIntelligentPointer<CThing> m_reactionTarget;
    CThing() : m_reactionTarget(0) {}
};
struct SThingNode {
    SThingNode* _M_next;
    SThingNode* _M_prev;
    CThing*     _M_data;
};
struct CThingList {
    SThingNode* _M_node;
    CThingList()
    {
        SThingNode* node = (SThingNode*)malloc(sizeof(SThingNode));
        node->_M_next = node;
        node->_M_prev = node;
        _M_node = node;
    }
    ~CThingList()
    {
        clear();
        free(_M_node);
    }
    void clear()
    {
        SThingNode* cur = _M_node->_M_next;
        while (cur != _M_node) {
            SThingNode* tmp = cur;
            cur = cur->_M_next;
            free(tmp);
        }
        _M_node->_M_next = _M_node;
        _M_node->_M_prev = _M_node;
    }
};
class CThingFilter_None { public: CThingFilter_None() { } };
static CThingList* g_all = 0;
class CThingSearchTools { public:
    CThingList* GetThingList(long type) { return type == 1 ? g_all : 0; }
    void GetAllLeafThings(const CThingFilter_None& filter, CThingList& out);
};
void CThingSearchTools::GetAllLeafThings(const CThingFilter_None&, CThingList& out)
{
    CThingList* all = GetThingList(1);
    for (SThingNode* n = all->_M_node->_M_next; n != all->_M_node; n = n->_M_next) {
        CThing* thing = n->_M_data;
        unsigned char flags = thing->m_flags;
        if ((flags & 4) && (flags & 8) && ((unsigned char)~flags & 1) && !(flags & 0x20)) {
            SThingNode* pos = out._M_node;
            SThingNode* node = (SThingNode*)malloc(sizeof(SThingNode));
            new (&node->_M_data) CThing*(thing);
            SThingNode* prev = pos->_M_prev;
            node->_M_next = pos;
            node->_M_prev = prev;
            prev->_M_next = node;
            pos->_M_prev = node;
        }
    }
}
static CThingSearchTools g_tools;
class CThingPlayerCreature : public CThing { public:
    CThingSearchTools* GetThingSearchTools() { return &g_tools; }
    void ResetThingReactions();
};

void CThingPlayerCreature::ResetThingReactions()
{
    CThingList things;
    GetThingSearchTools()->GetAllLeafThings(CThingFilter_None(), things);
    for (SThingNode* node = things._M_node->_M_next; node != things._M_node; node = node->_M_next) {
        CThing* thing = node->_M_data;
        thing->m_reactionTarget = CIntelligentPointer<CThing>(0);
    }
}

static void Append(CThingList& list, CThing* thing)
{
    SThingNode* node = (SThingNode*)my_malloc(sizeof(SThingNode)); node->_M_data = thing;
    SThingNode* pos = list._M_node; node->_M_next = pos; node->_M_prev = pos->_M_prev; pos->_M_prev->_M_next = node; pos->_M_prev = node;
}
int main()
{
    bool ok = true;
    static CThing things[5]; static CBaseObject target;
    unsigned char flags[5] = { 0x0c, 0x0d, 0x2c, 0x08, 0x0c };   // leaf, dead, limbo, not active, leaf
    for (int i = 0; i < 5; ++i) { things[i].m_flags = flags[i]; things[i].m_reactionTarget.m_pItem = &target; }
    CThingList all; g_all = &all;
    for (int i = 0; i < 5; ++i) Append(all, &things[i]);
    CThingPlayerCreature player;
    g_mallocs = g_frees = g_sets = g_gets = 0;
    player.ResetThingReactions();
    ok = ok && things[0].m_reactionTarget.m_pItem == 0 && things[4].m_reactionTarget.m_pItem == 0;   // the two leaf things cleared
    ok = ok && things[1].m_reactionTarget.m_pItem == &target && things[2].m_reactionTarget.m_pItem == &target && things[3].m_reactionTarget.m_pItem == &target;
    ok = ok && g_mallocs == 3 && g_frees == 3;                        // the temporary list: head + two nodes, all freed
    ok = ok && g_sets == 4 && g_gets == 2;                            // per cleared thing: temp set, copy read, member set
    if (!ok) { std::printf("006ac140_TEST FAIL\n"); return 1; }
    std::printf("006ac140_TEST PASS\n");
    return 0;
}