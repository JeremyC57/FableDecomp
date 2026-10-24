// CThingSearchTools::GetAllLeafThings(const TFilter& filter, TContainer& things) [TThing = CThingAICreature, TContainer = vector<CThingAICreature*>, TFilter = CFilter_Bind2<CThingFilter_EnemyCreature, CThingFilter_InMap>] -- append every scan-visible AI creature of its type list that passes both filters
// Leaf: GetAllLeafThings
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00618790, length 155 bytes (`ret 8`).
// Callees: PeekTypeList(long) 0x0049C780, the first filter's operator() 0x006A7550 (label propagated), CThingFilter_InMap::operator() 0x006A7930, vector::_M_insert_overflow 0x00515CC0.
// Model: CThingSearchTools from engine/CThingSearchTools.h (only passed through to the list getter); CThing, the thing list and
//   the result container are local retail-layout models (retail CThing sits 0x10 below Ego_r.pdb from ThingTypeFlags on; the
//   state byte +0x91 = Dead 0x01, IncludeInThingScans 0x04, Activated 0x08, InLimbo 0x20).
// Shape: the IsIncludedInThingScans test is inlined (`not cl ; test cl,1`); a vector push_back of an rvalue `(T*)thing` lands the value in the dead container argument slot,
//   and the same-TU noinline _M_insert_overflow stand-in lets VC7.1 home the __true_type tag in the filter argument slot.
#include "engine/CThingSearchTools.h"

struct CThing {
    unsigned char _pad_0x00[0x20];
    unsigned long UsingComponents[9];                 // +0x20
    unsigned char _pad_0x44[0x6c - 0x44];
    unsigned long ThingTypeFlags;                     // +0x6c
    unsigned char _pad_0x70[0x91 - 0x70];
    unsigned char StateBits;                          // +0x91
    bool IsIncludedInThingScans() const
    {
        unsigned char f = StateBits;
        unsigned char nf = ~f;
        if ((f & 0x04) && (f & 0x08) && (nf & 0x01) && !(f & 0x20))
            return true;
        return false;
    }
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
struct CThingAICreature : CThing { enum { ThingListType = 1 }; };

inline void* operator new(unsigned int, void* where) { return where; }
struct __true_type { __true_type() {} };

namespace std {
template <class T> struct vector {
    T* _M_start;
    T* _M_finish;
    T* _M_end_of_storage;
    void _M_insert_overflow(T* position, const T& x, const __true_type&, unsigned long fill_len, bool atend);
    void push_back(const T& x)
    {
        if (_M_finish != _M_end_of_storage) {
            new (_M_finish) T(x);
            ++_M_finish;
        }
        else
            _M_insert_overflow(_M_finish, x, __true_type(), 1, true);
    }
};
}
#pragma auto_inline(off)
template <> void std::vector<CThingAICreature*>::_M_insert_overflow(CThingAICreature** position, CThingAICreature* const& x, const __true_type&, unsigned long fill_len, bool atend)   // 0x00515CC0
{
    if (atend && fill_len == 1)
        _M_finish = position + (x != 0);
}
#pragma auto_inline(on)

struct CThingFilter_EnemyCreature {
    void* Self;
    bool operator()(CThing* thing) const;             // 0x006A7550
};
struct CThingFilter_InMap {
    void* Map;
    bool operator()(CThing* thing) const;             // 0x006A7930
};
template <class A, class B> struct CFilter_Bind2 {
    const A* Filter1;
    const B* Filter2;
    template <class T> bool operator()(T* thing) const { return (*Filter1)(thing) && (*Filter2)(thing); }
};

struct CThingSearchToolsMethods : CThingSearchTools {
    CThingList* PeekTypeList(long type);              // 0x0049C780
    template <class TThing, class TContainer, class TFilter> void GetAllLeafThings(const TFilter& filter, TContainer& things);
};
template <class TThing, class TContainer, class TFilter>
void CThingSearchToolsMethods::GetAllLeafThings(const TFilter& filter, TContainer& things)
{
    CThingList* list = PeekTypeList(TThing::ThingListType);
    for (CThingList::iterator it = list->begin(); it != list->end(); ++it)
    {
        CThing* thing = *it;
        if (thing->IsIncludedInThingScans() && filter((TThing*)thing))
            things.push_back((TThing*)thing);
    }
}
template void CThingSearchToolsMethods::GetAllLeafThings<CThingAICreature, std::vector<CThingAICreature*>, CFilter_Bind2<CThingFilter_EnemyCreature, CThingFilter_InMap> >(const CFilter_Bind2<CThingFilter_EnemyCreature, CThingFilter_InMap>&, std::vector<CThingAICreature*>&);