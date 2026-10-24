// CVectorMap<long,CRandomAppearanceMorph::CSkeletalMorph,CKeyPairCompareLess<long,CRandomAppearanceMorph::CSkeletalMorph> >::LowerBound(const long& key) -- binary search of the sorted 16-byte entries for the first entry whose key is not below the key
// Retail: Fable.exe (TLC, VC7.1) @ 0x0086A930, 80 bytes, `__fastcall`, `ret 4`, the entry pointer (STLport iterator) in eax.
// Instance: a long-keyed 16-byte-entry map (manifest: CRandomAppearanceMorph::CSkeletalMorph); the label's value type is a propagated hint, the entry stride (16 bytes) is what the body proves,
//   so the value is modelled as an opaque 12-byte payload unless it is a pointer.
// Body: the inline Dinkumware-style bound (as the landed CVectorMap<long,CIntelligentPointer<CThing>>::LowerBound 0x00447C80):
//   count = (last - first) / sizeof(entry); while (count > 0) { half = count / 2; compare the middle key; step past it
//   (`count -= half + 1`, emitted `or -1 ; sub ; add`) or keep the lower half }. The signed key compare inlines
//   (`jge` for `entry < key`, `jl` for the upper bound's `key < entry`) and the key is read once before the loop.
// Layout: STLport vector {first, last, end_of_storage}; entry pair<key, value>, the 4-byte key at +0.
struct SValue {
    unsigned char _payload[12];                      // opaque CRandomAppearanceMorph::CSkeletalMorph
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
    value_type* LowerBound(const K& key);
};
template <class K, class V, class C>
typename CVectorMap<K, V, C>::value_type* CVectorMap<K, V, C>::LowerBound(const K& key)
{
    value_type* first = First;
    int count = Last - First;
    C comp;
    while (count > 0)
    {
        int half = count / 2;
        value_type* middle = first + half;
        if (comp(*middle, key))
        {
            first = middle + 1;
            count -= half + 1;
        }
        else
            count = half;
    }
    return first;
}
template class CVectorMap<long, SValue, CKeyPairCompareLess<long, SValue> >;