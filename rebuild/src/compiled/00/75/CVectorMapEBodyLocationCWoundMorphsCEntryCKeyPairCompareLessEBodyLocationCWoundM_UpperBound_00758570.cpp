// CVectorMap<EBodyLocation,CWoundMorphs::CEntry,CKeyPairCompareLess<EBodyLocation,CWoundMorphs::CEntry> >::UpperBound(const EBodyLocation& key) -- binary search of the sorted 44-byte entries for the first entry whose key is above the key
// Retail: Fable.exe (TLC, VC7.1) @ 0x00758570, 84 bytes, `__fastcall`, `ret 4`, the entry pointer (STLport iterator) in eax.
// Instance: CWoundMorphs' EBodyLocation -> CEntry map (the UpperBound beside LowerBound 0x00758470); the label's value type is a propagated hint, the entry stride (44 bytes) is what the body proves,
//   so the value is modelled as an opaque 40-byte payload unless it is a pointer.
// Body: the inline Dinkumware-style bound (as the landed CVectorMap<long,CIntelligentPointer<CThing>>::LowerBound 0x00447C80):
//   count = (last - first) / sizeof(entry); while (count > 0) { half = count / 2; compare the middle key; step past it
//   (`count -= half + 1`, emitted `or -1 ; sub ; add`) or keep the lower half }. The signed key compare inlines
//   (`jge` for `entry < key`, `jl` for the upper bound's `key < entry`) and the key is read once before the loop.
// Layout: STLport vector {first, last, end_of_storage}; entry pair<key, value>, the 4-byte key at +0.
enum EBodyLocation { EBodyLocation_FIRST };
struct SValue {
    unsigned char _payload[40];                      // opaque CWoundMorphs::CEntry
};
namespace std {
template <class A, class B> struct pair {
    A first;
    B second;
};
}
template <class K, class V> struct CKeyPairCompareLess {
    bool operator()(const std::pair<K, V>& entry, const K& key) const { return entry.first < key; }
    bool operator()(const K& key, const std::pair<K, V>& entry) const { return key < entry.first; }
};
template <class K, class V, class C> class CVectorMap {
public:
    typedef std::pair<K, V> value_type;
    value_type* First;                                   // +0x00
    value_type* Last;                                    // +0x04
    value_type* EndOfStorage;                            // +0x08
    value_type* UpperBound(const K& key);
};
template <class K, class V, class C>
typename CVectorMap<K, V, C>::value_type* CVectorMap<K, V, C>::UpperBound(const K& key)
{
    value_type* first = First;
    int count = Last - First;
    C comp;
    while (count > 0)
    {
        int half = count / 2;
        value_type* middle = first + half;
        if (!comp(key, *middle))
        {
            first = middle + 1;
            count -= half + 1;
        }
        else
            count = half;
    }
    return first;
}
template class CVectorMap<EBodyLocation, SValue, CKeyPairCompareLess<EBodyLocation, SValue> >;