// Behaviour test for 006cdd50: src/006cdd50.cpp copied verbatim (the function body and its same-TU stand-ins), the remaining
// declared callees stubbed below so calls and arguments are observable. CGroundNavigatorExternalInterface::~CGroundNavigatorExternalInterface()
#include <cstdio>
#include <cstring>
static int fails = 0;
#define CHECK(c) do { if (!(c)) { ++fails; printf("FAIL %d: %s\n", __LINE__, #c); } } while (0)
// ---- source (verbatim) ----
// CGroundNavigatorExternalInterface::~CGroundNavigatorExternalInterface() -- removes the creature's navigation obstacle record from the global obstacle list, then the CNavigatorExternalInterface base destructor (its vtable store)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x006CDD50, length 158 bytes (padded to 160).
// Callees: the component-map LowerBound @ 0x0040F020 (kind 7), free @ 0x00BFEA14 (each removed list node).
// Globals: the obstacle list @ 0x013D2834 (STLport list<CNavigatorObstacles*>, its sentinel pointer).
// Vtables: CGroundNavigatorExternalInterface 0x0125EC9C (first), CNavigatorExternalInterface 0x01241D74 (the inline base
// destructor, last).
// Shape: the inline GetPNavigatorObstacles (out-parameter lookup, null without a kind-7 component) guarded by a null test,
// then a find-based duplicate sweep: `next` is the node after the FIRST match (std::find; when there is none, the list
// head), and every later match found from that fixed `next` is unlinked and freed (the first match itself stays; the
// sentinel is re-read after each free). This is what retail does, odd as it reads.
// Model: local polymorphic classes (engine/CGroundNavigatorExternalInterface.h flattens the vptr; Creature +4, Radius +8).
// Leaf: ??1CGroundNavigatorExternalInterface
extern "C" void* malloc(unsigned int size);        // 0x00BFEA0E
extern "C" void free(void* p);                     // 0x00BFEA14
struct CTCBase_Retail;
struct STCEntry_Retail {                           // one CVectorMap<ETCInterfaceType, CTCBase*> element
    long            m_kind;
    CTCBase_Retail* m_pTC;
};
struct CTCMap_Retail {                             // the thing's component map at +0x44: a sorted vector of pairs
    STCEntry_Retail* m_pFirst;
    STCEntry_Retail* m_pLast;
    STCEntry_Retail* m_pEnd;
    STCEntry_Retail* LowerBound(const unsigned long& kind);   // 0x0040F020 (the key by address)
    __forceinline CTCBase_Retail* Find(long kind)
    {
        STCEntry_Retail* it = LowerBound((unsigned long)kind);
        if (it == m_pLast || kind < it->m_kind)
            it = m_pLast;
        return it->m_pTC;
    }
};
struct CThingCreatureBase {
    unsigned char  _pad00[0x20];
    unsigned long  m_tcMask[8];                    // +0x20: which component kinds the thing holds
    unsigned char  _pad40[4];
    CTCMap_Retail  m_tcs;                          // +0x44
    __forceinline unsigned long HasTC(long kind) const { return m_tcMask[kind >> 5] & (1UL << (kind & 31)); }
    template <class T> __forceinline bool GetInterface(T** out, long kind)   // the out-parameter lookup: the component stays in eax
    {
        if (HasTC(kind)) {
            *out = (T*)m_tcs.Find(kind);
            return true;
        }
        return false;
    }
};
struct CNavigatorObstacles;
struct CTCCreatureNavigation_Retail {              // component kind 7
    unsigned char _pad[0x40];
    unsigned char m_obstacles[4];                  // +0x40 the obstacle record
};
struct _List_node {
    _List_node*          _M_next;
    _List_node*          _M_prev;
    CNavigatorObstacles* _M_data;
};
struct _List_iterator {
    _List_node* _M_node;
    _List_iterator(_List_node* n) : _M_node(n) {}
    _List_iterator(const _List_iterator& o) : _M_node(o._M_node) {}
    CNavigatorObstacles*& operator*() const { return _M_node->_M_data; }
    _List_iterator& operator++() { _M_node = _M_node->_M_next; return *this; }
    bool operator==(const _List_iterator& o) const { return _M_node == o._M_node; }
    bool operator!=(const _List_iterator& o) const { return _M_node != o._M_node; }
};
struct CObstaclesList {                            // STLport list<CNavigatorObstacles*>: the sentinel node pointer
    _List_node* _M_node;
    _List_iterator begin() { return _List_iterator(_M_node->_M_next); }
    _List_iterator end() { return _List_iterator(_M_node); }
    _List_iterator erase(_List_iterator position)
    {
        _List_node* after = position._M_node->_M_next;
        _List_node* before = position._M_node->_M_prev;
        before->_M_next = after;
        after->_M_prev = before;
        free(position._M_node);
        return _List_iterator(after);
    }
};
extern CObstaclesList g_registeredObstacles;       // 0x013D2834 (the obstacle records of every live ground navigator)
__forceinline _List_iterator Find(_List_iterator first, _List_iterator last, CNavigatorObstacles* const& value)   // std::find over the list
{
    while (first != last && *first != value)
        ++first;
    return first;
}
struct CNavigatorExternalInterface {               // vtable 0x01241D74
    virtual void v00();
    virtual ~CNavigatorExternalInterface() {}
};
struct CGroundNavigatorExternalInterface : CNavigatorExternalInterface {   // vtable 0x0125EC9C
    CThingCreatureBase* Creature;                  // +0x04
    float               Radius;                    // +0x08
    __forceinline CNavigatorObstacles* GetPNavigatorObstacles()   // inline twin of 0x006CD7B0
    {
        CTCCreatureNavigation_Retail* navigation;
        if (Creature->GetInterface(&navigation, 7))
            return (CNavigatorObstacles*)navigation->m_obstacles;
        return 0;
    }
    virtual void v00();
    CGroundNavigatorExternalInterface(const CGroundNavigatorExternalInterface& other);
    virtual ~CGroundNavigatorExternalInterface();
};

