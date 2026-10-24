// CThingSearchTools::GetPBestLeafThing(const TFilter& filter, const TCompare& compare, TThing** out) [TFilter = CFilter_Bind2<CThingFilter_In2DRange, anonymous-namespace CThingFilter_IsValidAttackTargetForCreature>, TCompare = CThingCompare_BestCreatureToAttack, TThing = CThingAICreature] -- the best creature to attack among the scan-visible AI creatures (type list 1) in 2D range that are valid attack targets
// Leaf: GetPBestLeafThing
// Retail: Fable.exe (TLC, VC7.1) @ 0x007F7040, 135 bytes (+ int3 padding to 144) (`ret 0Ch`).
// Callees: PeekTypeList 0x0049C780, CThingFilter_In2DRange 0x006A76B0, CThingFilter_IsValidAttackTargetForCreature 0x007F6DF0, CThingCompare_BestCreatureToAttack::operator()<T> 0x007F6FD0.
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

struct CThingAICreature : CThing { enum { ThingListType = 1 }; };
struct C3DVector { float X, Y, Z; };
struct CThingFilter_In2DRange {                    // Ego_r.pdb: Pos, Range
    C3DVector Pos;
    float     Range;
    bool operator()(const CThing& thing) const;       // 0x006A76B0
};
namespace {
struct CThingFilter_IsValidAttackTargetForCreature { // Ego_r.pdb: 32 bytes of sub-filters
    unsigned char _sub[0x20];
    bool operator()(const CThing& thing) const;       // 0x007F6DF0
};
}
template <class A, class B> struct CFilter_Bind2 {     // both filters, held by reference
    const A& First;
    const B& Second;
    template <class T> __forceinline bool operator()(const T& thing) const { return First(thing) && Second(thing); }
};
struct CThingCreatureBase;
struct CThingCompare_BestCreatureToAttack {          // Ego_r.pdb: Targeter, current best, four weights
    CThingCreatureBase* Targeter;
    CThing*             PCurrentBestTargetForAttack;
    float PreferPlayerWeight, PreferNonCombatantsWeight, PreferLastAttackerWeight, PreferCurrentBestEnemyWeight;
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

template bool CThingSearchToolsMethods::GetPBestLeafThing<CFilter_Bind2<CThingFilter_In2DRange, CThingFilter_IsValidAttackTargetForCreature >, CThingCompare_BestCreatureToAttack, CThingAICreature >(const CFilter_Bind2<CThingFilter_In2DRange, CThingFilter_IsValidAttackTargetForCreature >&, const CThingCompare_BestCreatureToAttack&, CThingAICreature**);