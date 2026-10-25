// CVectorMap<unsigned long,CTCCreatureModeManager::CRegisteredModeInfo,CKeyPairCompareLess<...> >::UpperBound(const unsigned long& key) const -- binary search of the sorted 8-byte entries for the first entry whose key is above `key`
// Leaf: ?UpperBound@?$CVectorMap@KVCRegisteredModeInfo@CTCCreatureModeManager@@V?$CKeyPairCompareLess@KVCRegisteredModeInfo@CTCCreatureModeManager@@@@@@QBE
// Retail: Fable.exe @ 0x0073D250, `__fastcall`, `ret 4`, no callees. Instance CVectorMap<unsigned long, CRegisteredModeInfo, CKeyPairCompareLess<...> >
// Identity (FINDINGS chunk 392: the landed line 1 / catalog module `CVectorMap<unsigned_long` was a truncated name): the
// callers are CTCCreatureModeManager::AllocateMode 0x0073A8E0 (Ego_r.pdb `?AllocateMode@CTCCreatureModeManager@@IAE...K@Z`,
// the mode id looked up in the registered-mode table) and that map's Count 0x0073DA50 (`UpperBound(key) - LowerBound(key)`,
// chunk 392); Ego_r.pdb `?UpperBound@?$CVectorMap@KVCRegisteredModeInfo@CTCCreatureModeManager@@V?$CKeyPairCompareLess@
// KVCRegisteredModeInfo@CTCCreatureModeManager@@@@@@QBE...ABK@Z` is that instance (no identical-code folding in retail:
// one body per instance). The PDB returns a Dinkumware _Vector_const_iterator; the STLport iterator is the entry pointer.
// Body: the STLport lower/upper_bound len/half walk: count = Last - First (`sar 3`); while (count > 0) { half = count / 2;
// middle = first + half; if (!(key < middle->first)) { first = middle + 1; count -= half + 1; } else count = half; } return first.
// Layout: STLport vector {First, Last, EndOfStorage} at +0 (the empty comparator follows it in the real CVectorMap); the
// iterator is a plain entry pointer returned in eax. Key compare is unsigned `jae`/`jb`; the key is read once before the loop.
// Shape: `count -= half + 1` comes out as `or reg,-1 ; sub ; add` (count += -1 - half).
namespace std {
template <class _T1, class _T2> struct pair {
    _T1 first;
    _T2 second;
};
}
class CThing;
class CTCCreatureModeManager {
public:
    class CModeBase;
    class CRegisteredModeInfo {    // Ego_r.pdb 4 bytes
    public:
        CModeBase* (__cdecl* AllocFunc)(CThing*);
    };
};
template <class K, class V> class CKeyPairCompareLess {
public:
    bool operator()(const std::pair<K, V>& entry, const K& key) const { return entry.first < key; }
    bool operator()(const K& key, const std::pair<K, V>& entry) const { return key < entry.first; }
};
template <class K, class V, class C> class CVectorMap {
public:
    typedef std::pair<K, V> value_type;
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
template class CVectorMap<unsigned long, CTCCreatureModeManager::CRegisteredModeInfo,
                          CKeyPairCompareLess<unsigned long, CTCCreatureModeManager::CRegisteredModeInfo> >;
