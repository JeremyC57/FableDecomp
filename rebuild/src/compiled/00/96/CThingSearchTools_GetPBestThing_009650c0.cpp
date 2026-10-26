// CThingSearchTools::GetPBestThing(const TFilter& filter, const TCompare& compare, TThing** out) [TFilter = CFilter_Bind4<CThingFilter_ObjectHasProperty, CThingFilter_HasFreeActionPoint, CThingFilter_HasTC, std::unary_negate<CThingFilter_In3DRange> >, TCompare = CThingCompare_Nearest, TThing = CThingObject] -- the nearest scan-visible object (mask 0x807) with the property, a free action point and the component, outside the 3D range
// Leaf: GetPBestThing
// Retail: Fable.exe (TLC, VC7.1) @ 0x009650C0, 178 bytes (+ int3 padding to 192) (`ret 0Ch`).
// Callees: PeekAllThings 0x0049C770, CThingFilter_ObjectHasProperty 0x006A7590, CThingFilter_HasFreeActionPoint 0x006A8B40, CThingFilter_HasTC 0x006A80A0, CThingFilter_In3DRange 0x006A7780 (negated), CThingCompare_Nearest::operator()<CThingObject> 0x00916B10.
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

class CThingGameObject : public CThing { public: enum { ThingTypeMask = 0x7 }; };
class CThingObject : public CThingGameObject { public: enum { ThingTypeMask = 0x807 }; };
class C3DVector { public: float X, Y, Z; };
class CThingFilter_ObjectHasProperty { public:
    long Property;                                    // EObjectProperty
    bool operator()(const CThing& thing) const;       // 0x006A7590
};
class CThingFilter_HasFreeActionPoint { public:
    bool operator()(const CThing& thing) const;       // 0x006A8B40
};
class CThingFilter_HasTC { public:
    long Type;                                        // ETCInterfaceType
    bool operator()(const CThing& thing) const;       // 0x006A80A0
};
class CThingFilter_In3DRange { public:                    // Ego_r.pdb: Pos, Range
    C3DVector Pos;
    float     Range;
    bool operator()(const CThing& thing) const;       // 0x006A7780
};
namespace std {
template <class P> class unary_negate {               // !pred(x)
protected:
    P _M_pred;
public:
    template <class T> bool operator()(const T& x) const { return !_M_pred(x); }
};
}
template <class A, class B, class C, class D> class CFilter_Bind4 { public:     // every filter, held by reference
    const A& First;
    const B& Second;
    const C& Third;
    const D& Fourth;
    template <class T> __forceinline bool operator()(const T& thing) const { return First(thing) && Second(thing) && Third(thing) && Fourth(thing); }
};
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

template bool CThingSearchToolsMethods::GetPBestThing<CFilter_Bind4<CThingFilter_ObjectHasProperty, CThingFilter_HasFreeActionPoint, CThingFilter_HasTC, std::unary_negate<CThingFilter_In3DRange> >, CThingCompare_Nearest, CThingObject >(const CFilter_Bind4<CThingFilter_ObjectHasProperty, CThingFilter_HasFreeActionPoint, CThingFilter_HasTC, std::unary_negate<CThingFilter_In3DRange> >&, const CThingCompare_Nearest&, CThingObject**);