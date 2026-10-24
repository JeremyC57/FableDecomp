// CThingSearchTools::GetPBestThing(const TFilter& filter, const TCompare& compare, TThing** out) [TFilter = CFilter_Bind2<std::binary_compose<std::logical_or<bool>, CThingFilter_HasTC, CThingFilter_HasTC>, CThingFilter_In2DRange>, TCompare = CThingCompare_Nearest, TThing = CThing] -- the nearest scan-visible thing that has either of two thing components and is in 2D range
// Leaf: GetPBestThing
// Retail: Fable.exe (TLC, VC7.1) @ 0x004AC2C0, 180 bytes (+ int3 padding to 192) (`ret 0Ch`).
// Callees: PeekAllThings 0x0049C770, CThingFilter_HasTC 0x006A80A0 (twice: _M_fn3 then _M_fn2, right-to-left arguments), CThingFilter_In2DRange 0x006A76B0, CThingCompare_Nearest::operator()<CThing> 0x004A8D80.
// Model: CThingSearchTools from engine/CThingSearchTools.h (`this` is only passed through to the list getter);
// CThing is a local retail-layout model; the filters and comparators are local models (Ego_r.pdb members),
// called out of line with the thing by reference; the CFilter_BindN / CFilter_Not hold their filters by reference.
// Shape: the template body of the landed siblings (0x00936950 GetPBestLeafThing, GetPBestThing re-reading `*it`).
#include "engine/CThingSearchTools.h"

// ---- CThing (retail layout; Ego_r.pdb offsets in brackets) -------------------------------------
// Retail CThing sits 0x10 below the PDB from ThingTypeFlags on: ThingTypeFlags +0x6c [PDB +0x7c], the state
// bit byte +0x91 [PDB +0xa1] = Dead 0x01, IncludeInThingScans 0x04, Activated 0x08, InLimbo 0x20.
struct CThing {
    enum { ThingTypeMask = 0 };                       // every thing matches the CThing type mask
    unsigned char _pad_0x00[0x6c];
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

// The type test: the mask is copied into a local first, so VC7.1 folds `x & 0` for CThing late
// and keeps retail's `xor al,al ; jne` (a constant enum operand folds the whole test away).
template <class T> inline bool IsThingOfType(const CThing* thing)
{
    unsigned long mask = T::ThingTypeMask;
    return (thing->ThingTypeFlags & mask) == mask;
}
// std::list<CThing*> (STLport: the node pointer at +0, node = next, prev, value).
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

struct C3DVector { float X, Y, Z; };
struct CThingFilter_HasTC {
    long Type;                                        // ETCInterfaceType
    bool operator()(const CThing& thing) const;       // 0x006A80A0
};
namespace std {
template <class T> struct logical_or { bool operator()(const T& x, const T& y) const { return x || y; } };
template <class Op1, class Op2, class Op3> class binary_compose {   // _M_fn1(_M_fn2(x), _M_fn3(x))
protected:
    Op1 _M_fn1;
    Op2 _M_fn2;
    Op3 _M_fn3;
public:
    __forceinline bool operator()(const CThing& x) const { return _M_fn1(_M_fn2(x), _M_fn3(x)); }
};
}
struct CThingFilter_In2DRange {                    // Ego_r.pdb: Pos, Range
    C3DVector Pos;
    float     Range;
    bool operator()(const CThing& thing) const;       // 0x006A76B0
};
template <class A, class B> struct CFilter_Bind2 {     // both filters, held by reference
    const A& First;
    const B& Second;
    template <class T> __forceinline bool operator()(const T& thing) const { return First(thing) && Second(thing); }
};
struct CThingCompare_Nearest {                       // out-of-line member template per thing type
    C3DVector Pos;
    template <class T> T* operator()(T* best, T* candidate) const;
};

struct CThingSearchToolsMethods : CThingSearchTools {
    CThingList* PeekAllThings();                      // 0x0049C770: the thing manager's all-things list
    CThingList* PeekTypeList(long type);              // 0x0049C780: the per-type thing list
    template <class TFilter, class TCompare, class TThing> bool GetPBestThing(const TFilter& filter, const TCompare& compare, TThing** out);
    template <class TFilter, class TCompare, class TThing> bool GetPBestLeafThing(const TFilter& filter, const TCompare& compare, TThing** out);
};


// Best (by the comparator) scan-visible TThing of the whole thing list that passes the filter.
template <class TFilter, class TCompare, class TThing>
bool CThingSearchToolsMethods::GetPBestThing(const TFilter& filter, const TCompare& compare, TThing** out)
{
    TThing* best = 0;
    CThingList* things = PeekAllThings();
    for (CThingList::iterator it = things->begin(); it != things->end(); ++it)
    {
        // `*it` is re-read for each use: VC7.1 drops a null test on a pointer it has seen dereferenced,
        // and only a fresh `*it` keeps retail's `test esi,esi` after the type test.
        if ((*it)->IsIncludedInThingScans() && IsThingOfType<TThing>(*it))
        {
            TThing* candidate = (TThing*)*it;
            if (candidate && filter(*candidate))
                best = compare(best, candidate);
        }
    }
    *out = best;
    return best != 0;
}

template bool CThingSearchToolsMethods::GetPBestThing<CFilter_Bind2<std::binary_compose<std::logical_or<bool>, CThingFilter_HasTC, CThingFilter_HasTC>, CThingFilter_In2DRange >, CThingCompare_Nearest, CThing >(const CFilter_Bind2<std::binary_compose<std::logical_or<bool>, CThingFilter_HasTC, CThingFilter_HasTC>, CThingFilter_In2DRange >&, const CThingCompare_Nearest&, CThing**);