// CThingSearchTools::GetPBestThing<CIsHopscotchBoard, CThingCompare_Nearest, CThingObject>(const CIsHopscotchBoard&, const CThingCompare_Nearest&, CThingObject**) -- nearest scan-visible object that is a hopscotch board
// Leaf: GetPBestThing
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00941450, length 148 bytes (`ret 0Ch`).
// Callees: PeekAllThings() 0x0049C770, CThingCompare_Nearest::operator()<CThingObject> 0x00916B10.
// Model: CThingSearchTools from engine/CThingSearchTools.h (this is only passed through to the list
// getter); CThing is a local retail-layout model (see below; retail sits 0x10 below Ego_r.pdb from
// ThingTypeFlags on). The thing type is selected by its THING_TYPE mask (TThing::ThingTypeMask).
// CIsHopscotchBoard is inline: virtual slot 48 (+0xc0) flag bit 15 and thing component 0x2d present
// (bit 13 of the UsingComponents word at +0x24).
#include "engine/CThingSearchTools.h"

// ---- CThing (retail layout; Ego_r.pdb offsets in brackets) -------------------------------------
// Retail CThing sits 0x10 below the PDB from ThingTypeFlags on (the PDB bitset<274> and vector map
// carry debug padding): UsingComponents +0x20 [PDB +0x20], ThingTypeFlags +0x6c [PDB +0x7c],
// the state bit byte +0x91 [PDB +0xa1] = Dead 0x01, IncludeInThingScans 0x04, Activated 0x08, InLimbo 0x20.
struct CThing {
    enum { ThingTypeMask = 0 };                       // every thing matches the CThing type mask
    virtual void VSlot00(); virtual void VSlot01(); virtual void VSlot02(); virtual void VSlot03();
    virtual void VSlot04(); virtual void VSlot05(); virtual void VSlot06(); virtual void VSlot07();
    virtual void VSlot08(); virtual void VSlot09(); virtual void VSlot10(); virtual void VSlot11();
    virtual void VSlot12(); virtual void VSlot13(); virtual void VSlot14(); virtual void VSlot15();
    virtual void VSlot16(); virtual void VSlot17(); virtual void VSlot18(); virtual void VSlot19();
    virtual void VSlot20(); virtual void VSlot21(); virtual void VSlot22(); virtual void VSlot23();
    virtual void VSlot24(); virtual void VSlot25(); virtual void VSlot26(); virtual void VSlot27();
    virtual void VSlot28(); virtual void VSlot29(); virtual void VSlot30(); virtual void VSlot31();
    virtual void VSlot32(); virtual void VSlot33(); virtual void VSlot34(); virtual void VSlot35();
    virtual void VSlot36(); virtual void VSlot37(); virtual void VSlot38(); virtual void VSlot39();
    virtual void VSlot40(); virtual void VSlot41(); virtual void VSlot42(); virtual void VSlot43();
    virtual void VSlot44(); virtual void VSlot45(); virtual void VSlot46(); virtual void VSlot47();
    virtual unsigned long GetVirtualFlags() const;    // slot 48 (+0xc0): thing flag word (bit 15 tested)
    unsigned char _pad_0x04[0x1c];
    unsigned long UsingComponents[9];                 // +0x20 bitset of present thing components
    unsigned char _pad_0x44[0x6c - 0x44];
    unsigned long ThingTypeFlags;                     // +0x6c THING_TYPE_* inheritance bits
    unsigned char _pad_0x70[0x91 - 0x70];
    unsigned char StateBits;                          // +0x91 Dead / IncludeInThingScans / Activated / InLimbo

    // CThing::IsIncludedInThingScans (out of line at 0x0048BC40), inlined here.
    bool HasTC(long type) const
    {
        if (UsingComponents[type >> 5] & (1u << (type & 31)))
            return true;
        return false;
    }
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

struct CThingGameObject : CThing {
    enum { ThingTypeMask = 0x7 };
};
struct CThingObject : CThingGameObject {
    enum { ThingTypeMask = 0x807 };
};

struct CIsHopscotchBoard {                           // inline: virtual flags bit 15 and thing component 0x2d present
    bool operator()(const CThing* thing) const
    {
        return ((unsigned char)(thing->GetVirtualFlags() >> 15) & 1) && thing->HasTC(0x2d);
    }
};
struct CThingCompare_Nearest {                       // out-of-line member template per thing type
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

template bool CThingSearchToolsMethods::GetPBestThing<CIsHopscotchBoard, CThingCompare_Nearest, CThingObject >(const CIsHopscotchBoard&, const CThingCompare_Nearest&, CThingObject**);