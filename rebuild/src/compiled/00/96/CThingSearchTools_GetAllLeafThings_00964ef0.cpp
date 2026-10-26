// CThingSearchTools::GetAllLeafThings(const TFilter& filter, TContainer& things) [TThing = CThingMarker, TContainer = vector<CThingMarker*>, TFilter = CThingFilter_MarkerHasProperty] -- append every scan-visible marker (type list 0x0b) that has the filter's property
// Leaf: GetAllLeafThings
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00964EF0, length 136 bytes (`ret 8`).
// Callees: PeekTypeList(long) 0x0049C780, CThingFilter_MarkerHasProperty::operator() 0x006A75B0, vector::_M_insert_overflow 0x0062E190.
// Model: CThingSearchTools from engine/CThingSearchTools.h (only passed through to the list getter); CThing, the thing list and
//   the result container are local retail-layout models (retail CThing sits 0x10 below Ego_r.pdb from ThingTypeFlags on; the
//   state byte +0x91 = Dead 0x01, IncludeInThingScans 0x04, Activated 0x08, InLimbo 0x20).
// Shape: the IsIncludedInThingScans test is inlined (`not cl ; test cl,1`); a vector push_back of an rvalue `(T*)thing` lands the value in the dead container argument slot,
//   and the same-TU noinline _M_insert_overflow stand-in lets VC7.1 home the __true_type tag in the filter argument slot.
// Identity: the catalog label <CThingAICreature, ..., CFilter_Bind2<...>> is propagated; PeekTypeList(0x0b), the single marker filter and
//   the caller CAIStateGroup_StealStealableItem::FindPEscapePoint make this the marker instance.
#include "engine/CThingSearchTools.h"

class CThing { public:
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
class CThingMarker : public CThing { public: enum { ThingListType = 0xb }; };

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
template <> void std::vector<CThingMarker*>::_M_insert_overflow(CThingMarker** position, CThingMarker* const& x, const __true_type&, unsigned long fill_len, bool atend)   // 0x0062E190
{
    if (atend && fill_len == 1)
        _M_finish = position + (x != 0);
}
#pragma auto_inline(on)

class CThingFilter_MarkerHasProperty { public:
    void* Property;
    bool operator()(CThingMarker* marker) const;      // 0x006A75B0
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
template void CThingSearchToolsMethods::GetAllLeafThings<CThingMarker, std::vector<CThingMarker*>, CThingFilter_MarkerHasProperty >(const CThingFilter_MarkerHasProperty&, std::vector<CThingMarker*>&);