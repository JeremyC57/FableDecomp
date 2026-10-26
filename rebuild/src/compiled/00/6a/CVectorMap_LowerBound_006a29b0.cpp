// CVectorMap::LowerBound(const long& key) -- (CVectorMap<long, CIntelligentPointer<CThing> >) binary search of the sorted 12-byte entries for the first entry whose key is not below `key`
// Leaf: ?LowerBound@?$CVectorMap@JV?$CIntelligentPointer@VCThing@@@@V?$CKeyPairCompareLess@JV?$CIntelligentPointer@VCThing@@@@@@@@QAEPAU?$pair@JV?$CIntelligentPointer@VCThing@@@@@std@@ABJ@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x006A29B0, 82 bytes (+ int3 padding to 96), thiscall `ret 4`; the STLport
//   iterator is a plain pointer returned in eax.
// Identity (proven): CVectorMap<long, CIntelligentPointer<CThing>, CKeyPairCompareLess<...> >::LowerBound(const long&); evidence: CTCCarrying::CarrySlots (Ego_r.pdb CVectorMap<long, CIntelligentPointer<CThing>, ...> at +0xC); its callers are CTCCarrying::RemoveThingInCarrySlot 0x006A16A0 (0x006A16ED) and PutThingInCarrySlot 0x006A1F80 (0x006A1FD7), both non-const in Ego_r.pdb (?RemoveThingInCarrySlot@CTCCarrying@@QAEPAVCThing@@J_N@Z, ?PutThingInCarrySlot@CTCCarrying@@QAEXAAVCThing@@J_N@Z), so the non-const overload; its byte twin 0x006A2A10 is the const one (callers HasCarrySlot / GetThingInCarrySlot / IsCarrySlotFree, all QBE). FableWin.pdb names both overloads.
// Label: the manifest's "LowerBound" is propagated (family J006: six byte-identical bodies; retail has no identical-code
//   folding, so each address is one instantiation and one const-ness: both PDBs name a non-const LowerBound returning an
//   iterator and a const one returning a const_iterator, and the two compile to the same bytes).
// Body: count = (last - first) / 12; while (count > 0) { half = count / 2; if (first[half].first < key) { first += half + 1;
//   count -= half + 1; } else count = half; } return first; -- the key read once before the loop, `count -= half + 1` as
//   `count += -1 - half`, the signed `cdq/sub/sar` halving.
// Layout (retail CVectorMap 0x10): CArray<pair<K, V> > Data +0 (STLport vector: _M_start, _M_finish, _M_end_of_storage),
//   the empty comparator +0xC, Dirty +0xD.
// Model: the landed 0x00447C80 (masked-identical body).
class CThing;
class CBaseObjectPointer;
class CBaseIntelligentPointer {                               // Ego_r.pdb, 8 bytes (virtual destructor)
public:
    virtual ~CBaseIntelligentPointer();
    CBaseObjectPointer* PData;                                // +0x4
};
template <class T> class CIntelligentPointer : public CBaseIntelligentPointer { };
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

template class CVectorMap<long, CIntelligentPointer<CThing>, CKeyPairCompareLess<long, CIntelligentPointer<CThing> > >;