// CThingSearchTools::GetAllThings(const TFilter& filter, TContainer& result) [TThing = CThing, TContainer = std::vector<CThing*>, TFilter = CThingFilter_None] -- append every scan-visible thing to the result vector
// Leaf: GetAllThings
// Retail: Fable.exe (TLC, VC7.1) @ 0x008093D0, 125 bytes (`ret 8`).
// Identity (proven): Ego_r.pdb's player-targeting TU (anon ?A0x58ed6966) instantiates exactly
//   ??$GetAllThings@VCThing@@V?$vector@PAVCThing@@...@std@@VCThingFilter_None@@@CThingSearchTools@@QBEXABVCThingFilter_None@@AAV...@Z;
//   its one caller 0x00803B6E (in the targeting TU, after CTCTargetingPlayer::PostProcessTargetedThing 0x00803B20) passes an
//   empty filter byte and a zeroed local vector<CThing*>. The body is the landed 0x0052B270 instantiation with the CThing
//   type mask (0: the `xor al,al ; jne` dead test) and the vector<CThing*> overflow.
// Callees: PeekAllThings 0x0049C770, vector<CThing*>::_M_insert_overflow 0x00436AC0; CThingFilter_None inlines to true.
// Model/shape: as landed 0x0052B270 (CThingSearchTools from engine/CThingSearchTools.h; scan test, type-mask test with the mask
//   in a local, filter; push_back with a by-value ThingCast temporary and a same-TU noinline overflow stand-in).
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

// The result: STLport vector<T*>; the overflow path is out of line (a same-TU stand-in below, so VC7.1 knows it
// never reads the __true_type tag and homes the tag and the pushed value in the dead argument slot as retail does).
template <class T> struct CResultVector {
    T* _M_start;
    T* _M_finish;
    T* _M_end_of_storage;
    void _M_insert_overflow(T* position, const T& x, const __true_type&, unsigned long fill_len, bool atend);   // 0x00436AC0
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
#pragma auto_inline(off)
template <class T> __declspec(noinline) void CResultVector<T>::_M_insert_overflow(T* position, const T& x, const __true_type&, unsigned long fill_len, bool atend)
{
    if (atend && fill_len == 1)
        _M_finish = position + (x != 0);
}
#pragma auto_inline(on)

class CThingFilter_None { public:
    bool operator()(const CThing&) const { return true; }
};

struct CThingSearchToolsMethods : CThingSearchTools {
    CThingList* PeekAllThings();                      // 0x0049C770: &ThingManager->(all-things list at +0x18)
    template <class TThing, class TContainer, class TFilter> void GetAllThings(const TFilter& filter, TContainer& result);
};

// Every scan-visible thing of the whole thing list that is of type TThing and passes the filter.
template <class TThing, class TContainer, class TFilter>
void CThingSearchToolsMethods::GetAllThings(const TFilter& filter, TContainer& result)
{
    CThingList* things = PeekAllThings();
    for (CThingList::iterator it = things->begin(); it != things->end(); ++it)
    {
        CThing* thing = *it;
        if (thing->IsIncludedInThingScans() && IsThingOfType<TThing>(thing) && filter(*(TThing*)thing))
            result.push_back(ThingCast<TThing>(thing));
    }
}

template void CThingSearchToolsMethods::GetAllThings<CThing, CResultVector<CThing*>, CThingFilter_None >(const CThingFilter_None&, CResultVector<CThing*>&);