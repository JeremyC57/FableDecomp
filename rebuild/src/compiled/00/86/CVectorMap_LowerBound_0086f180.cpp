// CVectorMap::LowerBound(const EExpressionType& key) const -- (CVectorMap<EExpressionType, CExpressionSet::CEntry >) binary search of the sorted 12-byte entries for the first entry whose key is not below `key`
// Leaf: ?LowerBound@?$CVectorMap@W4EExpressionType@@VCEntry@CExpressionSet@@V?$CKeyPairCompareLess@W4EExpressionType@@VCEntry@CExpressionSet@@@@@@QBEPBU?$pair@W4EExpressionType@@VCEntry@CExpressionSet@@@std@@ABW4EExpressionType@@@Z
// Plan phase 3 (2026-11-04), CArray wave: the local CArray (an empty public std::vector<T>) and its STLport stand-ins replaced by the shared engine/CArray.h (Lionhead's array: a public STLport 4.6 vector<T>, 12 bytes, no fields of its own). STLport algorithm / construct helpers and placement new come from the header's model (<new>).
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0086F180, 82 bytes (+ int3 padding to 96), thiscall `ret 4`; the STLport
//   iterator is a plain pointer returned in eax.
// Identity (proven): CVectorMap<EExpressionType, CExpressionSet::CEntry, CKeyPairCompareLess<...> >::LowerBound(const EExpressionType&) const; evidence: its only caller 0x0086F510 is CExpressionSet::GetRandomExpression(EExpressionType, unsigned long&) const (Ego_r.pdb ?GetRandomExpression@CExpressionSet@@QBEJW4EExpressionType@@AAK@Z: LowerBound and UpperBound of the type, one generator step, the entry's AnimIndex at value +4 returned; CTCCreatureExpression::SetExpression reaches it through the def's CCountedPointer<CExpressionSet>), whose only member is Expressions = CVectorMap<EExpressionType, CExpressionSet::CEntry, ...> (Ego_r.pdb; CEntry {EExpressionType Type, long AnimIndex}, 8 bytes) -- a const caller, so the const overload, the only one FableWin.pdb names for this map.
// Label: the manifest's "LowerBound" is propagated (family J006: six byte-identical bodies; retail has no identical-code
//   folding, so each address is one instantiation and one const-ness: both PDBs name a non-const LowerBound returning an
//   iterator and a const one returning a const_iterator, and the two compile to the same bytes).
// Body: count = (last - first) / 12; while (count > 0) { half = count / 2; if (first[half].first < key) { first += half + 1;
//   count -= half + 1; } else count = half; } return first; -- the key read once before the loop, `count -= half + 1` as
//   `count += -1 - half`, the signed `cdq/sub/sar` halving.
// Layout (retail CVectorMap 0x10): CArray<pair<K, V> > Data +0 (STLport vector: _M_start, _M_finish, _M_end_of_storage),
//   the empty comparator +0xC, Dirty +0xD.
// Model: the landed 0x00447C80 (masked-identical body).
enum EExpressionType { EExpressionType_Force32 = 0x7fffffff };   // enumerators not needed
class CExpressionSet {
public:
    class CEntry {                                            // Ego_r.pdb, 8 bytes
    public:
        EExpressionType Type;                                 // +0x0
        long AnimIndex;                                       // +0x4
    };
};
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

template class CVectorMap<EExpressionType, CExpressionSet::CEntry, CKeyPairCompareLess<EExpressionType, CExpressionSet::CEntry > >;