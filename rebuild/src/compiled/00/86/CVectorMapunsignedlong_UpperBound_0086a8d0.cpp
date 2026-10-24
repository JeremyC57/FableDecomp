// CVectorMap<unsigned_long::UpperBound(const unsigned long& key) const -- binary search of the sorted 12-byte entries for the first entry whose key is above `key`
// Leaf: UpperBound
// Retail: Fable.exe @ 0x0086A8D0, `__fastcall`, `ret 4`, no callees. Instance CVectorMap<unsigned long, SValue8, CKeyPairCompareLess<...> >
// (manifest label propagated across byte-identical bodies; the instance is pinned only by the 12-byte entry stride).
// Body: the STLport lower/upper_bound len/half walk: count = Last - First (the `0x2AAAAAAB` multiply); while (count > 0) { half = count / 2;
// middle = first + half; if (!(key < middle->first)) { first = middle + 1; count -= half + 1; } else count = half; } return first.
// Layout: STLport vector {First, Last, EndOfStorage} at +0 (the empty comparator follows it in the real CVectorMap); the
// iterator is a plain entry pointer returned in eax. Key compare is unsigned `jae`/`jb`; the key is read once before the loop.
// Shape: `count -= half + 1` comes out as `or reg,-1 ; sub ; add` (count += -1 - half).
template <class A, class B> struct pair {
    A first;
    B second;
};
struct SValue8 {                  // 8-byte mapped value (the propagated CAnimationEntry label is 16 bytes and does not fit the 12-byte stride)
    long m_a;
    long m_b;
};
template <class K, class V> struct CKeyPairCompareLess {
    bool operator()(const pair<K, V>& entry, const K& key) const { return entry.first < key; }
    bool operator()(const K& key, const pair<K, V>& entry) const { return key < entry.first; }
};
template <class K, class V, class C> class CVectorMap {
public:
    typedef pair<K, V> value_type;
    value_type* First;                                   // +0x00
    value_type* Last;                                    // +0x04
    value_type* EndOfStorage;                            // +0x08
    value_type* UpperBound(const K& key) const;
};
template <class K, class V, class C>
typename CVectorMap<K, V, C>::value_type* CVectorMap<K, V, C>::UpperBound(const K& key) const
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
template class CVectorMap<unsigned long, SValue8, CKeyPairCompareLess<unsigned long, SValue8> >;