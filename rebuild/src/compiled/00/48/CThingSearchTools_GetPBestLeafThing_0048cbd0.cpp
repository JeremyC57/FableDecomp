// CThingSearchTools::GetPBestLeafThing(const TFilter& filter, const TCompare& compare, TThing** out) [TFilter = std::binder2nd<std::const_mem_fun1_ref_t<bool, CThing, ETCInterfaceType> >, TCompare = CThingCompare_Nearest, TThing = const CThingVillage] -- the twin of 0x0048CB50 for the const overload of CPlayer::GetPNearestTCVillage (0x004886F0)
// Leaf: GetPBestLeafThing
// Retail: Fable.exe (TLC, VC7.1) @ 0x0048CBD0, 118 bytes (`ret 0Ch`).
// Callees: PeekTypeList 0x0049C780, the bound member through its pointer, CThingCompare_Nearest::operator()<T> 0x0048B220.
// Identity: 0x0048CB50 and this body are byte-identical but call different comparator instances (0x0048B160 /
// 0x0048B220) and are called by the two CPlayer::GetPNearestTCVillage copies 0x00488620 / 0x004886F0 (the second
// uses a different LowerBound instance, 0x004365B0): two instantiations of one template in one TU. The thing
// type of the second is not proven; it is modelled as `const CThingVillage`.
// Model: CThingSearchTools from engine/CThingSearchTools.h (`this` is only passed through to the list getter);
// CThing is a local retail-layout model; the filters and comparators are local models (Ego_r.pdb members),
// called out of line with the thing by reference; the CFilter_BindN / CFilter_Not hold their filters by reference.
// Shape: the template body of the landed siblings (0x00936950 GetPBestLeafThing, GetPBestThing re-reading `*it`).
#include "engine/CThingSearchTools.h"

// ---- CThing (retail layout; Ego_r.pdb offsets in brackets) -------------------------------------
// Retail CThing sits 0x10 below the PDB from ThingTypeFlags on: ThingTypeFlags +0x6c [PDB +0x7c], the state
// bit byte +0x91 [PDB +0xa1] = Dead 0x01, IncludeInThingScans 0x04, Activated 0x08, InLimbo 0x20.
class CThing { public:
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

class CThingVillage : public CThing { public: enum { ThingListType = 4 }; };
class C3DVector { public: float X, Y, Z; };
namespace std {
template <class R, class T, class A> class const_mem_fun1_ref_t {   // (thing.*pmf)(arg)
    R (T::*_M_f)(A) const;
public:
    R operator()(const T& r, A a) const { return (r.*_M_f)(a); }
};
template <class Op> class binder2nd;
template <class R, class T, class A> class binder2nd<const_mem_fun1_ref_t<R, T, A> > {   // op(x, value)
protected:
    const_mem_fun1_ref_t<R, T, A> op;
    A value;
public:
    R operator()(const T& x) const { return op(x, value); }
};
}
class CThingCompare_Nearest { public:                       // out-of-line member template per thing type
    C3DVector Pos;
    template <class T> T* operator()(T* best, T* candidate) const;
};

struct CThingSearchToolsMethods : CThingSearchTools {
    CThingList* PeekAllThings();                      // 0x0049C770: the thing manager's all-things list
    CThingList* PeekTypeList(long type);              // 0x0049C780: the per-type thing list
    template <class TFilter, class TCompare, class TThing> bool GetPBestThing(const TFilter& filter, const TCompare& compare, TThing** out);
    template <class TFilter, class TCompare, class TThing> bool GetPBestLeafThing(const TFilter& filter, const TCompare& compare, TThing** out);
};


// Best scan-visible thing of TThing's own type list that passes the filter.
template <class TFilter, class TCompare, class TThing>
bool CThingSearchToolsMethods::GetPBestLeafThing(const TFilter& filter, const TCompare& compare, TThing** out)
{
    TThing* best = 0;
    CThingList* things = PeekTypeList(TThing::ThingListType);
    for (CThingList::iterator it = things->begin(); it != things->end(); ++it)
    {
        CThing* thing = *it;
        if (thing->IsIncludedInThingScans() && filter(*(TThing*)thing))
            best = compare(best, (TThing*)thing);
    }
    *out = best;
    return best != 0;
}

template bool CThingSearchToolsMethods::GetPBestLeafThing<std::binder2nd<std::const_mem_fun1_ref_t<bool, CThing, long> >, CThingCompare_Nearest, const CThingVillage >(const std::binder2nd<std::const_mem_fun1_ref_t<bool, CThing, long> >&, const CThingCompare_Nearest&, const CThingVillage**);