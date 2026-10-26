// CTCHeroOpinionDeedLog::IsDeedWitnessed(long, bool) -- true when any live creature with an opinion-of-hero component knows of the deed
// Leaf: IsDeedWitnessed
// Retail: Fable.exe (TLC, VC7.1) function @ 0x006C5E90, 227 bytes (+ int3 padding), `ret 8`.
// Callees: malloc @ 0x00BFEA0E (the list header node), free @ 0x00BFEA14 (the inlined list destructor),
//          CTCBase::GetThingSearchTools() const @ 0x00686D90, CThingSearchTools::
//          GetAllThings<CThing, list<CThing*>, CFilter_Bind2<CThingFilter_None, CThingFilter_HasTC> >(filter, list)
//          @ 0x004ACF90 (manifest names a vector instantiation of the same template), CVectorMap<ETCInterfaceType,
//          CTCBase*>::LowerBound @ 0x0040F020, CTCCreatureOpinionOfHero::KnowsOfDeed(long, bool) @ 0x00683650, the
//          out-of-line list destructor @ 0x004AB4F0 (label "CGuiControlTreePane::SortTreeRecursively" is propagated).
// Shape: a local list<CThing*> (self-linked 12-byte header) is filled with every thing passing a 4-byte HasTC(0x4F)
// filter (CThingFilter_None is an empty base); each live entry (IS_THING_ALIVE inlined) with the opinion component
// (masked HasTC word, `test ch,ch ; jns`) is asked KnowsOfDeed (the GetTC key homed in the dead `deed` slot, the
// deed held in ebx, the component loaded straight into ecx by the one-ternary GetTC). The list header pointer
// stays in edi across GetAllThings because that template's body is compiled earlier in the TU (stand-in below). The list's in-class destructor is inlined on the shared `return false` path and called out of
// line on the early `return true` (VC7.1 inlines an in-class inline member at its first call only).
// Layout: CThing interface bitset +0x20, map +0x44, dead flag +0x91 bit 0.
class CTCBase;

extern "C" void* __cdecl malloc(unsigned int size);  // 0x00BFEA0E
extern "C" void __cdecl free(void* block);           // 0x00BFEA14

struct CInterfaceMap {
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type);            // 0x0040F020
};

class CThing { public:
    unsigned char _pad0[0x20];
    unsigned long m_presentBits[9];                  // +0x20 std::bitset of interface types
    CInterfaceMap m_interfaces;                      // +0x44
    unsigned char _pad1[0x45];
    unsigned char m_flags91;                         // +0x91 bit 0: dead
    unsigned long HasTC(long interfaceType) const
    {
        return m_presentBits[interfaceType >> 5] & (1UL << (interfaceType & 31));
    }
    template<class T> __forceinline T* GetTC(long interfaceType)
    {
        long type = interfaceType;
        CInterfaceMap* map = &m_interfaces;
        CInterfaceMap::SEntry* it = map->LowerBound(&type);
        return (T*)((it == map->last || interfaceType < it->type) ? map->last : it)->component;
    }
};

inline bool IS_THING_ALIVE(const CThing* thing)
{
    if (thing != 0) {
        if (!(thing->m_flags91 & 1))
            return true;
    }
    return false;
}

struct SThingListNode { SThingListNode* _M_next; SThingListNode* _M_prev; CThing* _M_data; };
namespace N_004ab4f0 { struct CThingList {                                  // std::list<CThing*>: the header-node pointer
    SThingListNode* _M_node;
    CThingList()
    {
        SThingListNode* node = (SThingListNode*)malloc(sizeof(SThingListNode));
        node->_M_next = node;
        node->_M_prev = node;
        _M_node = node;
    }
    ~CThingList()                                    // out-of-line copy @ 0x004AB4F0
    {
        SThingListNode* cur = _M_node->_M_next;
        while (cur != _M_node) {
            SThingListNode* victim = cur;
            cur = cur->_M_next;
            free(victim);
        }
        _M_node->_M_next = _M_node;
        _M_node->_M_prev = _M_node;
        free(_M_node);
    }
}; } using N_004ab4f0::CThingList;

class CThingFilter_None { public:
};
class CThingFilter_HasTC { public:
    long Type;
};
template <class A, class B> class CFilter_Bind2 : public A { public:
    B Second;
};

class CThingSearchTools { public:
    CThing** m_pFirst;                               // stand-in model of the searched things
    CThing** m_pLast;
    template <class T, class C, class F> void GetAllThings(const F& filter, C& found);   // 0x004ACF90
};

// Stand-in body of the GetAllThings instantiation (0x004ACF90): the template is instantiated in this TU in the
// original build, so VC7.1 compiled its body before the caller; compiled here above the caller with auto_inline
// off so the caller knows the list header pointer survives the call. Only IsDeedWitnessed is graded.
#pragma auto_inline(off)
template <class T, class C, class F> void CThingSearchTools::GetAllThings(const F& filter, C& found)
{
    for (CThing** it = m_pFirst; it != m_pLast; ++it) {
        if ((*it)->HasTC(filter.Second.Type)) {
            SThingListNode* node = (SThingListNode*)malloc(sizeof(SThingListNode));
            SThingListNode* head = found._M_node;
            node->_M_data = *it;
            node->_M_next = head;
            node->_M_prev = head->_M_prev;
            head->_M_prev->_M_next = node;
            head->_M_prev = node;
        }
    }
}
#pragma auto_inline(on)

class CTCCreatureOpinionOfHero { public:                    // interface type 0x4F
    bool KnowsOfDeed(long deed, bool flag);          // 0x00683650
};

class CTCHeroOpinionDeedLog { public:
    void*   __vftable;                               // +0x00
    CThing* ParentThing;                             // +0x04 CTCBase
    CThingSearchTools* GetThingSearchTools() const;  // 0x00686D90 (CTCBase)
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