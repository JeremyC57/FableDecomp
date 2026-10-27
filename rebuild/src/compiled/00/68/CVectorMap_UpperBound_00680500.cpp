// CVectorMap::UpperBound(const EReactionSpeechType& key) const -- (CVectorMap<EReactionSpeechType, long, CKeyPairCompareLess<EReactionSpeechType, long> >) binary search of the sorted 8-byte entries for the first entry whose key is above the key
// Leaf: ?UpperBound@?$CVectorMap@W4EReactionSpeechType@@JV?$CKeyPairCompareLess@W4EReactionSpeechType@@J@@@@QBEPBU?$pair@W4EReactionSpeechType@@J@std@@ABW4EReactionSpeechType@@@Z
// Retail: Fable.exe @ 0x00680500, 60 bytes, thiscall `ret 4`, the entry pointer (STLport const iterator) in eax.
// Identity (proven instance): its two callers are CSimVoice members on the map at CSimVoice +0x08 -- Ego_r.pdb
// CSimVoice::Voices, CVectorMap<EReactionSpeechType, long, CKeyPairCompareLess<EReactionSpeechType, long> > -- among them
// 0x006807F0 (`UpperBound(key) - LowerBound(key)` in entries: Ego_r.pdb CSimVoice::GetNoOfSampleIndexVariations(
// EReactionSpeechType) const); Ego_r.pdb and FableWin.pdb both hold ?UpperBound@?$CVectorMap@W4EReactionSpeechType@@J...QBE
// (const; the Dinkumware _Vector_const_iterator is STLport's plain const pointer in retail).
// Label correction: the landed partner 0x006804C0 ("CVectorMap<ETCInterfaceType, CTCBase*, ...>::LowerBound") is this map's
// LowerBound: all six of its callers (0x0068080A .. 0x00680AF9) are CSimVoice members on the same +0x08 map, and the CThing
// component map's LowerBound is 0x0040F020 (const 0x004365B0).
// Callees: none.
// Shape: the landed LowerBound body with the comparator reversed (`!(key < entry.first)` steps past the middle, the `else` keeps the lower half: `jl`); the key
// read once before the loop; `count -= half + 1` as `or -1 ; sub ; add`.
// Layout: STLport vector {first, last, end_of_storage} at the map's +0x00; entry pair<EReactionSpeechType, long>.
enum EReactionSpeechType { EReactionSpeechType_FORCE_DWORD = 0x7fffffff };
namespace std {
template <class A, class B> struct pair {
    A first;
    B second;
};
}
template <class K, class V> class CKeyPairCompareLess {
public:
    bool operator()(const std::pair<K, V>& entry, const K& key) const { return entry.first < key; }
    bool operator()(const K& key, const std::pair<K, V>& entry) const { return key < entry.first; }
};
template <class K, class V, class C> class CVectorMap {
public:
    typedef std::pair<K, V> value_type;
    value_type* First;                                      // +0x00
    value_type* Last;                                       // +0x04
    value_type* EndOfStorage;                               // +0x08
    const value_type* UpperBound(const K& key) const;
};
template <class K, class V, class C>
const typename CVectorMap<K, V, C>::value_type* CVectorMap<K, V, C>::UpperBound(const K& key) const
{
    const value_type* first = First;
    int count = Last - First;
    C comp;
    while (count > 0) {
        int half = count / 2;
        const value_type* middle = first + half;
        if (!comp(key, *middle)) {
            first = middle + 1;
            count -= half + 1;
        }
        else
            count = half;
    }
    return first;
}
template class CVectorMap<EReactionSpeechType, long, CKeyPairCompareLess<EReactionSpeechType, long> >;