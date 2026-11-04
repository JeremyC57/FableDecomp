// CVectorMap::LowerBound(const long& key) -- find the first entry whose key is at least key.
// Leaf: ?LowerBound@?$CVectorMap@JV?$CIntelligentPointer@VCThing@@@@V?$CKeyPairCompareLess@JV?$CIntelligentPointer@VCThing@@@@@@@@QBEPBU?$pair@JV?$CIntelligentPointer@VCThing@@@@@std@@ABJ@Z
// Plan phase 3 (2026-11-04), CArray wave: the local CArray (an empty public std::vector<T>) and its STLport stand-ins replaced by the shared engine/CArray.h (Lionhead's array: a public STLport 4.6 vector<T>, 12 bytes, no fields of its own). STLport algorithm / construct helpers and placement new come from the header's model (<new>).
// Identity: proven; CTCCarrying::CarrySlots; const HasCarrySlot/GetThingInCarrySlot/IsCarrySlotFree callers; J006 twin0x6A29B0; stride12.
class CThing;
class CBaseObjectPointer;
class CBaseIntelligentPointer {                               // Ego_r.pdb, 8 bytes (virtual destructor)
public:
    virtual ~CBaseIntelligentPointer();
    CBaseObjectPointer* PData;                                // +0x4
};
template <class T> class CIntelligentPointer : public CBaseIntelligentPointer { };
#include "engine/CArray.h"


template <class K, class V> class CKeyPairCompareLess {
public:
    bool operator()(const std::pair<K, V>& entry, const K& key) const { return entry.first < key; }
};

template <class K, class V, class TCompare> class CVectorMap {   // sorted-on-demand vector map, retail 0x10 bytes
public:
    typedef std::pair<K, V> value_type;
    typedef typename CArray<value_type>::iterator iterator;
    typedef typename CArray<value_type>::const_iterator const_iterator;
    CArray<value_type> Data;                                  // +0x00
    TCompare           Compare;                               // +0x0C (empty comparator)
    bool               Dirty;                                 // +0x0D
    const_iterator LowerBound(const K& key) const;
};

template <class K, class V, class TCompare>
typename CVectorMap<K, V, TCompare>::const_iterator CVectorMap<K, V, TCompare>::LowerBound(const K& key) const
{
    const_iterator first = Data.begin();
    int count = Data.end() - Data.begin();
    while (count > 0)
    {
        int half = count / 2;
        const_iterator middle = first + half;
        if (Compare(*middle, key))
        {
            first = middle + 1;
            count -= half + 1;
        }
        else
            count = half;
    }
    return first;
}

template class CVectorMap<long, CIntelligentPointer<CThing>, CKeyPairCompareLess<long, CIntelligentPointer<CThing> > >;