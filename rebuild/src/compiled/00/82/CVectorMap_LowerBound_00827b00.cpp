// CVectorMap::LowerBound(const long& key) -- (CVectorMap<long, CCountedPointer<CRegionConnectionNode> >) binary search of the sorted 12-byte entries for the first entry whose key is not below `key`
// Leaf: ?LowerBound@?$CVectorMap@JV?$CCountedPointer@VCRegionConnectionNode@@@@V?$CKeyPairCompareLess@JV?$CCountedPointer@VCRegionConnectionNode@@@@@@@@QAEPAU?$pair@JV?$CCountedPointer@VCRegionConnectionNode@@@@@std@@ABJ@Z
// Plan phase 5 (2026-11-05), CCountedPointer wave: the local CCountedPointer / count-block stand-ins replaced by the shared engine/CCountedPointer.h (Ego_r.pdb: CCountedPointer<T> Data +0x0 / Info +0x4; CCPPointerInfo RefCount +0x0 / DeleteFunc +0x4 / Data +0x8).
// Plan phase 3 (2026-11-04), CArray wave: the local CArray (an empty public std::vector<T>) and its STLport stand-ins replaced by the shared engine/CArray.h (Lionhead's array: a public STLport 4.6 vector<T>, 12 bytes, no fields of its own). STLport algorithm / construct helpers and placement new come from the header's model (<new>).
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00827B00, 82 bytes (+ int3 padding to 96), thiscall `ret 4`; the STLport
//   iterator is a plain pointer returned in eax.
// Identity (proven): CVectorMap<long, CCountedPointer<CRegionConnectionNode>, CKeyPairCompareLess<...> >::LowerBound(const long&); evidence: CRegionConnectionGraph::GraphNodes (Ego_r.pdb CVectorMap<long, CCountedPointer<CRegionConnectionNode>, ...> at +0); called by CRegionConnectionGraph::SetRegionsAsConnected 0x008286A0 (0x008286AB, 0x008286DE; Ego_r.pdb ?SetRegionsAsConnected@CRegionConnectionGraph@@QAEXJJ_N@Z, non-const) -- the non-const overload; its byte twin 0x00827B60 serves the const callers CRegionConnectionNode::GetNextNodeOnWayToRegion (QBE, const graph) and CRegionConnectionGraph::GetNextRegionOnWayToRegion (QBE). FableWin.pdb names both overloads.
// Label: the manifest's "LowerBound" is propagated (family J006: six byte-identical bodies; retail has no identical-code
//   folding, so each address is one instantiation and one const-ness: both PDBs name a non-const LowerBound returning an
//   iterator and a const one returning a const_iterator, and the two compile to the same bytes).
// Body: count = (last - first) / 12; while (count > 0) { half = count / 2; if (first[half].first < key) { first += half + 1;
//   count -= half + 1; } else count = half; } return first; -- the key read once before the loop, `count -= half + 1` as
//   `count += -1 - half`, the signed `cdq/sub/sar` halving.
// Layout (retail CVectorMap 0x10): CArray<pair<K, V> > Data +0 (STLport vector: _M_start, _M_finish, _M_end_of_storage),
//   the empty comparator +0xC, Dirty +0xD.
// Model: the landed 0x00447C80 (masked-identical body).
#include "engine/CCountedPointer.h"
class CRegionConnectionNode;
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
    iterator LowerBound(const K& key);
};

template <class K, class V, class TCompare>
typename CVectorMap<K, V, TCompare>::iterator CVectorMap<K, V, TCompare>::LowerBound(const K& key)
{
    iterator first = Data.begin();
    int count = Data.end() - Data.begin();
    while (count > 0)
    {
        int half = count / 2;
        iterator middle = first + half;
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

template class CVectorMap<long, CCountedPointer<CRegionConnectionNode>, CKeyPairCompareLess<long, CCountedPointer<CRegionConnectionNode> > >;