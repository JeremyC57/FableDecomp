// CVectorMap<long,CIntelligentPointer<CThing>,CKeyPairCompareLess<long,CIntelligentPointer<CThing> > >::LowerBound(const long& key) -- binary search of the sorted 12-byte entries for the first key not below `key`
// Leaf: ?LowerBound@?$CVectorMap@JV?$CIntelligentPointer@VCThing@@@@V?$CKeyPairCompareLess@JV?$CIntelligentPointer@VCThing@@@@@@@@QAEPAU?$pair@JV?$CIntelligentPointer@VCThing@@@@@std@@ABJ@Z
// Retail: Fable.exe @ 0x00447C80, 82 bytes, `ret 4`; callers 0x00445BEE, 0x00445C52, 0x00445CD6, 0x00446B51, 0x00446BA1,
// 0x00446D61, 0x00448169 -- these include CGamePlayerInterface's Controls map (+0x7a8, EGameAction -> CCountedPointer, also
// 12-byte entries), so this is one folded body for several 12-byte-entry CVectorMap instances. 0x00447CE0 next to it is
// the matching UpperBound (`key < entry` test), not a twin. Ego_r.pdb
// ?LowerBound@?$CVectorMap@JV?$CIntelligentPointer@VCThing@@@@...@@QAE?AV?$_Vector_iterator@...@std@@ABJ@Z (Dinkumware
// iterator there; the retail STLport iterator is a plain pointer returned in eax).
// Body: the inline lower_bound -- count = (last - first) / 12; while (count > 0) { half = count / 2; if (first[half].first
// < key) { first += half + 1; count -= half + 1; } else count = half; } return first.
// Layout: STLport vector {first, last, end_of_storage}; entry pair<long, CIntelligentPointer<CThing> > = 12 bytes (key
// +0, the 8-byte intelligent pointer +4).
// Shape: the key read once before the loop (`mov edi,[eax]`); `count -= half + 1` as `count += -1 - half`.
class CThing;
template <class T> class CIntelligentPointer {
public:
    T*   Data;
    long Handle;
};
namespace std {
template <class A, class B> struct pair {
    A first;
    B second;
};
}
template <class K, class V> class CKeyPairCompareLess { public:
    bool operator()(const std::pair<K, V>& entry, const K& key) const { return entry.first < key; }
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
template class CVectorMap<long, CIntelligentPointer<CThing>, CKeyPairCompareLess<long, CIntelligentPointer<CThing> > >;