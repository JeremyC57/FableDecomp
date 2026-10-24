// Grade: DIFFER(121v121) -- register assignment only: retail holds the thing in ecx (`mov ecx,[edi+8]`, the not-copy in dl, `push ecx` for x after `push edx` for the tag); ours holds it in edx (not-copy in cl) -- every instruction and offset otherwise identical
// CThingSearchTools::GetAllLeafThings(const TFilter& filter, TContainer& things) [TThing = const CThingBuilding, TContainer = vector<const CThingBuilding*>, TFilter = CThingFilter_None] -- append every scan-visible building of its type list (3) to the result vector
// Leaf: GetAllLeafThings
// Retail: Fable.exe (TLC, VC7.1) function @ 0x004AD700, length 121 bytes (`ret 8`).
// Callees: PeekTypeList(long) 0x0049C780, vector::_M_insert_overflow 0x004AC530 (same-TU stand-in).
// Identity: the catalog names the filter CThingFilter_HasTC; the body calls no filter at all (no bitset test either), so the
//   instance filter is CThingFilter_None (or an inline always-true filter). Caller CWorld::PreparePrimitivesForEngine.
// Tried: named CThing* local (this file), a TThing* local, re-reading *it for every use, ThingCast<T> by-value temporary +
//   the filter taking T& (all four give the same edx/ecx swap). The sibling 0x006186F0 (same template, real filters) matches.
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
struct CThingBuilding : CThing { enum { ThingListType = 3 }; };

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
template <> void std::vector<const CThingBuilding*>::_M_insert_overflow(const CThingBuilding** position, const CThingBuilding* const& x, const __true_type&, unsigned long fill_len, bool atend)   // 0x004AC530
{
    if (atend && fill_len == 1)
        _M_finish = position + (x != 0);
}
#pragma auto_inline(on)

struct CThingFilter_None {
    template <class T> bool operator()(T*) const { return true; }
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
        if (thing->IsIncludedInThingScans())
            things.push_back((TThing*)thing);
    }
}
template void CThingSearchToolsMethods::GetAllLeafThings<const CThingBuilding, std::vector<const CThingBuilding*>, CThingFilter_None >(const CThingFilter_None&, std::vector<const CThingBuilding*>&);
