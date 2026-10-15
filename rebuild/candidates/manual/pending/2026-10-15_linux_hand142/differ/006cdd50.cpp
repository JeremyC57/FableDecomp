// CGroundNavigatorExternalInterface::~CGroundNavigatorExternalInterface() -- removes the creature's navigation obstacle record from the global obstacle list, then the CNavigatorExternalInterface base destructor (its vtable store)
// Grade: DIFFER(ours v retail) -- 176v155: the body and loops match; retail pushes edi in the prologue (before the mask test) so every path shares one epilogue, ours pushes edi after the null test and emits separate early-out epilogues (plus the loop-head `mov edi,edi` pad moves) | tried: STLport list::remove, find + while(next != end), for(;;) with an explicit next test, for(;;) with the single find (this file) | next: make the absent-component path reach the same exit (e.g. no early `if (obstacles)` split, or the sweep as an auto_inline(off) helper defined above)
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
struct CObstaclesList {                            // STLport list<CNavigatorObstacles*>: the sentinel node pointer
    _List_node* _M_node;
};
extern CObstaclesList g_registeredObstacles;       // 0x013D2834 (the obstacle records of every live ground navigator)
__forceinline _List_node* Find(_List_node* first, _List_node* last, CNavigatorObstacles* value)   // std::find over the list
{
    while (first != last && first->_M_data != value)
        first = first->_M_next;
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
        _List_node* next = Find(g_registeredObstacles._M_node->_M_next, g_registeredObstacles._M_node, obstacles)->_M_next;
        for (;;) {
            _List_node* it = Find(next, g_registeredObstacles._M_node, obstacles);
            if (it == g_registeredObstacles._M_node)
                break;
            _List_node* after = it->_M_next;
            _List_node* before = it->_M_prev;
            before->_M_next = after;
            after->_M_prev = before;
            free(it);
        }
    }
}