CGroundNavigatorExternalInterface::~CGroundNavigatorExternalInterface()
{
    CNavigatorObstacles* obstacles = GetPNavigatorObstacles();
    if (obstacles) {
        _List_iterator it = Find(g_registeredObstacles.begin(), g_registeredObstacles.end(), obstacles);
        _List_iterator next = it;
        ++next;
        for (;;) {
            _List_iterator found = Find(next, g_registeredObstacles.end(), obstacles);
            if (found == g_registeredObstacles.end())
                break;
            g_registeredObstacles.erase(found);
        }
    }
}

// ---- stubs and driver ----
#include <stdlib.h>
STCEntry_Retail* CTCMap_Retail::LowerBound(const unsigned long& kind)
{
    STCEntry_Retail* it = m_pFirst;
    while (it != m_pLast && (unsigned long)it->m_kind < kind) ++it;
    return it;
}
CObstaclesList g_registeredObstacles;
void CNavigatorExternalInterface::v00() {}
void CGroundNavigatorExternalInterface::v00() {}
static _List_node* Push(_List_node* head, CNavigatorObstacles* v)
{
    _List_node* n = (_List_node*)malloc(sizeof(_List_node)); n->_M_data = v;
    n->_M_next = head; n->_M_prev = head->_M_prev; head->_M_prev->_M_next = n; head->_M_prev = n; return n;
}
static int Count(_List_node* h, CNavigatorObstacles* v) { int c = 0; for (_List_node* n = h->_M_next; n != h; n = n->_M_next) if (!v || n->_M_data == v) ++c; return c; }
struct TestNav : CGroundNavigatorExternalInterface { TestNav(const TestNav& o) : CGroundNavigatorExternalInterface(o) {} };
CGroundNavigatorExternalInterface::CGroundNavigatorExternalInterface(const CGroundNavigatorExternalInterface& other) : Creature(other.Creature), Radius(other.Radius) {}
int main()
{
    _List_node head; head._M_next = &head; head._M_prev = &head; g_registeredObstacles._M_node = &head;
    static CThingCreatureBase creature; memset(&creature, 0, sizeof creature);
    static CTCCreatureNavigation_Retail nav;
    STCEntry_Retail entries[2] = { { 3, 0 }, { 7, (CTCBase_Retail*)&nav } };
    creature.m_tcs.m_pFirst = entries; creature.m_tcs.m_pLast = entries + 2; creature.m_tcs.m_pEnd = entries + 2;
    creature.m_tcMask[0] = 1UL << 7;
    CNavigatorObstacles* mine = (CNavigatorObstacles*)nav.m_obstacles; CNavigatorObstacles* other = (CNavigatorObstacles*)0x7777;
    Push(&head, other); Push(&head, mine); Push(&head, other); Push(&head, mine); Push(&head, mine);
    {
        static unsigned char store[sizeof(CGroundNavigatorExternalInterface)];
        CGroundNavigatorExternalInterface* g = (CGroundNavigatorExternalInterface*)store;
        *(void**)g = 0; g->Creature = &creature; g->Radius = 1.0f;
        g->CGroundNavigatorExternalInterface::~CGroundNavigatorExternalInterface();
    }
    // the first registration survives, every later one is erased
    CHECK(Count(&head, 0) == 3); CHECK(Count(&head, mine) == 1); CHECK(Count(&head, other) == 2);
    CHECK(head._M_next->_M_data == other); CHECK(head._M_next->_M_next->_M_data == mine);
    // a creature without a navigation component leaves the list alone
    creature.m_tcMask[0] = 0; Push(&head, mine);
    {
        static unsigned char store[sizeof(CGroundNavigatorExternalInterface)];
        CGroundNavigatorExternalInterface* g = (CGroundNavigatorExternalInterface*)store;
        *(void**)g = 0; g->Creature = &creature;
        g->CGroundNavigatorExternalInterface::~CGroundNavigatorExternalInterface();
    }
    CHECK(Count(&head, 0) == 4);
    if (fails == 0) printf("006cdd50_TEST PASS\n"); else printf("006cdd50_TEST FAIL\n");
    return fails;
}