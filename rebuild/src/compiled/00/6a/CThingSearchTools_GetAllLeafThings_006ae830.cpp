// CThingSearchTools::GetAllLeafThings(const TFilter& filter, TContainer& things) [TThing = CThingAICreature, TContainer = list<CThingAICreature*>, TFilter = CThingFilter_None] -- append every scan-visible AI creature of its type list to the result list
// Leaf: GetAllLeafThings
// Retail: Fable.exe (TLC, VC7.1) function @ 0x006AE830, length 115 bytes (`ret 8`).
// Callees: PeekTypeList(long) 0x0049C780, malloc 0x00BFEA0E (the list node).
// Model: CThingSearchTools from engine/CThingSearchTools.h (only passed through to the list getter); CThing, the thing list and
//   the result container are local retail-layout models (retail CThing sits 0x10 below Ego_r.pdb from ThingTypeFlags on; the
//   state byte +0x91 = Dead 0x01, IncludeInThingScans 0x04, Activated 0x08, InLimbo 0x20).
// Shape: the IsIncludedInThingScans test is inlined (`not cl ; test cl,1`); the list push_back is insert(end(), x) over a copy-constructed iterator with
//   _M_create_node and end()->prev read into a local once (the landed 0x004ACF90 model); the filter takes the thing by reference.
// Identity: the catalog label <CThingBuilding, vector<CThingBuilding*>> is propagated; PeekTypeList(1) and the malloc(0x0c) node
//   make this the AI-creature LIST instance (caller CThingPlayerCreature::ResetThingReactions).
#include "engine/CThingSearchTools.h"
#include <stdlib.h>

inline void* operator new(unsigned int, void* where) { return where; }
struct __true_type { __true_type() {} };

// ---- CThing (retail layout; Ego_r.pdb offsets in brackets) -------------------------------------
// Retail CThing sits 0x10 below the PDB from ThingTypeFlags on: UsingComponents +0x20 [PDB +0x20],
// ThingTypeFlags +0x6c [PDB +0x7c], the state bit byte +0x91 [PDB +0xa1] = Dead 0x01,
// IncludeInThingScans 0x04, Activated 0x08, InLimbo 0x20.
class CThing { public:
    enum { ThingTypeMask = 0 };                       // every thing matches the CThing type mask
    unsigned char _pad_0x00[0x20];
    unsigned char UsingComponents[0x24];              // +0x20 bitset<274> of present thing components
    unsigned char _pad_0x44[0x6c - 0x44];
    unsigned long ThingTypeFlags;                     // +0x6c THING_TYPE_* inheritance bits
    unsigned char _pad_0x70[0x91 - 0x70];
    unsigned char StateBits;                          // +0x91 Dead / IncludeInThingScans / Activated / InLimbo

    // CThing::IsIncludedInThingScans (out of line at 0x0048BC40), inlined here.
    bool IsIncludedInThingScans() const
    {
        unsigned char f = StateBits;
        unsigned char nf = ~f;
        if ((f & 0x04) && (f & 0x08) && (nf & 0x01) && !(f & 0x20))
            return true;
        return false;
    }
};

// The type test: the mask is copied into a local first, so the CThing (mask 0) test folds late and
// keeps retail's `xor dl,dl ; jne`, and a byte-sized mask compares through the low byte.
template <class T> inline bool IsThingOfType(const CThing* thing)
{
    unsigned long mask = T::ThingTypeMask;
    return (thing->ThingTypeFlags & mask) == mask;
}

// The thing as the searched type, returned by value (a fresh temporary for push_back's const T&).
template <class T> inline T* ThingCast(CThing* thing) { return (T*)thing; }

// The thing manager's lists: std::list<CThing*> (STLport: the node pointer at +0, node = next, prev, value).
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

// The result: STLport list<T*>; push_back is insert(end(), x), the node comes from the CRT malloc 0x00BFEA0E.
template <class T> struct CResultList {
    struct SNode { SNode* _M_next; SNode* _M_prev; T _M_data; };
    struct iterator {
        SNode* _M_node;
        iterator(SNode* node) : _M_node(node) {}
        iterator(const iterator& o) : _M_node(o._M_node) {}
    };
    SNode* _M_node;
    iterator end() { return iterator(_M_node); }
    SNode* _M_create_node(const T& x)
    {
        SNode* p = (SNode*)malloc(sizeof(SNode));
        new (&p->_M_data) T(x);
        return p;
    }
    iterator insert(iterator position, const T& x)
    {
        SNode* tmp = _M_create_node(x);
        SNode* n = position._M_node;
        SNode* prev = n->_M_prev;
        tmp->_M_next = n;
        tmp->_M_prev = prev;
        prev->_M_next = tmp;
        n->_M_prev = tmp;
        return iterator(tmp);
    }
    void push_back(const T& x) { insert(end(), x); }
};

class CThingAICreature : public CThing { public: enum { ThingTypeMask = 0x1f }; enum { ThingListType = 1 }; };
class CThingFilter_None { public:
    template <class T> bool operator()(const T&) const { return true; }
};

struct CThingSearchToolsMethods : CThingSearchTools {
    CThingList* PeekTypeList(long type);              // 0x0049C780
    template <class TThing, class TContainer, class TFilter> void GetAllLeafThings(const TFilter& filter, TContainer& result);
};

// Every scan-visible thing of the whole thing list that is of type TThing and passes the filter.
template <class TThing, class TContainer, class TFilter>
void CThingSearchToolsMethods::GetAllLeafThings(const TFilter& filter, TContainer& result)
{
    CThingList* things = PeekTypeList(TThing::ThingListType);
    for (CThingList::iterator it = things->begin(); it != things->end(); ++it)
    {
        CThing* thing = *it;
        if (thing->IsIncludedInThingScans() && filter(*(TThing*)thing))
            result.push_back((TThing*)thing);
    }
}

template void CThingSearchToolsMethods::GetAllLeafThings<CThingAICreature, CResultList<CThingAICreature*>, CThingFilter_None >(const CThingFilter_None&, CResultList<CThingAICreature*>&);