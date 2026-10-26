// CThingSearchTools::GetPBestThing<CFilter_Bind2<CThingFilter_IsNot, CThingFilter_IsChild >, CThingCompare_Nearest, CThingAICreature>(const CFilter_Bind2<CThingFilter_IsNot, CThingFilter_IsChild >&, const CThingCompare_Nearest&, CThingAICreature**) -- nearest scan-visible AI creature that is not the excluded thing and is a child
// Leaf: GetPBestThing
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00914B20, length 160 bytes (`ret 0Ch`).
// Callees: PeekAllThings() 0x0049C770, CThingFilter_IsNot::operator() 0x006A76A0, CThingCompare_Nearest::operator()<CThingAICreature> 0x0065DA97.
// Model: CThingSearchTools from engine/CThingSearchTools.h (this is only passed through to the list
// getter); CThing is a local retail-layout model (see below; retail sits 0x10 below Ego_r.pdb from
// ThingTypeFlags on). The thing type is selected by its THING_TYPE mask (TThing::ThingTypeMask).
// CThingFilter_IsChild is inline: the creature category getter 0x00662000 compared with 1.
#include "engine/CThingSearchTools.h"

// ---- CThing (retail layout; Ego_r.pdb offsets in brackets) -------------------------------------
// Retail CThing sits 0x10 below the PDB from ThingTypeFlags on (the PDB bitset<274> and vector map
// carry debug padding): UsingComponents +0x20 [PDB +0x20], ThingTypeFlags +0x6c [PDB +0x7c],
// the state bit byte +0x91 [PDB +0xa1] = Dead 0x01, IncludeInThingScans 0x04, Activated 0x08, InLimbo 0x20.
class CThing { public:
    enum { ThingTypeMask = 0 };                       // every thing matches the CThing type mask
    unsigned char _pad_0x00[0x20];
    unsigned long UsingComponents[9];                 // +0x20 bitset of present thing components
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

class CThingGameObject : public CThing { public:
    enum { ThingTypeMask = 0x7 };
};
class CThingCreatureBase : public CThingGameObject { public:
    enum { ThingTypeMask = 0xf };
    long GetCreatureCategory() const;               // 0x00662000: def (+0xe0)->+0xc4
};
class CThingAICreature : public CThingCreatureBase { public:
    enum { ThingTypeMask = 0x1f };
};

class CThingFilter_IsNot { public:
    const CThing* Excluded;
    bool operator()(const CThing* thing) const;      // 0x006A76A0
};
class CThingFilter_IsChild { public:                         // inline: the creature's category getter == 1
    bool operator()(const CThingCreatureBase* creature) const { return creature->GetCreatureCategory() == 1; }
};
template <class A, class B> class CFilter_Bind2 { public:     // both filters, held by reference
    const A& First;
    const B& Second;
    CFilter_Bind2(const A& a, const B& b) : First(a), Second(b) {}
    template <class T> bool operator()(T* thing) const { return First(thing) && Second(thing); }
};
template <class A, class B, class C> class CFilter_Bind3 { public:
    const A& First;
    const B& Second;
    const C& Third;
    CFilter_Bind3(const A& a, const B& b, const C& c) : First(a), Second(b), Third(c) {}
    template <class T> bool operator()(T* thing) const { return First(thing) && Second(thing) && Third(thing); }
};
class CThingCompare_Nearest { public:                       // out-of-line member template per thing type
    float X, Y, Z;
    template <class T> T* operator()(T* best, T* candidate) const;
};

struct CThingSearchToolsMethods : CThingSearchTools {
    CThingList* PeekAllThings();                      // 0x0049C770: &ThingManager->(all-things list at +0x18)
    CThingList* PeekTypeList(long type);              // 0x0049C780: the per-type thing list
    template <class TFilter, class TThing> bool GetPFirstThing(const TFilter& filter, TThing** out);
    template <class TFilter, class TThing> bool GetPFirstLeafThing(const TFilter& filter, TThing** out);
    template <class TFilter, class TCompare, class TThing> bool GetPBestThing(const TFilter& filter, const TCompare& compare, TThing** out);
    template <class TFilter, class TCompare, class TThing> bool GetPBestLeafThing(const TFilter& filter, const TCompare& compare, TThing** out);
};

// First thing of the whole thing list that is of type TThing, scan-visible and passes the filter.
template <class TFilter, class TThing>
bool CThingSearchToolsMethods::GetPFirstThing(const TFilter& filter, TThing** out)
{
    CThingList* things = PeekAllThings();
    for (CThingList::iterator it = things->begin(); it != things->end(); ++it)
    {
        CThing* thing = *it;
        if (IsThingOfType<TThing>(thing) && thing->IsIncludedInThingScans() && filter((TThing*)thing))
        {
            *out = (TThing*)thing;
            return true;
        }
    }
    return false;
}

// First scan-visible thing of TThing's own type list that passes the filter.
template <class TFilter, class TThing>
bool CThingSearchToolsMethods::GetPFirstLeafThing(const TFilter& filter, TThing** out)
{
    CThingList* things = PeekTypeList(TThing::ThingListType);
    for (CThingList::iterator it = things->begin(); it != things->end(); ++it)
    {
        CThing* thing = *it;
        if (thing->IsIncludedInThingScans() && filter((TThing*)thing))
        {
            *out = (TThing*)thing;
            return true;
        }
    }
    return false;
}

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
            if (candidate && filter(candidate))
                best = compare(best, candidate);
        }
    }
    *out = best;
    return best != 0;
}

// Best scan-visible thing of TThing's own type list that passes the filter.
template <class TFilter, class TCompare, class TThing>
bool CThingSearchToolsMethods::GetPBestLeafThing(const TFilter& filter, const TCompare& compare, TThing** out)
{
    TThing* best = 0;
    CThingList* things = PeekTypeList(TThing::ThingListType);
    for (CThingList::iterator it = things->begin(); it != things->end(); ++it)
    {
        CThing* thing = *it;
        if (thing->IsIncludedInThingScans() && filter((TThing*)thing))
            best = compare(best, (TThing*)thing);
    }
    *out = best;
    return best != 0;
}

template bool CThingSearchToolsMethods::GetPBestThing<CFilter_Bind2<CThingFilter_IsNot, CThingFilter_IsChild >, CThingCompare_Nearest, CThingAICreature >(const CFilter_Bind2<CThingFilter_IsNot, CThingFilter_IsChild >&, const CThingCompare_Nearest&, CThingAICreature**);