// CVectorMap::LowerBound(const EExpressionType& key) const -- (CVectorMap<EExpressionType, CExpressionSet::CEntry >) binary search of the sorted 12-byte entries for the first entry whose key is not below `key`
// Leaf: ?LowerBound@?$CVectorMap@W4EExpressionType@@VCEntry@CExpressionSet@@V?$CKeyPairCompareLess@W4EExpressionType@@VCEntry@CExpressionSet@@@@@@QBEPBU?$pair@W4EExpressionType@@VCEntry@CExpressionSet@@@std@@ABW4EExpressionType@@@Z
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
namespace std {
template <class _T1, class _T2> struct pair {
    typedef _T1 first_type;
    typedef _T2 second_type;
    _T1 first;
    _T2 second;
};
template <class _Tp> class allocator { };
template <class _Value, class _Tp, class _MaybeReboundAlloc> struct _STLP_alloc_proxy {   // the end-of-storage pointer
    _Value _M_data;
};
template <class _Tp, class _Alloc> class vector {           // retail STLport vector (12 bytes)
public:
    typedef _Tp* iterator;
    typedef const _Tp* const_iterator;
    _Tp* _M_start;                                            // +0x00
    _Tp* _M_finish;                                           // +0x04
    _STLP_alloc_proxy<_Tp*, _Tp, _Alloc> _M_end_of_storage;   // +0x08
    iterator begin() { return this->_M_start; }
    iterator end() { return this->_M_finish; }
    const_iterator begin() const { return this->_M_start; }
    const_iterator end() const { return this->_M_finish; }
};
}   // namespace std

template <class T> class CArray : public std::vector<T, std::allocator<T> > { };   // Fable's CArray over the STLport vector

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